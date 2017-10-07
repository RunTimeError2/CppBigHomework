/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.cpp
 * 摘要：服务员端主界面功能实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QStandardItemModel>

//默认使用的端口
#define DEFAULT_PORT 8000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Waiter");
    this->setWindowFlags(Qt::FramelessWindowHint);
    status=false;
    port=DEFAULT_PORT;
    serverIP=new QHostAddress();
    select_feedback=false;
    msg_target=-1;
    isRun=false;
    tmp_dishstate.clear();
    dataprocessing=new wDataProcessing;

    connect(ui->loginBtn,SIGNAL(clicked()),this,SLOT(btn_Login()));
    connect(ui->registerBtn,SIGNAL(clicked()),this,SLOT(btn_Register()));
    connect(ui->btnSendMsg,SIGNAL(clicked()),this,SLOT(slot_sendmsg()));
    connect(ui->btnCloseWindow,SIGNAL(clicked()),this,SLOT(slot_closeWindow()));
    connect(ui->btnMinWindow,SIGNAL(clicked()),this,SLOT(slot_minWindow()));

    connect(ui->tableViewSelect,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_tableviewClicked()));

    connect(dataprocessing,SIGNAL(login_success()),this,SLOT(slot_login_success()));
    connect(dataprocessing,SIGNAL(login_usernameerror()),this,SLOT(slot_login_usernameerror()));
    connect(dataprocessing,SIGNAL(login_passworderror()),this,SLOT(slot_login_passworderror()));
    connect(dataprocessing,SIGNAL(register_success()),this,SLOT(slot_register_success()));
    connect(dataprocessing,SIGNAL(register_usernameerror()),this,SLOT(slot_register_usernameerror()));
    connect(dataprocessing,SIGNAL(waiter_getallinfo()),this,SLOT(slot_waiter_getallinfo()));
    connect(dataprocessing,SIGNAL(waiter_singlemessage(QString)),this,SLOT(slot_singlemessage(QString)));

    ui->tableViewSelect->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewSelect->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewSelect->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDishState->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewDishState->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewDishState->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->frameLogin->setVisible(true);
    ui->frameMain->setVisible(false);

    InitLoginForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 连接成功
 */
void MainWindow::slotConnected() {
    status=true;
    if(state) { //将登录信息编码后发给服务器端
        QByteArray loginData=dataprocessing->EncodingWaiterLogin(ui->unLineEdit->text(),ui->pwLineEdit->text());
        tcpSocket->write(loginData,loginData.length());
    }
    else { //将注册信息编码后发给服务器端
        QByteArray registerData=dataprocessing->EncodingWaiterRegister(ui->UserNameLineEdit->text(),ui->PasswordLineEdit->text());
        tcpSocket->write(registerData,registerData.length());
    }
}

/*
 * 断开连接
 */
void MainWindow::slotDisconnected() {
    if(isRun) {
        QMessageBox::information(this,"Disconnected","与服务器端断开连接，请重新运行服务员端！");
        this->close();
    }
}

/*
 * 收到消息
 */
void MainWindow::dataReceived() {
    while(tcpSocket->bytesAvailable()>0) {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());

        dataprocessing->ProcessingData(datagram);
    }
}

/*
 * 登录按钮
 */
void MainWindow::btn_Login() {
    state=true;
    if(status) {
        status=false;
        tcpSocket->close();
    }
    if(!status) {
        QString ip=ui->serverIPLineEdit->text();
        if(!serverIP->setAddress(ip)) {
            QMessageBox::information(this,tr("Error"),tr("请输入有效的IP地址"));
            return;
        }
        QString username=ui->unLineEdit->text();
        if(username=="") {
            QMessageBox::information(this,tr("Error"),tr("用户名不能为空"));
            return;
        }
        QString password=ui->pwLineEdit->text();
        if(password.length()<6) {
            QMessageBox::information(this,tr("Error"),tr("密码至少6位"));
            return;
        }
        tcpSocket=new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        tcpSocket->connectToHost(*serverIP,port);
    }
}

/*
 * 登录成功
 */
void MainWindow::slot_login_success() {
    QByteArray msg;
    msg.append(TOKEN_WAITER_GETALLINFO);
    tcpSocket->write(msg,1); //发送获取所有信息请求
    QMessageBox::information(this,tr("success"),tr("登录成功"));
    isRun=true;
    ui->frameMain->setVisible(true);
    ui->frameLogin->setVisible(false);
    InitMainForm();
}

/*
 * 登录失败，密码错误
 */
void MainWindow::slot_login_passworderror() {
    QMessageBox::information(this,tr("failure"),tr("密码错误"));
}

/*
 * 登录失败，用户不存在
 */
void MainWindow::slot_login_usernameerror() {
    QMessageBox::information(this,tr("failure"),tr("用户不存在"));
}

/*
 * 注册按钮
 */
void MainWindow::btn_Register() {
    state=false;
    if(status) {
        status=false;
        tcpSocket->close();
    }

    if(!status) {
        QString ip=ui->serverIPLineEdit->text();
        if(!serverIP->setAddress(ip)) {
            QMessageBox::information(this,tr("Error"),tr("请输入有效的IP地址"));
            return;
        }
        QString password=ui->PasswordLineEdit->text();
        QString pwconfirm=ui->ConfirmLineEdit->text();
        if(pwconfirm!=password) {
            QMessageBox::information(this,tr("Error"),tr("请输入相同的密码"));
            return;
        }
        if(password.length()<6) {
            QMessageBox::information(this,tr("Error"),tr("密码至少6位"));
            return;
        }
        QString username=ui->UserNameLineEdit->text();
        if(username=="") {
            QMessageBox::information(this,tr("Error"),tr("用户名不能为空"));
            return;
        }
        tcpSocket=new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        tcpSocket->connectToHost(*serverIP,port);
    }
}

/*
 * 注册成功
 */
void MainWindow::slot_register_success() {
    QMessageBox::information(this,tr("success"),tr("注册成功"));
}

/*
 * 注册失败，用户名已注册
 */
void MainWindow::slot_register_usernameerror() {
    QMessageBox::information(this,tr("failure"),tr("用户名已注册"));
}

/*
 * 获取所有所需信息，并将其显示
 */
void MainWindow::slot_waiter_getallinfo() {
    //餐桌表
    QStandardItemModel *model1=new QStandardItemModel;
    model1->setColumnCount(6);
    model1->setHeaderData(0,Qt::Horizontal,"序号");
    model1->setHeaderData(1,Qt::Horizontal,"餐桌类型");
    model1->setHeaderData(2,Qt::Horizontal,"餐桌容纳人数");
    model1->setHeaderData(3,Qt::Horizontal,"状态");
    model1->setHeaderData(4,Qt::Horizontal,"菜品数量");
    model1->setHeaderData(5,Qt::Horizontal," ");
    ui->tableViewSelect->setModel(model1);
    ui->tableViewSelect->setColumnWidth(0,50);
    ui->tableViewSelect->setColumnWidth(1,110);
    ui->tableViewSelect->setColumnWidth(2,80);
    ui->tableViewSelect->setColumnWidth(3,100);
    ui->tableViewSelect->setColumnWidth(4,80);
    ui->tableViewSelect->setColumnWidth(5,80);
    ButtonDelegate *delegate=new ButtonDelegate(this,"认领");
    ui->tableViewSelect->setItemDelegateForColumn(5,delegate);
    connect(delegate,SIGNAL(buttonClicked(int,int)),this,SLOT(slot_btnSelectTable(int,int)));
    for(int i=0;i<dataprocessing->tableInfo.count();i++) {
        model1->setItem(i,0,new QStandardItem(QString::number(i+1)));
        model1->setItem(i,1,new QStandardItem(dataprocessing->tableInfo.at(i).type));
        model1->setItem(i,2,new QStandardItem(QString::number(dataprocessing->tableInfo.at(i).fitin)));

        if(dataprocessing->tableInfo.at(i).isValid)
            model1->setItem(i,3,new QStandardItem("无顾客"));
        else
            if(!dataprocessing->tableInfo.at(i).isSelected)
                model1->setItem(i,3,new QStandardItem("有顾客，未认领"));
            else
                if(dataprocessing->tableInfo.at(i).isMine)
                    model1->setItem(i,3,new QStandardItem("已认领"));
                else
                    model1->setItem(i,3,new QStandardItem("已被其他服务员认领"));
        model1->setItem(i,4,new QStandardItem(QString::number(dataprocessing->tableInfo.at(i).dishcount)));
        model1->setItem(i,5,new QStandardItem(" "));
        model1->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,3)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,4)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewSelect->verticalHeader()->setVisible(false);

    if(select_feedback && select_feedback>=0 && select_feedback<ui->tableViewSelect->model()->rowCount()) {
        if(ui->tableViewSelect->model()->data(ui->tableViewSelect->model()->index(table_feedback,3)).toString()=="已认领")
            QMessageBox::information(this,"success",QString("选桌成功\n桌号：%1").arg(table_feedback+1));
        else
            QMessageBox::information(this,"failure","选桌失败");
        select_feedback=false;
    }

    //菜品信息表
    QStandardItemModel *model2=new QStandardItemModel;
    model2->setColumnCount(4);
    model2->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model2->setHeaderData(1,Qt::Horizontal,"菜品编号");
    model2->setHeaderData(2,Qt::Horizontal,"菜品状态");
    model2->setHeaderData(3,Qt::Horizontal,"菜品所属桌号");
    ui->tableViewDishState->setModel(model2);
    for(int i=0;i<dataprocessing->dishInfo.count();i++) {
        model2->setItem(i,0,new QStandardItem(dataprocessing->dishInfo.at(i).name));
        model2->setItem(i,1,new QStandardItem(QString::number(dataprocessing->dishInfo.at(i).index)));
        if(dataprocessing->dishInfo.at(i).state==DISH_ABORTED)
            model2->setItem(i,2,new QStandardItem("已取消"));
        if(dataprocessing->dishInfo.at(i).state==DISH_CHOSEN_UNSTARTED)
            model2->setItem(i,2,new QStandardItem("未开始"));
        if(dataprocessing->dishInfo.at(i).state==DISH_STARTED)
            model2->setItem(i,2,new QStandardItem("正在做"));
        if(dataprocessing->dishInfo.at(i).state==DISH_UNCHOSEN)
            model2->setItem(i,2,new QStandardItem("未认领"));
        if(!tmp_dishstate.contains(dataprocessing->dishInfo.at(i).index))
            tmp_dishstate.insert(dataprocessing->dishInfo.at(i).index,false);
        if(dataprocessing->dishInfo.at(i).state==DISH_FINISHED) {
            model2->setItem(i,2,new QStandardItem("已完成"));
            if(!tmp_dishstate.contains(dataprocessing->dishInfo.at(i).index))
                tmp_dishstate.insert(dataprocessing->dishInfo.at(i).index,true);
            else
                if(!tmp_dishstate[dataprocessing->dishInfo.at(i).index]) {
                    tmp_dishstate[dataprocessing->dishInfo.at(i).index]=true;
                    ui->listMsg->addItem(QString("%1号桌，编号%2的菜品 %3 已完成，请及时上菜！").arg(1+dataprocessing->dishInfo.at(i).tableindex).arg(dataprocessing->dishInfo.at(i).index).arg(dataprocessing->dishInfo.at(i).name));
                }
        }
        model2->setItem(i,3,new QStandardItem(QString::number(dataprocessing->dishInfo.at(i).tableindex+1)));
        model2->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewDishState->verticalHeader()->setVisible(false);

    //消息列表
    for(int i=0;i<dataprocessing->tmp_msg.count();i++)
        ui->listMsg->addItem(dataprocessing->tmp_msg.at(i));
    dataprocessing->tmp_msg.clear();
}

/*
 * 服务员选桌
 */
void MainWindow::slot_btnSelectTable(int row,int) {
    if(row==-1)
        row=ui->tableViewSelect->currentIndex().row();
    QString currentstate=ui->tableViewSelect->model()->data(ui->tableViewSelect->model()->index(row,3)).toString();
    if(currentstate=="无顾客") {
        QMessageBox::information(this,"Error","餐桌无顾客，无法认领");
        return;
    }
    if(currentstate=="已认领") {
        QMessageBox::information(this,"Error","餐桌已被认领，无法认领");
        return;
    }
    if(currentstate=="已被其他服务员认领") {
        QMessageBox::information(this,"Error","餐桌已被其他服务员认领，无法认领");
        return;
    }
    QByteArray msg;
    msg.append(TOKEN_WAITER_CHOOSETABLE);
    msg.append((unsigned char)(row/256));
    msg.append((unsigned char)(row%256));
    select_feedback=true;
    table_feedback=row;
    tcpSocket->write(msg,msg.length());
}

/*
 * 点击tableview时，设定发送消息的对象
 */
void MainWindow::slot_tableviewClicked() {
    int row=ui->tableViewSelect->currentIndex().row();
    ui->labelMsg->setText(QString("消息  给%1号桌").arg(row+1));
    msg_target=row;
}

/*
 * 发送消息
 */
void MainWindow::slot_sendmsg() {
    if(msg_target==-1) {
        QMessageBox::information(this,"Error","必须先选定一个餐桌");
        return;
    }
    if(ui->tableViewSelect->model()->data(ui->tableViewSelect->model()->index(msg_target,3)).toString()!="已认领") {
        QMessageBox::information(this,"Error","只能给自己认领的顾客发消息");
        return;
    }
    QString msg=ui->lineEditMsg->text();
    QByteArray ans;
    ans.append(TOKEN_WAITER_MSGTOGUEST);
    ans.append((unsigned char)(msg_target/256));
    ans.append((unsigned char)(msg_target%256));
    ans.append((unsigned char)(msg.length()/256));
    ans.append((unsigned char)(msg.length()%256));
    ans.append(msg);
    tcpSocket->write(ans,ans.length());
    ui->listMsg->addItem(QString("[给%1桌的消息]%2").arg(msg_target+1).arg(ui->lineEditMsg->text()));
    ui->lineEditMsg->setText("");
}

/*
 * 收到单条消息
 */
void MainWindow::slot_singlemessage(QString msg) {
    ui->listMsg->addItem(msg);
}

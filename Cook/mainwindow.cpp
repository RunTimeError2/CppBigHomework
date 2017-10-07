/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.cpp
 * 摘要：厨师端主界面功能实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHostInfo>

//默认使用的端口
#define DEFAULT_PORT 8000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Cook");
    dataprocessing=new cDataProcessing;
    isRun=false;
    this->setWindowFlags(Qt::FramelessWindowHint);

    set_feedback=false;
    select_feedback=false;

    status=false;
    port=DEFAULT_PORT;
    serverIP=new QHostAddress();

    connect(ui->loginBtn,SIGNAL(clicked()),this,SLOT(btn_Login()));                                                                             //登录按钮
    connect(ui->registerBtn,SIGNAL(clicked()),this,SLOT(btn_Register()));                                                                       //注册按钮
    connect(ui->btnRefreshValiddish,SIGNAL(clicked()),this,SLOT(slot_btnRefreshValidDish()));                                                   //刷新"可认领菜品列表"按钮
    connect(ui->btnRefreshSelectedDish,SIGNAL(clicked()),this,SLOT(slot_btnRefreshSelectedDish()));                                             //刷新"已认领菜品列表"按钮
    connect(ui->btnCloseWindow,SIGNAL(clicked()),this,SLOT(slot_closeWindow()));
    connect(ui->btnMinWindow,SIGNAL(clicked()),this,SLOT(slot_minWindow()));

    connect(dataprocessing,SIGNAL(login_success()),this,SLOT(slot_login_success()));                                                            //登录成功
    connect(dataprocessing,SIGNAL(login_usernameerror()),this,SLOT(slot_login_usernameerror()));                                                //登录失败，用户不存在
    connect(dataprocessing,SIGNAL(login_passworderror()),this,SLOT(slot_login_passworderror()));                                                //登录失败，密码错误
    connect(dataprocessing,SIGNAL(register_success()),this,SLOT(slot_register_success()));                                                      //注册成功
    connect(dataprocessing,SIGNAL(register_usernameerror()),this,SLOT(slot_register_usernameerror()));                                          //注册失败，用户名已注册
    connect(dataprocessing,SIGNAL(cook_getunstarted(QList<SingleDishOrdered>&)),this,SLOT(slot_cook_getunstarted(QList<SingleDishOrdered>&)));  //获取可认领的菜品列表并显示
    connect(dataprocessing,SIGNAL(cook_getselected(QList<SingleDishOrdered>&)),this,SLOT(slot_cook_getselected(QList<SingleDishOrdered>&)));    //获取已认领的菜品列表并显示

    ui->frameLogin->setVisible(true);
    ui->frameMain->setVisible(false);

    ui->tvSelectDish->setSelectionMode(QAbstractItemView::SingleSelection);     //仅能选择一项
    ui->tvSelectDish->setSelectionBehavior(QAbstractItemView::SelectRows);      //整行选择
    ui->tvSelectDish->setEditTriggers(QAbstractItemView::NoEditTriggers);       //不可编辑
    ui->tvSelected->setSelectionMode(QAbstractItemView::SingleSelection);       //仅能选择一项
    ui->tvSelected->setSelectionBehavior(QAbstractItemView::SelectRows);        //整行选择
    ui->tvSelected->setEditTriggers(QAbstractItemView::NoEditTriggers);         //不可编辑

    setUpLoginForm();
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
        QByteArray loginData=dataprocessing->EncodingCookLogin(ui->unLineEdit->text(),ui->pwLineEdit->text());
        tcpSocket->write(loginData,loginData.length());
    }
    else { //将注册信息编码后发给服务器端
        QByteArray registerData=dataprocessing->EncodingCookRegister(ui->UserNameLineEdit->text(),ui->PasswordLineEdit->text());
        tcpSocket->write(registerData,registerData.length());
    }
}

/*
 * 断开连接
 */
void MainWindow::slotDisconnected() {
    if(isRun) {
        QMessageBox::information(this,"Disconnected","与服务器端断开连接，请重新运行厨师端！");
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
 * 登录成功，同时获取可认领菜品信息
 */
void MainWindow::slot_login_success() {
    QByteArray msg;
    msg.append(TOKEN_COOK_GETALLINFO);
    tcpSocket->write(msg,1);
    QMessageBox::information(this,tr("success"),tr("登录成功"));
    isRun=true;
    setUpMainForm();
    ui->frameLogin->setVisible(false);
    ui->frameMain->setVisible(true);
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
        //status=true;
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
 * 厨师设置菜品状态
 */
void MainWindow::slot_cook_setdishstate(int index,int state) {
    QByteArray msg;
    msg.append(TOKEN_COOK_SETDISHSTATE);
    msg.append((unsigned char)(index/256));
    msg.append((unsigned char)(index%256));
    msg.append((unsigned char)state);
    tcpSocket->write(msg,msg.length());
    set_feedback=true;
    set_index=index;
    set_state=state;
}

/*
 * 获取认领的菜品列表，并显示在QTableView中
 */
void MainWindow::slot_cook_getselected(QList<SingleDishOrdered> &dishSelected) {
    QStandardItemModel *model=new QStandardItemModel;
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model->setHeaderData(1,Qt::Horizontal,"菜品编号");
    model->setHeaderData(2,Qt::Horizontal,"状态");
    model->setHeaderData(3,Qt::Horizontal," ");
    model->setHeaderData(4,Qt::Horizontal," ");
    ui->tvSelected->setModel(model);
    ButtonDelegate *delegate1=new ButtonDelegate(this,"设为'正在做'");
    ButtonDelegate *delegate2=new ButtonDelegate(this,"设为'已完成'");
    ui->tvSelected->setItemDelegateForColumn(3,delegate1);
    ui->tvSelected->setItemDelegateForColumn(4,delegate2);
    connect(delegate1,SIGNAL(buttonClicked(int,int)),this,SLOT(slot_btnToDoing(int,int)));
    connect(delegate2,SIGNAL(buttonClicked(int,int)),this,SLOT(slot_btnToFinished(int,int)));
    bool select_success=false,set_success=false;
    for(int i=0;i<dishSelected.count();i++) {
        model->setItem(i,0,new QStandardItem(dishSelected.at(i).name));
        model->setItem(i,1,new QStandardItem(QString::number(dishSelected.at(i).index)));
        switch(dishSelected.at(i).state) {
            case DISH_ABORTED: {
                model->setItem(i,2,new QStandardItem("已取消"));
                break;
            }
            case DISH_CHOSEN_UNSTARTED: {
                model->setItem(i,2,new QStandardItem("未开始"));
                break;
            }
            case DISH_FINISHED: {
                model->setItem(i,2,new QStandardItem("已完成"));
                break;
            }
            case DISH_STARTED: {
                model->setItem(i,2,new QStandardItem("正在做"));
                break;
            }
            default: {
                model->setItem(i,2,new QStandardItem("未认领"));
                break;
            }
        }

        if(select_feedback)
            if(dishSelected.at(i).index==select_index) {
                select_success=true;
                select_feedback=false;
            }
        if(set_feedback)
            if(dishSelected.at(i).index==set_index && dishSelected.at(i).state==set_state) {
                set_success=true;
                set_feedback=false;
            }

        //居中对齐
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        //不可编辑
        model->item(i,0)->setFlags(model->item(i,0)->flags() & (~Qt::ItemIsEditable));
        model->item(i,1)->setFlags(model->item(i,1)->flags() & (~Qt::ItemIsEditable));
        model->item(i,2)->setFlags(model->item(i,2)->flags() & (~Qt::ItemIsEditable));
    }
    ui->tvSelected->setColumnWidth(0,120);
    ui->tvSelected->setColumnWidth(1,80);
    ui->tvSelected->setColumnWidth(2,80);
    ui->tvSelected->setColumnWidth(3,80);
    ui->tvSelected->setColumnWidth(4,80);
    if(select_success)
        QMessageBox::information(this,"success","认领菜品成功！");
    if(set_success)
        QMessageBox::information(this,"success","设置菜品状态成功！");
    if(select_feedback) {
        QMessageBox::information(this,"failure","认领菜品失败！");
        select_feedback=false;
    }
    if(set_feedback) {
        QMessageBox::information(this,"failure","设置菜品状态失败！");
        set_feedback=false;
    }
}

/*
 * 获取可认领的菜品列表并显示
 */
void MainWindow::slot_cook_getunstarted(QList<SingleDishOrdered> &dishAvailable) {
    QStandardItemModel *model=new QStandardItemModel;
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model->setHeaderData(1,Qt::Horizontal,"菜品编号");
    model->setHeaderData(2,Qt::Horizontal,"状态");
    model->setHeaderData(3,Qt::Horizontal," ");
    ui->tvSelectDish->setModel(model);
    ButtonDelegate *delegate=new ButtonDelegate(this,"认领菜品");
    ui->tvSelectDish->setItemDelegateForColumn(3,delegate);
    connect(delegate,SIGNAL(buttonClicked(int,int)),this,SLOT(slot_btnSelectdish(int,int)));
    for(int i=0;i<dishAvailable.count();i++) {
        model->setItem(i,0,new QStandardItem(dishAvailable.at(i).name));
        model->setItem(i,1,new QStandardItem(QString::number(dishAvailable.at(i).index)));
        model->setItem(i,2,new QStandardItem("未认领"));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tvSelectDish->setColumnWidth(0,200);
    ui->tvSelectDish->setColumnWidth(0,150);
    ui->tvSelectDish->setColumnWidth(0,100);
}

/*
 * 刷新"可认领菜品表"
 */
void MainWindow::slot_btnRefreshValidDish() {
    QByteArray msg;
    msg.append(TOKEN_COOK_GETALLINFO);
    tcpSocket->write(msg,1);
}

/*
 * 认领菜品
 */
void MainWindow::slot_btnSelectdish(int row,int /*col*/) {
    if(row==-1)
        row=ui->tvSelectDish->currentIndex().row();
    if(row>=0 && row<ui->tvSelectDish->model()->rowCount()) {
        int index=ui->tvSelectDish->model()->data(ui->tvSelectDish->model()->index(row,1)).toInt();
        QByteArray msg;
        msg.append(TOKEN_COOK_SELECTDISH);
        msg.append((unsigned char)(index/256));
        msg.append((unsigned char)(index%256));
        tcpSocket->write(msg,msg.length());
        select_feedback=true;
        select_index=index;
    }
    else
        QMessageBox::information(this,"Error","必须选择一个菜品");
}

/*
 * 刷新"已认领菜品"表
 */
void MainWindow::slot_btnRefreshSelectedDish() {
    QByteArray msg;
    msg.append(TOKEN_COOK_GETALLINFO);
    tcpSocket->write(msg,1);
}

/*
 * 设为"正在做"
 */
void MainWindow::slot_btnToDoing(int row,int /*col*/) {
    QByteArray msg;
    msg.append(TOKEN_COOK_SETDISHSTATE);
    if(row==-1)
        row=ui->tvSelected->currentIndex().row();
    QString currentstate=ui->tvSelected->model()->data(ui->tvSelected->model()->index(row,2)).toString();
    if(currentstate=="已完成") {
        QMessageBox::information(this,"Error","菜品已完成，无法设置状态");
        return;
    }
    if(currentstate=="正在做") {
        QMessageBox::information(this,"Error","菜品已开始制作，无法设置为正在做");
        return;
    }
    if(currentstate=="已取消") {
        QMessageBox::information(this,"Error","菜品已取消，无法设置状态");
        return;
    }
    if(row>=0 && row<ui->tvSelected->model()->rowCount()) {
        int index=ui->tvSelected->model()->data(ui->tvSelected->model()->index(row,1)).toInt();
        msg.append((unsigned char)(index/256));
        msg.append((unsigned char)(index%256));
        msg.append(DISH_STARTED);
        set_feedback=true;
        set_index=index;
        set_state=DISH_STARTED;
        tcpSocket->write(msg,msg.length());
    }
}

/*
 * 设为"已完成"
 */
void MainWindow::slot_btnToFinished(int row,int /*col*/) {
    QByteArray msg;
    msg.append(TOKEN_COOK_SETDISHSTATE);
    if(row==-1)
        row=ui->tvSelected->currentIndex().row();
    QString currentstate=ui->tvSelected->model()->data(ui->tvSelected->model()->index(row,2)).toString();
    if(currentstate=="已完成") {
        QMessageBox::information(this,"Error","菜品已完成，无法设置");
        return;
    }
    if(currentstate=="已取消") {
        QMessageBox::information(this,"Error","菜品已取消，无法设置");
        return;
    }
    if(currentstate=="未开始") {
        QMessageBox::information(this,"Error","菜品未开始制作，无法设置");
        return;
    }
    if(row>=0 && row<ui->tvSelected->model()->rowCount()) {
        int index=ui->tvSelected->model()->data(ui->tvSelected->model()->index(row,1)).toInt();
        msg.append((unsigned char)(index/256));
        msg.append((unsigned char)(index%256));
        msg.append(DISH_FINISHED);
        set_feedback=true;
        set_index=index;
        set_state=DISH_FINISHED;
        tcpSocket->write(msg,msg.length());
    }
}

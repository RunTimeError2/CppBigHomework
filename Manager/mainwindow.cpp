/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.cpp
 * 摘要：经理端主界面功能实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QStandardItemModel>
#include "buttondelegate.h"

//默认使用的端口
#define DEFAULT_PORT 8000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Manager");
    status=false;
    port=DEFAULT_PORT;
    serverIP=new QHostAddress();
    isRun=false;
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->loginBtn,SIGNAL(clicked()),this,SLOT(btn_Login()));
    connect(ui->registerBtn,SIGNAL(clicked()),this,SLOT(btn_Register()));
    connect(ui->btnRefreshReport,SIGNAL(clicked()),this,SLOT(manager_getreport_request()));
    connect(ui->btnCloseWindow,SIGNAL(clicked()),this,SLOT(slot_closeWindow()));
    connect(ui->btnMinWindow,SIGNAL(clicked()),this,SLOT(slot_minWindow()));

    dataprocessing=new mDataProcessing;
    connect(dataprocessing,SIGNAL(login_success()),this,SLOT(slot_login_success()));
    connect(dataprocessing,SIGNAL(login_usernameerror()),this,SLOT(slot_login_usernameerror()));
    connect(dataprocessing,SIGNAL(login_passworderror()),this,SLOT(slot_login_passworderror()));
    connect(dataprocessing,SIGNAL(register_success()),this,SLOT(slot_register_success()));
    connect(dataprocessing,SIGNAL(register_usernameerror()),this,SLOT(slot_register_usernameerror()));
    connect(dataprocessing,SIGNAL(manager_getreport()),this,SLOT(manager_displayReport()));

    ui->tableViewWaiter->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewWaiter->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewWaiter->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDishEvaluation->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewDishEvaluation->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewDishEvaluation->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewRecord->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewRecord->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewRecord->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewCook->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewCook->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCook->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewWaiterEvaluation->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewWaiterEvaluation->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewWaiterEvaluation->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setupLoginForm();
    ui->frameLogin->setVisible(true);
    ui->frameMain->setVisible(false);
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
        QByteArray loginData=dataprocessing->EncodingManagerLogin(ui->unLineEdit->text(),ui->pwLineEdit->text());
        tcpSocket->write(loginData,loginData.length());
    }
    else { //将注册信息编码后发给服务器端
        QByteArray registerData=dataprocessing->EncodingManagerRegister(ui->UserNameLineEdit->text(),ui->PasswordLineEdit->text());
        tcpSocket->write(registerData,registerData.length());
    }
}

/*
 * 断开连接
 */
void MainWindow::slotDisconnected() {
    if(isRun) {
        QMessageBox::information(this,"Disconnected","与服务器端断开连接，请重新运行经理端！");
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
    manager_getreport_request();
    QMessageBox::information(this,tr("success"),tr("登录成功"));
    setupMainForm();
    isRun=true;
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
 * 发送获取报表请求
 */
void MainWindow::manager_getreport_request() {
    QByteArray msg;
    msg.append(TOKEN_MANAGER_REPORT);
    tcpSocket->write(msg,1);
}

/*
 * 显示报表
 */
void MainWindow::manager_displayReport() {
    //服务员工作记录
    QStandardItemModel *model1=new QStandardItemModel;
    model1->setColumnCount(7);
    model1->setHeaderData(0,Qt::Horizontal,"序号");
    model1->setHeaderData(1,Qt::Horizontal,"服务员用户名");
    model1->setHeaderData(2,Qt::Horizontal,"当天认领桌数");
    model1->setHeaderData(3,Qt::Horizontal,"总认领桌数");
    model1->setHeaderData(4,Qt::Horizontal,"收到评价数");
    model1->setHeaderData(5,Qt::Horizontal,"平均评分");
    model1->setHeaderData(6,Qt::Horizontal,"   ");
    ButtonDelegate *delegate=new ButtonDelegate;
    ui->tableViewWaiter->setModel(model1);
    ui->tableViewWaiter->setItemDelegateForColumn(6,delegate);
    connect(delegate,SIGNAL(buttonClicked(int,int)),this,SLOT(tableView_buttonclicked(int,int)));
    for(int i=0;i<dataprocessing->report->waiter.count();i++) {
        model1->setItem(i,0,new QStandardItem(QString::number(i+1)));
        model1->setItem(i,1,new QStandardItem(dataprocessing->report->waiter.at(i).username));
        model1->setItem(i,2,new QStandardItem(QString::number(dataprocessing->report->waiter.at(i).tmp_record)));
        model1->setItem(i,3,new QStandardItem(QString::number(dataprocessing->report->waiter.at(i).record)));
        model1->setItem(i,4,new QStandardItem(QString::number(dataprocessing->report->waiter.at(i).evaluationCount)));
        model1->setItem(i,5,new QStandardItem(QString::number(dataprocessing->report->waiter.at(i).avaerageScore)));
        model1->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,3)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,4)->setTextAlignment(Qt::AlignCenter);
        model1->item(i,5)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewWaiter->setColumnWidth(0,40);
    ui->tableViewWaiter->setColumnWidth(1,90);
    ui->tableViewWaiter->setColumnWidth(2,80);
    ui->tableViewWaiter->setColumnWidth(3,80);
    ui->tableViewWaiter->setColumnWidth(4,70);
    ui->tableViewWaiter->setColumnWidth(5,60);

    //厨师工作记录
    QStandardItemModel *model2=new QStandardItemModel;
    model2->setColumnCount(4);
    model2->setHeaderData(0,Qt::Horizontal,"序号");
    model2->setHeaderData(1,Qt::Horizontal,"厨师用户名");
    model2->setHeaderData(2,Qt::Horizontal,"当天完成菜品数");
    model2->setHeaderData(3,Qt::Horizontal,"总共完成菜品数");
    ui->tableViewCook->setModel(model2);
    for(int i=0;i<dataprocessing->report->cook.count();i++) {
        model2->setItem(i,0,new QStandardItem(QString::number(i+1)));
        model2->setItem(i,1,new QStandardItem(dataprocessing->report->cook.at(i).username));
        model2->setItem(i,2,new QStandardItem(QString::number(dataprocessing->report->cook.at(i).tmp_record)));
        model2->setItem(i,3,new QStandardItem(QString::number(dataprocessing->report->cook.at(i).record)));
        model2->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewCook->setColumnWidth(0,80);
    ui->tableViewCook->setColumnWidth(1,100);
    ui->tableViewCook->setColumnWidth(2,120);
    ui->tableViewCook->setColumnWidth(3,120);

    //初始化服务员评价表
    QStandardItemModel *model3=new QStandardItemModel;
    model3->setColumnCount(3);
    model3->setHeaderData(0,Qt::Horizontal,"序号");
    model3->setHeaderData(1,Qt::Horizontal,"评价");
    model3->setHeaderData(2,Qt::Horizontal,"评分");
    ui->tableViewWaiterEvaluation->setModel(model3);
    ui->tableViewWaiterEvaluation->setColumnWidth(0,80);
    ui->tableViewWaiterEvaluation->setColumnWidth(1,200);
    ui->tableViewWaiterEvaluation->setColumnWidth(2,100);

    //菜品评价信息
    QStandardItemModel *model4=new QStandardItemModel;
    model4->setColumnCount(4);
    model4->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model4->setHeaderData(1,Qt::Horizontal,"顾客手机号");
    model4->setHeaderData(2,Qt::Horizontal,"评价");
    model4->setHeaderData(3,Qt::Horizontal,"评分");
    float score_sum=0;
    ui->tableViewDishEvaluation->setModel(model4);
    for(int i=0;i<dataprocessing->report->dishEvaluation.count();i++) {
        model4->setItem(i,0,new QStandardItem(dataprocessing->report->dishEvaluation.at(i).dishname));
        model4->setItem(i,1,new QStandardItem(dataprocessing->report->dishEvaluation.at(i).phonenumber));
        model4->setItem(i,2,new QStandardItem(dataprocessing->report->dishEvaluation.at(i).comment));
        model4->setItem(i,3,new QStandardItem(QString::number(dataprocessing->report->dishEvaluation.at(i).score)));
        score_sum+=dataprocessing->report->dishEvaluation.at(i).score;
        model4->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model4->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model4->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model4->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewDishEvaluation->setColumnWidth(0,120);
    ui->tableViewDishEvaluation->setColumnWidth(1,120);
    ui->tableViewDishEvaluation->setColumnWidth(2,150);
    ui->tableViewDishEvaluation->setColumnWidth(3,100);
    int average100=(int)(score_sum/(float)dataprocessing->report->dishEvaluation.count()*100.0);
    ui->labelEvaluation->setText(QString("  总共有%1条评价，所有菜品平均评价分数为%2").arg(dataprocessing->report->dishEvaluation.count()).arg((float)average100/100.0));

    //交易记录
    QStandardItemModel *model5=new QStandardItemModel;
    model5->setColumnCount(3);
    model5->setHeaderData(0,Qt::Horizontal,"顾客手机号");
    model5->setHeaderData(1,Qt::Horizontal,"时间");
    model5->setHeaderData(2,Qt::Horizontal,"金额");
    float sum_price=0;
    ui->tableViewRecord->setModel(model5);
    for(int i=0;i<dataprocessing->report->payRecordList.count();i++) {
        model5->setItem(i,0,new QStandardItem(dataprocessing->report->payRecordList.at(i).phonenumber));
        model5->setItem(i,1,new QStandardItem(dataprocessing->report->payRecordList.at(i).datetime));
        model5->setItem(i,2,new QStandardItem(dataprocessing->report->payRecordList.at(i).price));
        sum_price+=dataprocessing->report->payRecordList.at(i).price.toFloat();
        model5->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model5->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model5->item(i,2)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableViewRecord->setColumnWidth(0,120);
    ui->tableViewRecord->setColumnWidth(1,280);
    ui->tableViewRecord->setColumnWidth(2,120);
    ui->labelRecord->setText(QString("  总共有%1条交易记录，金额总计为%2元").arg(dataprocessing->report->payRecordList.count()).arg(sum_price));
}

/*
 * 显示某个服务员的评价
 */
void MainWindow::tableView_buttonclicked(int row,int /*column*/) {
    bool flag=false;
    if(dataprocessing->report->waiterEvaluation.count()>0 && row>=0 && row<ui->tableViewWaiter->model()->rowCount()) {
        QStandardItemModel *model=new QStandardItemModel;
        model->setColumnCount(3);
        model->setHeaderData(0,Qt::Horizontal,"序号");
        model->setHeaderData(1,Qt::Horizontal,"评价");
        model->setHeaderData(2,Qt::Horizontal,"评分");
        ui->tableViewWaiterEvaluation->setModel(model);
        QString username=ui->tableViewWaiter->model()->data(ui->tableViewWaiter->model()->index(row,1)).toString();
        int count=0;
        for(int i=0;i<dataprocessing->report->waiterEvaluation.count();i++)
            if(dataprocessing->report->waiterEvaluation.at(i).username==username) {
                flag=true;
                model->setItem(count,0,new QStandardItem(QString::number(count+1)));
                model->setItem(count,1,new QStandardItem(dataprocessing->report->waiterEvaluation.at(i).comment));
                model->setItem(count,2,new QStandardItem(QString::number(dataprocessing->report->waiterEvaluation.at(i).score)));
                count++;
            }
    }
    if(!flag)
        QMessageBox::information(this,"Error","暂时没有评价");
}

/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.cpp
 * 摘要：服务器端主界面
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTcpSocket>

#define DEFAULT_PORT 8000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainLogin=new Login;

    mainlogic=new mainLogic;
    mainlogic->ReadAllData();

    setWindowTitle("Server");
    port=DEFAULT_PORT;
    ui->PortLineEdit->setText(QString::number(port));
    connect(ui->CreateBtn,SIGNAL(clicked()),this,SLOT(slotCreateServer()));
    connect(ui->ExitBtn,SIGNAL(clicked()),this,SLOT(slot_closeServer()));

    setupUI();
}

QString MainWindow::getIPAddress() {
    QString localHostName=QHostInfo::localHostName();
    QHostInfo hostInfo=QHostInfo::fromName(localHostName);
    QList<QHostAddress> listAddress=hostInfo.addresses();
    for(int i=0;i<listAddress.count();i++)
        if(listAddress.at(i).protocol()==QAbstractSocket::IPv4Protocol)
            return listAddress.at(i).toString(); //返回得到的第一个IPv4地址
    return "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

//新建服务器
void MainWindow::slotCreateServer() {
    //server=new TcpServer(this,port);
    server=new TcpServer(this,port,mainlogic); //========================================================

    server->serverLogin=mainLogin;

    connect(mainlogic,SIGNAL(updateAllOrderInfo()),server,SLOT(updateAllOrderInfo()));
    connect(mainlogic,SIGNAL(updateAllOrderInfoExceptMe(QString)),server,SLOT(updateAllOrderInfoExceptMe(QString)));
    connect(mainlogic,SIGNAL(MessagetoGuest(QByteArray,QString)),server,SLOT(slot_MessagetoGuest(QByteArray,QString)));
    connect(mainlogic,SIGNAL(MessagetoWaiter(QByteArray,QString)),server,SLOT(slot_MessagetoWaiter(QByteArray,QString)));
    //connect(server,SIGNAL(updateServer(QString,int)),this,SLOT(updateServer(QString,int)));
    ui->CreateBtn->setEnabled(false);
    ui->CreateBtn->setVisible(false);
    ui->ExitBtn->setEnabled(true);
    ui->ExitBtn->setVisible(true);
}

//收到消息并显示
/*void MainWindow::updateServer(QString,int) {

}*/

void MainWindow::setupUI() {
    this->setGeometry(300,300,600,450);
    this->setWindowFlags(Qt::FramelessWindowHint);
    bgLabel=new BGLabel("",ui->centralWidget,this);
    QImage *imageBG=new QImage;
    imageBG->load("img\\s_bg.png");
    bgLabel->setTitleBarHeight(450);
    bgLabel->setGeometry(0,0,600,450);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));

    QFont font("微软雅黑",16,75);
    ui->btnClose->setGeometry(560,0,40,40);
    ui->btnClose->setText("×");
    ui->btnClose->setParent(bgLabel);
    ui->btnClose->setFont(font);
    ui->btnClose->setStyleSheet("QPushButton { background-color: rgba(0,0,0,0); border: 1px solid rgba(0,0,0,0); }"
                                "QPushButton:hover { background-color: rgba(255,0,0,0.7); border: 1px solid rgba(0,0,0,0); }");

    ui->btnMin->setGeometry(520,0,40,40);
    ui->btnMin->setText("-");
    ui->btnMin->setParent(bgLabel);
    ui->btnMin->setFont(font);
    ui->btnMin->setStyleSheet("QPushButton { background-color: rgba(0,0,0,0); border: 1px solid rgba(0,0,0,0); }"
                              "QPushButton:hover { background-color: rgba(255,255,255,0.4); border: 1px solid rgba(0,0,0,0); }");

    connect(ui->btnClose,SIGNAL(clicked()),this,SLOT(slot_close()));
    connect(ui->btnMin,SIGNAL(clicked()),this,SLOT(slot_minimize()));

    ui->CreateBtn->setParent(bgLabel);
    ui->CreateBtn->setGeometry(150,320,300,50);
    ui->CreateBtn->setObjectName("btnCreate");
    ui->ExitBtn->setParent(bgLabel);
    ui->ExitBtn->setGeometry(150,320,300,50);
    ui->ExitBtn->setObjectName("btnExit");
    ui->ExitBtn->setVisible(false);
    ui->ExitBtn->setEnabled(false);

    ui->labelIP->setGeometry(150,250,300,15);
    ui->labelPort->setGeometry(150,270,300,15);
    ui->labelIP->setParent(bgLabel);
    ui->labelPort->setParent(bgLabel);
    ui->labelIP->setText("本机IP地址： "+getIPAddress());
    ui->labelPort->setText(QString("端口： %1").arg(DEFAULT_PORT));
    ui->labelIP->setAlignment(Qt::AlignCenter);
    ui->labelPort->setAlignment(Qt::AlignCenter);
    QFont fontlabel("微软雅黑",10,50);
    ui->labelIP->setFont(fontlabel);
    ui->labelPort->setFont(fontlabel);
    ui->CreateBtn->setFont(fontlabel);
    ui->ExitBtn->setFont(fontlabel);

    this->setStyleSheet("QPushButton#btnCreate {"
                        "    background-color: rgb(39,174,97);"
                        "    border: 2px solid rgb(39,174,97);"
                        "}"
                        "QPushButton#btnCreate:hover {"
                        "    background-color: rgb(87,215,141);"
                        "    border: 2px solid rgb(87,215,141);"
                        "}"
                        "QPushButton#btnExit {"
                        "    background-color: rgb(232,76,61);"
                        "    border: 2px solid rgb(232,76,61);"
                        "}"
                        "QPushButton#btnExit:hover {"
                        "    background-color: rgb(193,57,43);"
                        "    border: 2px solid rgb(193,57,43);"
                        "}");
}

void MainWindow::slot_close() {
    this->close();
}

void MainWindow::slot_minimize() {
    this->showMinimized();
}

void MainWindow::slot_closeServer() {
    server->exit();
    ui->ExitBtn->setEnabled(false);
    ui->ExitBtn->setVisible(false);
    ui->CreateBtn->setEnabled(true);
    ui->CreateBtn->setVisible(true);
}

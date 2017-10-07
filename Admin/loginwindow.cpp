/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：loginwindow.cpp
 * 摘要：登录界面
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QtSql>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setUpUI();

    //从数据库读取管理员账户信息
    QSqlQuery query;
    query.exec("SELECT * FROM 管理员账户");
    query.next();
    username=query.value(0).toString();
    password=query.value(1).toString();

    connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(login()));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

/*
 * 登录操作
 */
void LoginWindow::login()
{
    if(ibPassword->getText()==password && ibUsername->getText()==username) {
        target->show();
        this->close();
    }
    else
        QMessageBox::critical(NULL,"Error","账号或密码错误，请重新输入！",QMessageBox::Ok);
}

/*
 * 初始化登录界面
 */
void LoginWindow::setUpUI() {
    bgLabel=new BGLabel("",ui->centralwidget,this);
    this->resize(600,450);
    QImage *imageBG=new QImage;
    imageBG->load("img\\a_bg.png");
    bgLabel->setTitleBarHeight(100);
    bgLabel->setGeometry(0,0,600,450);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));

    QFont font("微软雅黑",10,50);
    ibUsername=new myInputBox(bgLabel);
    ibUsername->setGeometry(255,190,300,30);
    ibUsername->setLabelText("  用户名：");
    ibUsername->update();
    ibUsername->setFont(font);
    ibUsername->setStyleSheet("QPushButton {"
                              "    background-color: white; "
                              "    color: grey; "
                              "}"
                              "QPushButton:hover {"
                              "    color: black; "
                              "}");

    ibPassword=new myInputBox(bgLabel);
    ibPassword->setGeometry(255,240,300,30);
    ibPassword->setLabelText("  密码：");
    ibPassword->update();
    ibPassword->setFont(font);
    ibPassword->setPasswordMode(true);
    ibPassword->setStyleSheet("QPushButton {"
                              "    background-color: white; "
                              "    color: grey;"
                              "}"
                              "QPushButton:hover {"
                              "    color: black;"
                              "}");

    ui->btnLogin->setParent(bgLabel);
    ui->btnLogin->setGeometry(290,300,240,30);
    ui->btnLogin->setFont(font);
    ui->btnLogin->setObjectName("btnLogin");
    this->setStyleSheet("QPushButton#btnLogin {"
                        "    background-color: rgb(100,102,105);"
                        "    border: 2px solid rgb(100,102,105);"
                        "    color: rgb(255,255,255);"
                        "}"
                        "QPushButton#btnLogin:hover {"
                        "    background-color: rgb(120,122,125);"
                        "    border: 2px solid rgb(120,122,125);"
                        "    color: rgb(255,255,255);"
                        "}");

    QFont fontbtn("微软雅黑",16,75);
    ui->btnClose->setParent(bgLabel);
    ui->btnClose->setFlat(true);
    ui->btnClose->setGeometry(560,0,40,40);
    ui->btnClose->setFont(fontbtn);
    ui->btnClose->setText("×");

    ui->btnMin->setParent(bgLabel);
    ui->btnMin->setFlat(true);
    ui->btnMin->setGeometry(520,0,40,40);
    ui->btnMin->setFont(fontbtn);
    ui->btnMin->setText("-");

    connect(ui->btnClose,SIGNAL(clicked()),this,SLOT(slot_close()));
    connect(ui->btnMin,SIGNAL(clicked()),this,SLOT(slot_minimize()));
}

/*
 * 关闭按钮
 */
void LoginWindow::slot_close() {
    this->close();
}

/*
 * 最小化按钮
 */
void LoginWindow::slot_minimize() {
    this->showMinimized();
}

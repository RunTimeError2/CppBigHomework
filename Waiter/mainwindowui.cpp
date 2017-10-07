/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindowui.cpp
 * 摘要：服务员端主界面界面实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

/*
 * 切换到登录界面
 */
void MainWindow::slot_btnToLogin() {
    ui->frame_login->setVisible(true);
    ui->frame_Register->setVisible(false);

    ui->btnToLogin->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0.2);"
                                  "    border: 0px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.2);"
                                  "    border: 2px solid rgb(200,200,200);"
                                  "}");
    ui->btnToRegister->setStyleSheet("QPushButton {"
                                     "    background-color: rgba(255,255,255,0);"
                                     "    border: 0px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.1);"
                                     "    border: 2px solid rgb(200,200,200);"
                                     "}");
}

/*
 * 切换到注册界面
 */
void MainWindow::slot_btnToRegister() {
    ui->frame_login->setVisible(false);
    ui->frame_Register->setVisible(true);

    ui->btnToLogin->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0);"
                                  "    border: 0px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.1);"
                                  "    border: 2px solid rgb(200,200,200);"
                                  "}");
    ui->btnToRegister->setStyleSheet("QPushButton {"
                                     "    background-color: rgba(255,255,255,0.2);"
                                     "    border: 0px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.2);"
                                     "    border: 2px solid rgb(200,200,200);"
                                     "}");
}

/*
 * 初始化登录界面
 */
void MainWindow::InitLoginForm() {
    bgLabelLogin=new BGLabel("",ui->centralWidget,this);
    this->resize(500,650);
    QImage *imageBG=new QImage;
    imageBG->load("img\\w_bgLogin.png");
    bgLabelLogin->setTitleBarHeight(185);
    bgLabelLogin->setGeometry(0,0,500,650);
    bgLabelLogin->setPixmap(QPixmap::fromImage(*imageBG));
    connect(ui->btnToLogin,SIGNAL(clicked()),this,SLOT(slot_btnToLogin()));
    connect(ui->btnToRegister,SIGNAL(clicked()),this,SLOT(slot_btnToRegister()));

    ui->btnCloseWindow->setGeometry(470,0,30,30);
    ui->btnMinWindow->setGeometry(440,0,30,30);
    ui->btnCloseWindow->setFont(QFont("微软雅黑",14));
    ui->btnMinWindow->setFont(QFont("微软雅黑",14));
    ui->btnCloseWindow->setParent(bgLabelLogin);
    ui->btnMinWindow->setParent(bgLabelLogin);
    ui->btnCloseWindow->setStyleSheet("QPushButton {"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(255,0,0,0.4);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnMinWindow->setStyleSheet("QPushButton {"
                                    "    color: rgb(0,0,0);"
                                    "    background-color: rgba(0,0,0,0);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}"
                                    "QPushButton:hover {"
                                    "    color: rgb(0,0,0);"
                                    "    background-color: rgba(255,255,255,0.4);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}");

    ui->frameLogin->setVisible(true);
    ui->frameMain->setVisible(false);
    ui->frameLogin->setParent(bgLabelLogin);

    ui->frameLogin->setGeometry(60,80,380,490);
    ui->frame_login->setGeometry(40,250,300,295);
    ui->frame_Register->setGeometry(40,250,300,295);
    ui->btnToLogin->setGeometry(40,105,135,90);
    ui->btnToRegister->setGeometry(205,105,135,90);

    QFont font("微软雅黑",8);
    ui->serverIPLabel->setFont(font);
    ui->serverIPLineEdit->setFont(font);
    ui->PasswordLabel->setFont(font);
    ui->PasswordLineEdit->setFont(font);
    ui->pwLabel->setFont(font);
    ui->pwLineEdit->setFont(font);
    ui->UserNameLabel->setFont(font);
    ui->UserNameLineEdit->setFont(font);
    ui->unLineEdit->setFont(font);
    ui->usernameLabel->setFont(font);
    ui->ConfirmLabel->setFont(font);
    ui->ConfirmLineEdit->setFont(font);
    ui->btnToLogin->setFont(QFont("微软雅黑",10));
    ui->btnToRegister->setFont(QFont("微软雅黑",10));
    slot_btnToLogin();
}

/*
 * 初始化主界面
 */
void MainWindow::InitMainForm() {
    //bgLabelMain=new BGLabel("",ui->centralWidget,this);
    this->resize(1000,680);
    QImage *imageBG=new QImage;
    imageBG->load("img\\w_bgMain.png");
    bgLabelLogin->setPixmap(QPixmap::fromImage(*imageBG));
    bgLabelLogin->setTitleBarHeight(80);
    bgLabelLogin->setGeometry(0,0,1000,680);
    ui->frameMain->setParent(bgLabelLogin);
    //bgLabelMain->setTitleBarHeight(80);
    //bgLabelMain->setGeometry(0,0,1000,680);
    //bgLabelMain->setPixmap(QPixmap::fromImage(*imageBG));
    //ui->frameMain->setParent(bgLabelMain);
    ui->frameMain->setGeometry(0,0,1000,680);
    connect(ui->btnToDish,SIGNAL(clicked()),this,SLOT(slot_btnToDish()));
    connect(ui->btnToTable,SIGNAL(clicked()),this,SLOT(slot_btnToTable()));

    ui->btnCloseWindow->setParent(ui->frameMain);
    ui->btnMinWindow->setParent(ui->frameMain);
    ui->btnCloseWindow->setGeometry(970,0,30,30);
    ui->btnMinWindow->setGeometry(940,0,30,30);

    ui->frameLogin->setVisible(false);
    ui->frameMain->setVisible(true);
    ui->btnToDish->setGeometry(25,245,40,140);
    ui->btnToTable->setGeometry(25,105,40,140);

    ui->tableViewDishState->setGeometry(70,110,550,540);
    ui->tableViewSelect->setGeometry(70,110,550,540);
    slot_btnToTable();

    ui->listMsg->setGeometry(655,110,315,490);
    ui->lineEditMsg->setGeometry(745,605,230,20);
    ui->labelMsg->setGeometry(655,605,100,20);
    ui->labelMsg->setFont(QFont("微软雅黑",8));
    ui->lineEditMsg->setFont(QFont("微软雅黑",8));
    ui->btnSendMsg->setGeometry(870,630,100,20);
    ui->tableViewSelect->setShowGrid(false);
    ui->tableViewDishState->setShowGrid(false);
    ui->tableViewDishState->setStyleSheet("QTableView {"
                                          "    background-color: rgba(255,255,255,0.4);"
                                          "}");
    ui->tableViewSelect->setStyleSheet("QTableView {"
                                       "    background-color: rgba(255,255,255,0.4);"
                                       "}");
    ui->listMsg->setStyleSheet("QListWidget {"
                               "    background-color: rgba(255,255,255,0.4);"
                               "}");
}

/*
 * 切换到餐桌状态按钮
 */
void MainWindow::slot_btnToTable() {
    ui->tableViewDishState->setVisible(false);
    ui->tableViewSelect->setVisible(true);
    ui->btnToTable->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0.2);"
                                  "    border: 2px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.2);"
                                  "    border: 2px solid rgb(200,200,200);"
                                  "}");
    ui->btnToDish->setStyleSheet("QPushButton {"
                                 "    background-color: rgba(255,255,255,0);"
                                 "    border: 2px solid rgba(0,0,0,0);"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: rgba(255,255,255,0.1);"
                                 "    border: 2px solid rgb(200,200,200);"
                                 "}");
}

/*
 * 切换到菜品状态按钮
 */
void MainWindow::slot_btnToDish() {
    ui->tableViewDishState->setVisible(true);
    ui->tableViewSelect->setVisible(false);
    ui->btnToTable->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0);"
                                  "    border: 2px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.1);"
                                  "    border: 2px solid rgb(200,200,200);"
                                  "}");
    ui->btnToDish->setStyleSheet("QPushButton {"
                                 "    background-color: rgba(255,255,255,0.2);"
                                 "    border: 2px solid rgba(0,0,0,0);"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: rgba(255,255,255,0.2);"
                                 "    border: 2px solid rgb(200,200,200);"
                                 "}");
}

/*
 * 关闭按钮
 */
void MainWindow::slot_closeWindow() {
    this->close();
}

/*
 * 最小化按钮
 */
void MainWindow::slot_minWindow() {
    this->showMinimized();
}

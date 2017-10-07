/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindowui.cpp
 * 摘要：厨师端主界面界面实现
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
    ui->frame_register->setVisible(false);

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
    ui->frame_register->setVisible(true);

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
void MainWindow::setUpLoginForm() {
    bgLabel=new BGLabel("",ui->centralWidget,this);
    this->resize(500,650);
    QImage *imageBG=new QImage;
    imageBG->load("img\\c_bgLogin.png");
    bgLabel->setTitleBarHeight(185);
    bgLabel->setGeometry(0,0,500,650);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));

    connect(ui->btnToLogin,SIGNAL(clicked()),this,SLOT(slot_btnToLogin()));
    connect(ui->btnToRegister,SIGNAL(clicked()),this,SLOT(slot_btnToRegister()));

    ui->btnCloseWindow->setGeometry(470,0,30,30);
    ui->btnMinWindow->setGeometry(440,0,30,30);
    ui->btnCloseWindow->setFont(QFont("微软雅黑",14));
    ui->btnMinWindow->setFont(QFont("微软雅黑",14));
    ui->btnCloseWindow->setParent(bgLabel);
    ui->btnMinWindow->setParent(bgLabel);
    ui->btnCloseWindow->setStyleSheet("QPushButton {"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(255,0,0,0.5);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnMinWindow->setStyleSheet("QPushButton {"
                                    "    color: rgb(0,0,0);"
                                    "    background-color: rgba(0,0,0,0);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}"
                                    "QPushButton:hover {"
                                    "    color: rgb(0,0,0);"
                                    "    background-color: rgba(255,255,255,0.5);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}");

    ui->frameLogin->setVisible(true);
    ui->frameMain->setVisible(false);
    ui->frameLogin->setParent(bgLabel);

    ui->frameLogin->setGeometry(60,80,380,490);
    ui->frame_login->setGeometry(40,250,300,295);
    ui->frame_register->setGeometry(40,250,300,295);
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
void MainWindow::setUpMainForm() {
    this->resize(1000,680);
    QImage *imageBG=new QImage;
    imageBG->load("img\\c_bgMain.png");
    bgLabel->setTitleBarHeight(80);
    bgLabel->setGeometry(0,0,1000,680);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));
    ui->frameLogin->setVisible(false);
    ui->frameMain->setVisible(true);
    ui->frameMain->setParent(bgLabel);
    ui->frameMain->setGeometry(0,80,1000,600);

    ui->btnCloseWindow->setGeometry(970,0,30,30);
    ui->btnMinWindow->setGeometry(940,0,30,30);
    ui->tvSelectDish->setGeometry(25,55,450,520);
    ui->tvSelected->setGeometry(515,55,450,520);
    ui->btnRefreshValiddish->setGeometry(200,20,110,30);
    ui->btnRefreshSelectedDish->setGeometry(665,20,110,30);
    ui->btnRefreshValiddish->setText("");
    ui->btnRefreshSelectedDish->setText("");
    ui->btnRefreshValiddish->setStyleSheet("QPushButton {"
                                           "    background-color: rgba(0,0,0,0);"
                                           "    border: 2px solid rgba(0,0,0,0);"
                                           "}"
                                           "QPushButton:hover {"
                                           "    background-color: rgba(255,255,255,0.2);"
                                           "    border: 2px solid rgba(255,255,255,0.5);"
                                           "}");
    ui->btnRefreshSelectedDish->setStyleSheet("QPushButton {"
                                              "    background-color: rgba(0,0,0,0);"
                                              "    border: 2px solid rgba(0,0,0,0);"
                                              "}"
                                              "QPushButton:hover {"
                                              "    background-color: rgba(255,255,255,0.2);"
                                              "    border: 2px solid rgba(255,255,255,0.5);"
                                              "}");
    QString tablestyle="QTableView { background-color: rgba(255,255,255,0.4); }";
    ui->tvSelectDish->setStyleSheet(tablestyle);
    ui->tvSelected->setStyleSheet(tablestyle);
    ui->tvSelectDish->verticalHeader()->setVisible(false);
    ui->tvSelected->verticalHeader()->setVisible(false);
    ui->tvSelectDish->setShowGrid(false);
    ui->tvSelected->setShowGrid(false);
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

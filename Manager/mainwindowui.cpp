/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindowui.cpp
 * 摘要：经理端主界面界面实现
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
void MainWindow::setupLoginForm() {
    bgLabel=new BGLabel("",ui->centralWidget,this);
    this->resize(500,650);
    QImage *imageBG=new QImage;
    imageBG->load("img\\m_bgLogin.png");
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
                                      "    color: rgb(255,255,255);"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    color: rgb(255,255,255);"
                                      "    background-color: rgba(255,0,0,0.5);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnMinWindow->setStyleSheet("QPushButton {"
                                    "    color: rgb(255,255,255);"
                                    "    background-color: rgba(0,0,0,0);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}"
                                    "QPushButton:hover {"
                                    "    color: rgb(255,255,255);"
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
void MainWindow::setupMainForm() {
    this->resize(600,800);
    QImage *imageBG=new QImage;
    imageBG->load("img\\m_bgMain.png");
    bgLabel->setTitleBarHeight(60);
    bgLabel->setGeometry(0,0,600,800);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));

    ui->btnCloseWindow->setGeometry(570,0,30,30);
    ui->btnMinWindow->setGeometry(540,0,30,30);

    connect(ui->btnToWaiter,SIGNAL(clicked()),this,SLOT(slot_btnToWaiter()));
    connect(ui->btnToCook,SIGNAL(clicked()),this,SLOT(slot_btnToCook()));
    connect(ui->btnToOther,SIGNAL(clicked()),this,SLOT(slot_btnToOther()));

    ui->frameMain->setParent(bgLabel);
    ui->frameLogin->setVisible(false);
    ui->frameMain->setVisible(true);
    ui->frameMain->setGeometry(20,80,560,700);
    ui->frame_cook->setGeometry(10,40,540,650);
    ui->frame_waiter->setGeometry(10,40,540,650);
    ui->frame_other->setGeometry(10,40,540,650);
    ui->tableViewCook->setGeometry(0,0,540,650);
    ui->tableViewWaiter->setGeometry(0,0,540,400);
    ui->tableViewWaiterEvaluation->setGeometry(0,410,540,240);
    ui->tableViewRecord->setGeometry(0,0,540,300);
    ui->tableViewDishEvaluation->setGeometry(0,330,540,300);
    ui->labelRecord->setGeometry(0,300,540,20);
    ui->labelEvaluation->setGeometry(0,630,540,20);
    ui->labelRecord->setStyleSheet("QLabel { background-color: rgba(255,255,255,0.4); color: rgb(0,0,0); }");
    ui->labelEvaluation->setStyleSheet("QLabel { background-color: rgba(255,255,255,0.4); color: rgb(0,0,0); }");
    ui->labelRecord->setFont(QFont("微软雅黑",9));
    ui->labelEvaluation->setFont(QFont("微软雅黑",9));
    ui->btnToWaiter->setGeometry(0,0,170,30);
    ui->btnToCook->setGeometry(170,0,130,30);
    ui->btnToOther->setGeometry(300,0,160,30);
    ui->btnRefreshReport->setGeometry(460,0,100,30);
    QString style="QPushButton {"
                  "    background-color: rgba(0,0,0,0);"
                  "    border: 2px solid rgba(0,0,0,0);"
                  "}"
                  "QPushButton:hover {"
                  "    background-color: rgba(255,255,255,0.2);"
                  "    border: 2px solid rgba(255,255,255,0.4);"
                  "}";
    ui->btnRefreshReport->setStyleSheet(style);
    ui->btnToCook->setStyleSheet(style);
    ui->btnToWaiter->setStyleSheet(style);
    ui->btnToOther->setStyleSheet(style);
    slot_btnToWaiter();
    QString tablestyle="QTableView { background-color: rgba(255,255,255,0.4); }";
    ui->tableViewCook->setStyleSheet(tablestyle);
    ui->tableViewWaiter->setStyleSheet(tablestyle);
    ui->tableViewWaiterEvaluation->setStyleSheet(tablestyle);
    ui->tableViewDishEvaluation->setStyleSheet(tablestyle);
    ui->tableViewRecord->setStyleSheet(tablestyle);
    ui->tableViewCook->verticalHeader()->setVisible(false);
    ui->tableViewWaiter->verticalHeader()->setVisible(false);
    ui->tableViewWaiterEvaluation->verticalHeader()->setVisible(false);
    ui->tableViewDishEvaluation->verticalHeader()->setVisible(false);
    ui->tableViewRecord->verticalHeader()->setVisible(false);
    ui->tableViewCook->setShowGrid(false);
    ui->tableViewWaiter->setShowGrid(false);
    ui->tableViewWaiterEvaluation->setShowGrid(false);
    ui->tableViewDishEvaluation->setShowGrid(false);
    ui->tableViewRecord->setShowGrid(false);
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

/*
 * 切换到服务员工作记录、评价部分
 */
void MainWindow::slot_btnToWaiter() {
    ui->frame_waiter->setVisible(true);
    ui->frame_cook->setVisible(false);
    ui->frame_other->setVisible(false);
    ui->btnToWaiter->setStyleSheet("QPushButton {"
                                   "    background-color: rgba(255,255,255,0.2);"
                                   "    border: 2px solid rgba(255,255,255,0);"
                                   "}"
                                   "QPusbButton:hover {"
                                   "    background-color: rgba(255,255,255,0.2);"
                                   "    border: 2px solid rgba(255,255,255,0.4);"
                                   "}");
    ui->btnToCook->setStyleSheet("QPushButton {"
                                 "    background-color: rgba(255,255,255,0);"
                                 "    border: 2px solid rgba(0,0,0,0);"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: rgba(255,255,255,0.1);"
                                 "    border: 2px solid rgba(255,255,255,0.4);"
                                 "}");
    ui->btnToOther->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0);"
                                  "    border: 2px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.1);"
                                  "    border: 2px solid rgba(255,255,255,0.4);"
                                  "}");
}

/*
 * 切换到厨师工作记录部分
 */
void MainWindow::slot_btnToCook() {
    ui->frame_waiter->setVisible(false);
    ui->frame_cook->setVisible(true);
    ui->frame_other->setVisible(false);
    ui->btnToWaiter->setStyleSheet("QPushButton {"
                                   "    background-color: rgba(255,255,255,0);"
                                   "    border: 2px solid rgba(0,0,0,0);"
                                   "}"
                                   "QPushButton:hover {"
                                   "    background-color: rgba(255,255,255,0.1);"
                                   "    border: 2px solid rgba(255,255,255,0.4);"
                                   "}");
    ui->btnToCook->setStyleSheet("QPushButton {"
                                 "    background-color: rgba(255,255,255,0.2);"
                                 "    border: 2px solid rgba(255,255,255,0);"
                                 "}"
                                 "QPusbButton:hover {"
                                 "    background-color: rgba(255,255,255,0.2);"
                                 "    border: 2px solid rgba(255,255,255,0.4);"
                                 "}");
    ui->btnToOther->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0);"
                                  "    border: 2px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.1);"
                                  "    border: 2px solid rgba(255,255,255,0.4);"
                                  "}");
}

/*
 * 切换到交易记录及菜品评价部分
 */
void MainWindow::slot_btnToOther() {
    ui->frame_waiter->setVisible(false);
    ui->frame_cook->setVisible(false);
    ui->frame_other->setVisible(true);
    ui->btnToWaiter->setStyleSheet("QPushButton {"
                                   "    background-color: rgba(255,255,255,0);"
                                   "    border: 2px solid rgba(0,0,0,0);"
                                   "}"
                                   "QPushButton:hover {"
                                   "    background-color: rgba(255,255,255,0.1);"
                                   "    border: 2px solid rgba(255,255,255,0.4);"
                                   "}");
    ui->btnToCook->setStyleSheet("QPushButton {"
                                 "    background-color: rgba(255,255,255,0);"
                                 "    border: 2px solid rgba(0,0,0,0);"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: rgba(255,255,255,0.1);"
                                 "    border: 2px solid rgba(255,255,255,0.4);"
                                 "}");
    ui->btnToOther->setStyleSheet("QPushButton {"
                                  "    background-color: rgba(255,255,255,0.2);"
                                  "    border: 2px solid rgba(255,255,255,0);"
                                  "}"
                                  "QPusbButton:hover {"
                                  "    background-color: rgba(255,255,255,0.2);"
                                  "    border: 2px solid rgba(255,255,255,0.4);"
                                  "}");
}

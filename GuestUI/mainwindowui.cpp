/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindowui.cpp
 * 摘要：顾客端主界面界面实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myinputbox.h"
#include <QPushButton>
#include <QMessageBox>

/*
 * 显示登录界面
 */
void MainWindow::setLoginForm() {
    this->resize(600,450);
    ui->frameLogin->setVisible(true);
    ui->frameMain->setVisible(false);
    ui->frameLogin->setGeometry(0,0,600,450);

    //显示背景
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_loginBG.png");
    bgLabel=new BGLabel("",ui->frameLogin,this);
    bgLabel->setTitleBarHeight(450);
    bgLabel->setGeometry(0,0,600,450);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));

    //设置登录、注册的文本框
    ui->frameSubLogin->setGeometry(150,220,300,400);
    ui->frameSubRegister->setGeometry(150,220,300,400);
    ui->frameSubLogin->setParent(bgLabel);
    ui->frameSubRegister->setParent(bgLabel);

    ibLoginPhonenumber=new myInputBox(ui->frameSubLogin);
    ibLoginPhonenumber->setGeometry(0,0,300,30);
    ibLoginPhonenumber->setLabelText("  手机号：");
    ibLoginPhonenumber->update();
    ibLoginPhonenumber->setStyleSheet("QPushButton {"
                                      "    background-color: white; "
                                      "    color: grey; }"
                                      "QPushButton:hover {"
                                      "    color: black; }");

    ibLoginPassword=new myInputBox(ui->frameSubLogin);
    ibLoginPassword->setGeometry(0,30,300,30);
    ibLoginPassword->setLabelText("  密码：");
    ibLoginPassword->update();
    ibLoginPassword->setPasswordMode(true);
    ibLoginPassword->setStyleSheet("QPushButton {"
                                   "    background-color: white; "
                                   "    color: grey; }"
                                   "QPushButton:hover {"
                                   "    color: black; }");

    ibServerIP=new myInputBox(ui->frameSubLogin);
    ibServerIP->setGeometry(0,60,300,30);
    ibServerIP->setLabelText("  服务器IP：");
    ibServerIP->update();
    ibServerIP->setStyleSheet("QPushButton {"
                              "    background-color: white; "
                              "    color: grey; }"
                              "QPushButton:hover {"
                              "    color: black; }");

    ibRegisterPhonenumber=new myInputBox(ui->frameSubRegister);
    ibRegisterPhonenumber->setGeometry(0,0,300,30);
    ibRegisterPhonenumber->setLabelText("  手机号：");
    ibRegisterPhonenumber->update();
    ibRegisterPhonenumber->setStyleSheet("QPushButton {"
                                   "    background-color: white; "
                                   "    color: grey; }"
                                   "QPushButton:hover {"
                                   "    color: black; }");

    ibRegisterUsername=new myInputBox(ui->frameSubRegister);
    ibRegisterUsername->setGeometry(0,30,300,30);
    ibRegisterUsername->setLabelText("  用户名：");
    ibRegisterUsername->update();
    ibRegisterUsername->setStyleSheet("QPushButton {"
                                   "    background-color: white; "
                                   "    color: grey; }"
                                   "QPushButton:hover {"
                                   "    color: black; }");

    ibRegisterPassword=new myInputBox(ui->frameSubRegister);
    ibRegisterPassword->setGeometry(0,60,300,30);
    ibRegisterPassword->setLabelText("  密码：");
    ibRegisterPassword->update();
    ibRegisterPassword->setPasswordMode(true);
    ibRegisterPassword->setStyleSheet("QPushButton {"
                                   "    background-color: white; "
                                   "    color: grey; }"
                                   "QPushButton:hover {"
                                   "    color: black; }");

    ibConfirmPassword=new myInputBox(ui->frameSubRegister);
    ibConfirmPassword->setGeometry(0,90,300,30);
    ibConfirmPassword->setLabelText("  确认密码：");
    ibConfirmPassword->update();
    ibConfirmPassword->setPasswordMode(true);
    ibConfirmPassword->setStyleSheet("QPushButton {"
                                   "    background-color: white; "
                                   "    color: grey; }"
                                   "QPushButton:hover {"
                                   "    color: black; }");

    QFont font("微软雅黑",10,50);
    ibLoginPassword->setFont(font);
    ibLoginPhonenumber->setFont(font);
    ibRegisterPhonenumber->setFont(font);
    ibRegisterUsername->setFont(font);
    ibRegisterPassword->setFont(font);
    ibServerIP->setFont(font);
    ibConfirmPassword->setFont(font);
    ui->btnLogin->setFont(font);
    ui->btnRegister->setFont(font);
    ui->btnToLogin->setFont(font);
    ui->btnToRegister->setFont(font);
    ui->btnLogin->setParent(ui->frameSubLogin);
    ui->btnToRegister->setParent(ui->frameSubLogin);
    ui->btnRegister->setParent(ui->frameSubRegister);
    ui->btnToLogin->setParent(ui->frameSubRegister);
    ui->btnLogin->setGeometry(0,160,150,30);
    ui->btnRegister->setGeometry(0,160,150,30);
    ui->btnToRegister->setGeometry(150,160,150,30);
    ui->btnToLogin->setGeometry(150,160,150,30);


    ui->frameSubLogin->setVisible(true);
    ui->frameSubRegister->setVisible(false);

    connect(ui->btnToRegister,SIGNAL(clicked()),this,SLOT(slot_toRegister()));
    connect(ui->btnToLogin,SIGNAL(clicked()),this,SLOT(slot_toLogin()));

    QFont btnfont("微软雅黑",16,75);
    ui->btnCloseWindow->setParent(bgLabel);
    ui->btnMinWindow->setParent(bgLabel);
    ui->btnCloseWindow->setFont(btnfont);
    ui->btnMinWindow->setFont(btnfont);
    ui->btnCloseWindow->setGeometry(560,0,40,40);
    ui->btnMinWindow->setGeometry(520,0,40,40);
    ui->btnCloseWindow->setObjectName("closeButton");
    ui->btnMinWindow->setObjectName("minimumButton");

    connect(ui->btnCloseWindow,SIGNAL(clicked()),this,SLOT(slot_closeWindow()));
    connect(ui->btnMinWindow,SIGNAL(clicked()),this,SLOT(slot_miniWindow()));
    ui->btnCloseWindow->setStyleSheet("QPushButton {"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,0,0,0.7);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnMinWindow->setStyleSheet("QPushButton {"
                                    "    background-color: rgba(0,0,0,0);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: rgba(255,255,255,0.6);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}");
    bgLabel->show();
}

void MainWindow::slot_closeWindow() {
    this->close();
}

void MainWindow::slot_miniWindow() {
    this->showMinimized();
}

/*
 * 显示登录界面
 */
void MainWindow::slot_toLogin() {
    showLogin();
}

void MainWindow::showLogin() {
    ibServerIP->setGeometry(0,60,300,30);
    ibServerIP->setParent(ui->frameSubLogin);
    ui->frameSubLogin->setVisible(true);
    ui->frameSubRegister->setVisible(false);
}

/*
 * 显示注册界面
 */
void MainWindow::slot_toRegister() {
    showRegister();
}

void MainWindow::showRegister() {
    ibServerIP->setGeometry(0,120,300,30);
    ibServerIP->setParent(ui->frameSubRegister);
    ui->frameSubLogin->setVisible(false);
    ui->frameSubRegister->setVisible(true);
}

/*
 * 显示主界面
 */
void MainWindow::setMainForm() {
    this->resize(1000,700);
    ui->frameLogin->setVisible(false);
    ui->frameMain->setVisible(true);
    ui->frameMain->setGeometry(0,0,1000,700);

    //显示背景
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain0.png");
    bgLabel2=new BGLabel("",ui->frameMain,this);
    bgLabel2->setTitleBarHeight(80);
    bgLabel2->setGeometry(0,0,1000,700);
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    QFont btnfont("微软雅黑",16,75);
    ui->btnCloseWindow->setParent(bgLabel2);
    ui->btnMinWindow->setParent(bgLabel2);
    ui->btnCloseWindow->setFont(btnfont);
    ui->btnMinWindow->setFont(btnfont);
    ui->btnCloseWindow->setGeometry(980,0,20,20);
    ui->btnMinWindow->setGeometry(960,0,20,20);
    ui->btnCloseWindow->setText("");
    ui->btnMinWindow->setText("");
    ui->btnCloseWindow->setStyleSheet("QPushButton {"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,0,0,0.7);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnMinWindow->setStyleSheet("QPushButton {"
                                    "    background-color: rgba(0,0,0,0);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: rgba(255,255,255,0.6);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}");

    connect(ui->btnCloseWindow,SIGNAL(clicked()),this,SLOT(slot_closeWindow()));
    connect(ui->btnMinWindow,SIGNAL(clicked()),this,SLOT(slot_miniWindow()));

    ui->frameTabButtons->setParent(bgLabel2);
    ui->frameTabButtons->setGeometry(0,250,200,500);
    ui->btnTab1->setGeometry(0,50,200,50); //点菜
    ui->btnTab1->setText("");
    ui->btnTab2->setGeometry(0,0,200,50); //选桌
    ui->btnTab2->setText("");
    ui->btnTab3->setGeometry(0,100,200,50); //下单与结账
    ui->btnTab3->setText("");
    ui->btnTab4->setGeometry(0,250,200,50); //评价
    ui->btnTab4->setText("");
    ui->btnTab5->setGeometry(0,200,200,50); //账号与服务员信息
    ui->btnTab5->setText("");
    ui->btnTab6->setGeometry(0,150,200,50); //菜品状态
    ui->btnTab6->setText("");
    ui->frameTabButtons->setStyleSheet("QPushButton {"
                                       "    background: rgba(0,0,0,0);"
                                       "}"
                                       "QPushButton:hover {"
                                       "    background: rgba(255,255,255,0.2);"
                                       "}");
    tabsLayout();
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(false);

    connect(ui->btnTab1,SIGNAL(clicked()),this,SLOT(slot_tab1()));
    connect(ui->btnTab2,SIGNAL(clicked()),this,SLOT(slot_tab2()));
    connect(ui->btnTab3,SIGNAL(clicked()),this,SLOT(slot_tab3()));
    connect(ui->btnTab4,SIGNAL(clicked()),this,SLOT(slot_tab4()));
    connect(ui->btnTab5,SIGNAL(clicked()),this,SLOT(slot_tab5()));
    connect(ui->btnTab6,SIGNAL(clicked()),this,SLOT(slot_tab6()));
    bgLabel2->show();

    QFont fonttable("微软雅黑",10,50);
    ui->tableViewEvaluate->setFont(fonttable);
    ui->tableViewMenu->setFont(fonttable);
    ui->tableViewOrder->setFont(fonttable);
    ui->tableViewTable->setFont(fonttable);
}

/*
 * 实现左侧6个按钮的切换
 */
void MainWindow::switchTab(int index,QPushButton *btn) {
    if(index<0 || index>6)
        return;
    ui->btnTab1->setStyleSheet("QPushButton {"
                               "    background-color: rgba(0,0,0,0);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "    color: rgb(250,250,250);"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(255,255,255,0.2);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "}");
    ui->btnTab2->setStyleSheet("QPushButton {"
                               "    background-color: rgba(0,0,0,0);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "    color: rgb(250,250,250);"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(255,255,255,0.2);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "}");
    ui->btnTab3->setStyleSheet("QPushButton {"
                               "    background-color: rgba(0,0,0,0);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "    color: rgb(250,250,250);"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(255,255,255,0.2);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "}");
    ui->btnTab4->setStyleSheet("QPushButton {"
                               "    background-color: rgba(0,0,0,0);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "    color: rgb(250,250,250);"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(255,255,255,0.2);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "}");
    ui->btnTab5->setStyleSheet("QPushButton {"
                               "    background-color: rgba(0,0,0,0);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "    color: rgb(250,250,250);"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(255,255,255,0.2);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "}");
    ui->btnTab6->setStyleSheet("QPushButton {"
                               "    background-color: rgba(0,0,0,0);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "    color: rgb(250,250,250);"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(255,255,255,0.2);"
                               "    border: 2px solid rgba(0,0,0,0);"
                               "}");
    if(index>0 && btn)
        btn->setStyleSheet("QPushButton {"
                           "    background-color: rgba(255,255,255,0.15);"
                           "    border: 1px solid rgba(0,0,0,0);"
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(255,255,255,0.2);"
                           "    border: 1px solid rgba(0,0,0,0);"
                           "}");
}

/*
 * 点菜界面
 */
void MainWindow::slot_tab1() {
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain2.png");
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    switchTab(1,ui->btnTab1);
    ui->frameTab1->setVisible(true);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(false);

    ui->frameTab1->setGeometry(200,80,800,620);
    ui->tableViewMenu->setGeometry(25,55,730,515);
    ui->btnOrderDone->setGeometry(20,20,150,35);
    ui->btnOrderDone->setText("");
    ui->btnOrderDone->setStyleSheet("QPushButton {"
                                    "    background-color: rgba(0,0,0,0);"
                                    "    border: 1px solid rgba(0,0,0,0);"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: rgba(255,255,255,0.4);"
                                    "    border: 2px solid rgba(128,128,128,0.6);"
                                    "}");
    ui->btnClearOrder->setGeometry(210,20,150,35);
    ui->btnClearOrder->setText("");
    ui->btnClearOrder->setStyleSheet("QPushButton {"
                                     "    background-color: rgba(0,0,0,0);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.4);"
                                     "    border: 2px solid rgba(128,128,128,0.6);"
                                     "}");
    ui->tableViewMenu->setStyleSheet("QTableView {"
                                     "    color: rgb(0,0,0);  "
                                     "    border: 1px solid rgb(128,128,128);"
                                     "    background-color: rgba(255,255,255,0.5);"
                                     "}");
    ui->labelDishCount->setGeometry(50,570,700,30);
    ui->labelDishCount->setFont(QFont("微软雅黑",10));
}

/*
 * 选桌界面
 */
void MainWindow::slot_tab2() {
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain1.png");
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    switchTab(2,ui->btnTab2);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(true);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(false);

    ui->frameTab2->setGeometry(200,80,800,620);
    ui->tableViewTable->setGeometry(25,55,730,545);
    ui->btnSelectTable->setGeometry(20,20,150,35);
    ui->btnSelectTable->setText("");
    ui->btnSelectTable->setStyleSheet("QPushButton {"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,255,255,0.4);"
                                      "    border: 2px solid rgba(128,128,128,0.6);"
                                      "}");

    ui->tableViewTable->setStyleSheet("QTableView {"
                                      "    color: rgb(0,0,0);  "
                                      "    border: 1px solid rgb(128,128,128);"
                                      "    background-color: rgba(255,255,255,0.5);"
                                      "}");
}

/*
 * 食物托盘界面
 */
void MainWindow::slot_tab3() {
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain3.png");
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    switchTab(3,ui->btnTab3);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(true);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(false);

    ui->frameTab3->setGeometry(200,80,800,620);
    ui->tableViewOrder->setGeometry(25,55,730,515);
    ui->btnConfirmOrder->setGeometry(20,20,150,35);
    ui->btnConfirmOrder->setText("");
    ui->btnConfirmOrder->setStyleSheet("QPushButton {"
                                       "    background-color: rgba(0,0,0,0);"
                                       "    border: 1px solid rgba(0,0,0,0);"
                                       "}"
                                       "QPushButton:hover {"
                                       "    background-color: rgba(255,255,255,0.4);"
                                       "    border: 2px solid rgba(128,128,128,0.6);"
                                       "}");
    ui->btnContinue->setGeometry(210,20,150,35);
    ui->btnContinue->setText("");
    ui->btnContinue->setStyleSheet("QPushButton {"
                                   "    background-color: rgba(0,0,0,0);"
                                   "    border: 1px solid rgba(0,0,0,0);"
                                   "}"
                                   "QPushButton:hover {"
                                   "    background-color: rgba(255,255,255,0.4);"
                                   "    border: 2px solid rgba(128,128,128,0.6);"
                                   "}");

    ui->tableViewOrder->setStyleSheet("QTableView {"
                                      "    color: rgb(0,0,0);  "
                                      "    border: 1px solid rgb(128,128,128);"
                                      "    background-color: rgba(255,255,255,0.5);"
                                      "}");
    ui->labelOrderCount->setGeometry(50,570,700,30);
    ui->labelOrderCount->setFont(QFont("微软雅黑",10));
}

/*
 * 评价界面
 */
void MainWindow::slot_tab4() {
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain6.png");
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    switchTab(4,ui->btnTab4);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(true);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(false);

    ui->frameTab4->setGeometry(200,80,800,620);
    ui->tableViewEvaluate->setGeometry(25,55,755,445);
    ui->btnConfirmEvaluate->setGeometry(20,20,150,35);
    ui->btnConfirmEvaluate->setText("");
    ui->btnConfirmEvaluate->setStyleSheet("QPushButton {"
                                          "    background-color: rgba(0,0,0,0);"
                                          "    border: 1px solid rgba(0,0,0,0);"
                                          "}"
                                          "QPushButton:hover {"
                                          "    background-color: rgba(255,255,255,0.4);"
                                          "    border: 2px solid rgba(128,128,128,0.6);"
                                          "}");
    ui->lineEditEvaluateWaiter->setGeometry(30,550,745,20);
    ui->spinBoxScoreWaiter->setGeometry(80,575,200,20);
    ui->labelWaiterScore->setGeometry(30,575,100,20);
    ui->labelWaiterScore->setFont(QFont("微软雅黑",8));
    ui->btnEvaluateWaiter->setGeometry(625,575,150,20);
}

/*
 * 账号与服务员信息
 */
void MainWindow::slot_tab5() {
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain5.png");
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    switchTab(5,ui->btnTab5);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(true);
    ui->frameTab6->setVisible(false);

    ui->frameTab5->setGeometry(200,80,800,620);
    ui->labelChangeNew->setGeometry(60,70,300,30);
    ui->lineEditChangeNew->setGeometry(130,75,200,20);
    ui->labelChangeOldpw->setGeometry(60,110,300,30);
    ui->lineEditChangeOldpw->setGeometry(130,115,200,20);
    ui->labelChangePw->setGeometry(380,70,300,30);
    ui->lineEditChangeNewpw->setGeometry(450,75,200,20);
    ui->labelChangeConfirm->setGeometry(380,110,300,30);
    ui->lineEditChangeConfirm->setGeometry(450,115,200,20);
    ui->labelChangeNew->setFont(QFont("微软雅黑",9));
    ui->labelChangeOldpw->setFont(QFont("微软雅黑",9));
    ui->labelChangeConfirm->setFont(QFont("微软雅黑",9));
    ui->labelChangePw->setFont(QFont("微软雅黑",9));
    ui->lineEditChangeConfirm->setFont(QFont("微软雅黑",8));
    ui->lineEditChangeNew->setFont(QFont("微软雅黑",8));
    ui->lineEditChangeNewpw->setFont(QFont("微软雅黑",8));
    ui->lineEditChangeOldpw->setFont(QFont("微软雅黑",8));
    ui->btnChangeAccount->setGeometry(280,180,150,30);

    ui->labelWaiterInfo->setGeometry(200,243,200,30);
    ui->labelWaiterInfo->setFont(QFont("微软雅黑",9));
    ui->listMsg->setGeometry(30,280,745,290);
    ui->lineEditMsg->setGeometry(30,575,650,20);
    ui->btnMsg->setGeometry(685,575,90,20);
}

/*
 * 菜品状态
 */
void MainWindow::slot_tab6() {
    QImage *imageBG=new QImage;
    imageBG->load("img\\g_BgMain4.png");
    bgLabel2->setPixmap(QPixmap::fromImage(*imageBG));

    switchTab(6,ui->btnTab6);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(true);

    ui->frameTab6->setGeometry(200,80,800,620);
    ui->tableViewState->setGeometry(25,55,730,545);
    ui->btnPayConfirm->setGeometry(20,20,150,35);
    ui->btnPayConfirm->setText("");
    ui->btnPayConfirm->setStyleSheet("QPushButton {"
                                     "    background-color: rgba(0,0,0,0);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.4);"
                                     "    border: 2px solid rgba(128,128,128,0.6);"
                                     "}");
    ui->tableViewState->setStyleSheet("QTableView {"
                                      "    color: rgb(0,0,0);  "
                                      "    border: 1px solid rgb(128,128,128);"
                                      "    background-color: rgba(255,255,255,0.5);"
                                      "}");
    ui->labelFinalPrice->setGeometry(190,20,150,30);
    ui->labelFinalPrice->setFont(QFont("微软雅黑",10));
}

/*
 * 完成各标签的布局
 */
void MainWindow::tabsLayout() {
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    ui->frameTab6->setVisible(false);
    ui->frameTab1->setParent(bgLabel2);
    ui->frameTab2->setParent(bgLabel2);
    ui->frameTab3->setParent(bgLabel2);
    ui->frameTab4->setParent(bgLabel2);
    ui->frameTab5->setParent(bgLabel2);
    ui->frameTab6->setParent(bgLabel2);
}

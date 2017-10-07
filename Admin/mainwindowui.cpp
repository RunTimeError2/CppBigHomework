/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindowui.cpp
 * 摘要：主界面界面实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "combodelegate.h"
#include "spindelegate.h"
#include <QMessageBox>

/*
 * 初始化主界面
 */
void MainWindow::setUpUI() {
    //初始化自定义无边框窗体
    bgLabel=new BGLabel("",ui->centralWidget,this);
    this->resize(1000,700);
    QImage *imageBG=new QImage;
    imageBG->load("img\\a_bgMain.png");
    bgLabel->setTitleBarHeight(45);
    bgLabel->setGeometry(0,0,1000,700);
    bgLabel->setPixmap(QPixmap::fromImage(*imageBG));

    //关闭和最小化按钮
    QFont fontbtn("微软雅黑",16,75);
    ui->btnClose->setParent(bgLabel);
    ui->btnClose->setStyleSheet("QPushButton {"
                                "    background-color: rgba(255,255,255,0);"
                                "    border: 1px solid rgba(0,0,0,0);"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: rgba(255,0,0,0.5);"
                                "    border: 1px solid rgba(0,0,0,0);"
                                "}");
    ui->btnClose->setText("");
    ui->btnClose->setGeometry(972,0,28,28);
    ui->btnClose->setFont(fontbtn);

    ui->btnMin->setParent(bgLabel);
    ui->btnMin->setStyleSheet("QPushButton {"
                              "    background-color: rgba(0,0,0,0);"
                              "    border: 1px solid rgba(0,0,0,0);"
                              "}"
                              "QPushButton:hover {"
                              "    background-color: rgba(255,255,255,0.5);"
                              "    border: 1px solid rgba(0,0,0,0);"
                              "}");
    ui->btnMin->setText("");
    ui->btnMin->setGeometry(944,0,28,28);
    ui->btnMin->setFont(fontbtn);

    connect(ui->btnClose,SIGNAL(clicked()),this,SLOT(slot_close()));
    connect(ui->btnMin,SIGNAL(clicked()),this,SLOT(slot_min()));

    //左侧切换界面按钮
    btnTab1=new QPushButton(bgLabel);
    btnTab1->setText(""); //编辑菜单
    btnTab2=new QPushButton(bgLabel);
    btnTab2->setText(""); //编辑餐桌信息
    btnTab3=new QPushButton(bgLabel);
    btnTab3->setText(""); //管理顾客账号
    btnTab4=new QPushButton(bgLabel);
    btnTab4->setText(""); //管理其他账号
    btnTab5=new QPushButton(bgLabel);
    btnTab5->setText(""); //更改管理员账号
    ui->frameTab1->setParent(bgLabel);
    ui->frameTab2->setParent(bgLabel);
    ui->frameTab3->setParent(bgLabel);
    ui->frameTab4->setParent(bgLabel);
    ui->frameTab5->setParent(bgLabel);
    connect(btnTab1,SIGNAL(clicked()),this,SLOT(slot_tab1()));
    connect(btnTab2,SIGNAL(clicked()),this,SLOT(slot_tab2()));
    connect(btnTab3,SIGNAL(clicked()),this,SLOT(slot_tab3()));
    connect(btnTab4,SIGNAL(clicked()),this,SLOT(slot_tab4()));
    connect(btnTab5,SIGNAL(clicked()),this,SLOT(slot_tab5()));
    //ui->frameTabButtons->setGeometry(0,0,1000,700);
    btnTab1->setGeometry(0,200,180,40);
    btnTab2->setGeometry(0,240,180,40);
    btnTab3->setGeometry(0,280,180,40);
    btnTab4->setGeometry(0,320,180,40);
    btnTab5->setGeometry(680,22,140,23);

    switchTab(btnTab1);

    ui->lineEditChangeName->setFont(QFont("微软雅黑",9));
    ui->lineEditChangepw->setFont(QFont("微软雅黑",9));
    ui->lineEditConfirmpw->setFont(QFont("微软雅黑",9));
    ui->lineEditNewpw->setFont(QFont("微软雅黑",9));

    //菜单栏
    ui->frameTab1->setGeometry(180,45,819,654);
    QImage *imageTab1=new QImage;
    imageTab1->load("img\\a_bgTab1.png");
    ui->labelTab1->setGeometry(0,0,820,655);
    ui->labelTab1->setText("");
    ui->labelTab1->setPixmap(QPixmap::fromImage(*imageTab1));
    ui->tvMenu->setGeometry(22,50,776,475);
    ui->btnMenuRefresh->setGeometry(350,20,120,30);
    ui->btnMenuRefresh->setText("");
    ui->btnMenuNew->setGeometry(470,20,120,30);
    ui->btnMenuNew->setText("");
    ui->btnMenuDelete->setGeometry(590,20,120,30);
    ui->btnMenuDelete->setText("");
    ui->tvMenu->setAlternatingRowColors(true);
    ui->tvMenu->setStyleSheet( "QTableView{"
                               "    color: rgb(0,0,0);"
                               "    background-color: rgb(230,230,230);"
                               "    selection-background-color: rgb(23,164,232);"
                               "    alternate-background-color: rgb(200,200,200);"
                               "    gridline-color: rgba(0,0,0,0);"
                               "}");
    ui->tvMenu->setColumnWidth(0,250);
    ui->tvMenu->setColumnWidth(1,100);
    ui->tvMenu->setColumnWidth(2,100);
    ui->tvMenu->setColumnWidth(3,100);
    ui->tvMenu->setColumnWidth(4,100);
    ui->tvMenu->setColumnWidth(5,100);
    ui->tvMenu->setShowGrid(false);
    ui->tvMenu->verticalHeader()->setVisible(false);
    QFont btnfont("微软雅黑",8,50);
    ui->btnMenuRefresh->setFont(btnfont);
    ui->btnMenuNew->setFont(btnfont);
    ui->btnMenuDelete->setFont(btnfont);
    ui->btnMenuRefresh->setStyleSheet("QPushButton { "
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,255,255,0.15);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnMenuNew->setStyleSheet("QPushButton { "
                                  "    background-color: rgba(0,0,0,0);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.15);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}");
    ui->btnMenuDelete->setStyleSheet("QPushButton { "
                                     "    background-color: rgba(0,0,0,0);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.15);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}");
    ui->labelMenuTotal->setGeometry(55,531,180,30);
    ui->labelMenuTotal->setAlignment(Qt::AlignLeft);
    ui->labelMenuTotal->setText("菜品总数：");
    ui->labelMenuTotal->setStyleSheet("color: rgb(250,250,250)");
    ui->labelMenuTotal->setFont(btnfont);

    ui->labelMenuNum1->setGeometry(285,531,100,30);
    ui->labelMenuNum2->setGeometry(385,531,100,30);
    ui->labelMenuNum3->setGeometry(485,531,100,30);
    ui->labelMenuNum4->setGeometry(585,531,100,30);
    ui->labelMenuNum5->setGeometry(685,531,100,30);
    ui->labelMenuNum1->setStyleSheet("color: rgb(250,250,250)");
    ui->labelMenuNum2->setStyleSheet("color: rgb(250,250,250)");
    ui->labelMenuNum3->setStyleSheet("color: rgb(250,250,250)");
    ui->labelMenuNum4->setStyleSheet("color: rgb(250,250,250)");
    ui->labelMenuNum5->setStyleSheet("color: rgb(250,250,250)");
    ui->labelMenuNum1->setText("冷菜：");
    ui->labelMenuNum2->setText("热菜：");
    ui->labelMenuNum3->setText("汤：");
    ui->labelMenuNum4->setText("主食：");
    ui->labelMenuNum5->setText("饮料：");
    ui->labelMenuNum1->setAlignment(Qt::AlignLeft);
    ui->labelMenuNum2->setAlignment(Qt::AlignLeft);
    ui->labelMenuNum3->setAlignment(Qt::AlignLeft);
    ui->labelMenuNum4->setAlignment(Qt::AlignLeft);
    ui->labelMenuNum5->setAlignment(Qt::AlignLeft);
    ui->labelMenuNum1->setFont(btnfont);
    ui->labelMenuNum2->setFont(btnfont);
    ui->labelMenuNum3->setFont(btnfont);
    ui->labelMenuNum4->setFont(btnfont);
    ui->labelMenuNum5->setFont(btnfont);

    ui->lineEditFind->setGeometry(35,610,545,20);
    ui->lineEditFind->setFont(btnfont);
    ui->btnFindNext->setGeometry(690,610,90,20);
    ui->btnFindNext->setFont(btnfont);
    ui->btnFindLast->setGeometry(590,610,90,20);
    ui->btnFindLast->setFont(btnfont);

    //餐桌信息栏
    ui->frameTab2->setGeometry(180,45,819,654);
    QImage *imageTab2=new QImage;
    imageTab2->load("img\\a_bgTab2.png");
    ui->labelTab2->setGeometry(0,0,820,655);
    ui->labelTab2->setText("");
    ui->labelTab2->setPixmap(QPixmap::fromImage(*imageTab2));
    ui->labelTableCount->setGeometry(55,612,150,30);
    ui->labelTableCount->setStyleSheet("color: rgb(250,250,250)");
    ui->labelTableCount->setFont(btnfont);
    ui->labelTableCount->setAlignment(Qt::AlignLeft);
    ui->labelTableNum->setGeometry(290,612,150,30);
    ui->labelTableNum->setStyleSheet("color: rgb(250,250,250)");
    ui->labelTableNum->setFont(btnfont);
    ui->labelTableNum->setAlignment(Qt::AlignLeft);

    ui->tvTable->setGeometry(22,50,776,555);
    ui->btnTableRefresh->setGeometry(350,20,120,30);
    ui->btnTableRefresh->setText("");
    ui->btnTableNew->setGeometry(470,20,120,30);
    ui->btnTableNew->setText("");
    ui->btnTableDelete->setGeometry(590,20,120,30);
    ui->btnTableDelete->setText("");
    ui->btnTableRefresh->setStyleSheet("QPushButton { "
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,255,255,0.15);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnTableNew->setStyleSheet("QPushButton { "
                                  "    background-color: rgba(0,0,0,0);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.15);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}");
    ui->btnTableDelete->setStyleSheet("QPushButton { "
                                     "    background-color: rgba(0,0,0,0);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.15);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}");
    ui->tvTable->setAlternatingRowColors(true);
    ui->tvTable->setStyleSheet( "QTableView{"
                                "    color: rgb(0,0,0);"
                                "    background-color: rgb(230,230,230);"
                                "    alternate-background-color: rgb(200,200,200);"
                                "    gridline-color: rgba(0,0,0,0);"
                                "}");
    ui->tvTable->setColumnWidth(0,250);
    ui->tvTable->setColumnWidth(1,150);
    ui->tvTable->setColumnWidth(2,150);
    ui->tvTable->setShowGrid(false);
    ui->tvTable->verticalHeader()->setVisible(false);
    ui->tvTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->btnTableRefresh->setFont(btnfont);
    ui->btnTableNew->setFont(btnfont);
    ui->btnTableDelete->setFont(btnfont);

    //顾客账号栏
    ui->frameTab3->setGeometry(180,45,819,654);
    QImage *imageTab3=new QImage;
    imageTab3->load("img\\a_bgTab3.png");
    ui->labelTab3->setGeometry(0,0,820,655);
    ui->labelTab3->setText("");
    ui->labelTab3->setPixmap(QPixmap::fromImage(*imageTab3));
    ui->labelGuestCount->setGeometry(55,612,150,30);
    ui->labelGuestCount->setFont(btnfont);
    ui->labelGuestCount->setStyleSheet("color: rgb(250,250,250)");
    ui->labelGuestCount->setAlignment(Qt::AlignLeft);

    ui->tvGuest->setGeometry(22,50,776,555);
    ui->btnGuestRefresh->setGeometry(350,20,120,30);
    ui->btnGuestRefresh->setText("");
    ui->btnGuestNew->setGeometry(470,20,120,30);
    ui->btnGuestNew->setText("");
    ui->btnGuestDelete->setGeometry(590,20,120,30);
    ui->btnGuestDelete->setText("");
    ui->btnGuestRefresh->setStyleSheet("QPushButton { "
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,255,255,0.15);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnGuestNew->setStyleSheet("QPushButton { "
                                  "    background-color: rgba(0,0,0,0);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.15);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}");
    ui->btnGuestDelete->setStyleSheet("QPushButton { "
                                     "    background-color: rgba(0,0,0,0);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.15);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}");

    ui->tvGuest->setAlternatingRowColors(true);
    ui->tvGuest->setStyleSheet( "QTableView{"
                                "    color: rgb(0,0,0);"
                                "    background-color: rgb(230,230,230);"
                                "    alternate-background-color: rgb(200,200,200);"
                                "    gridline-color: rgba(0,0,0,0);"
                                "}");
    ui->tvGuest->setColumnWidth(0,150);
    ui->tvGuest->setColumnWidth(1,250);
    ui->tvGuest->setColumnWidth(2,150);
    ui->tvGuest->setColumnWidth(3,150);
    ui->tvGuest->setShowGrid(false);
    ui->tvGuest->verticalHeader()->setVisible(false);
    ui->tvGuest->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->btnGuestRefresh->setFont(btnfont);
    ui->btnGuestNew->setFont(btnfont);
    ui->btnGuestDelete->setFont(btnfont);

    //其他账号栏
    ui->frameTab4->setGeometry(180,45,819,654);
    QImage *imageTab4=new QImage;
    imageTab4->load("img\\a_bgTab4.png");
    ui->labelTab4->setGeometry(0,0,820,655);
    ui->labelTab4->setText("");
    ui->labelTab4->setPixmap(QPixmap::fromImage(*imageTab4));
    ui->labelOtherCount->setGeometry(55,612,150,30);
    ui->labelOtherCount->setStyleSheet("color: rgb(250,250,250)");
    ui->labelOtherCount->setFont(btnfont);
    ui->labelOtherCount->setAlignment(Qt::AlignLeft);
    ui->labelOtherNum1->setGeometry(270,612,100,30);
    ui->labelOtherNum2->setGeometry(370,612,100,30);
    ui->labelOtherNum3->setGeometry(470,612,100,30);
    ui->labelOtherNum1->setFont(btnfont);
    ui->labelOtherNum2->setFont(btnfont);
    ui->labelOtherNum3->setFont(btnfont);
    ui->labelOtherNum1->setStyleSheet("color: rgb(250,250,250)");
    ui->labelOtherNum2->setStyleSheet("color: rgb(250,250,250)");
    ui->labelOtherNum3->setStyleSheet("color: rgb(250,250,250)");
    ui->labelOtherNum1->setAlignment(Qt::AlignLeft);
    ui->labelOtherNum2->setAlignment(Qt::AlignLeft);
    ui->labelOtherNum3->setAlignment(Qt::AlignLeft);

    ui->tvOther->setGeometry(22,50,776,555);
    ui->btnOtherRefresh->setGeometry(350,20,120,30);
    ui->btnOtherRefresh->setText("");
    ui->btnOtherNew->setGeometry(470,20,120,30);
    ui->btnOtherNew->setText("");
    ui->btnOtherDelete->setGeometry(590,20,120,30);
    ui->btnOtherDelete->setText("");
    ui->btnOtherRefresh->setStyleSheet("QPushButton { "
                                      "    background-color: rgba(0,0,0,0);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(255,255,255,0.15);"
                                      "    border: 1px solid rgba(0,0,0,0);"
                                      "}");
    ui->btnOtherNew->setStyleSheet("QPushButton { "
                                  "    background-color: rgba(0,0,0,0);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: rgba(255,255,255,0.15);"
                                  "    border: 1px solid rgba(0,0,0,0);"
                                  "}");
    ui->btnOtherDelete->setStyleSheet("QPushButton { "
                                     "    background-color: rgba(0,0,0,0);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(255,255,255,0.15);"
                                     "    border: 1px solid rgba(0,0,0,0);"
                                     "}");

    ui->tvOther->setAlternatingRowColors(true);
    ui->tvOther->setStyleSheet( "QTableView{"
                                "    color: rgb(0,0,0);"
                                "    background-color: rgb(230,230,230);"
                                "    alternate-background-color: rgb(200,200,200);"
                                "    gridline-color: rgba(0,0,0,0);"
                                "}");
    ui->tvOther->setColumnWidth(0,150);
    ui->tvOther->setColumnWidth(1,150);
    ui->tvOther->setColumnWidth(2,150);
    ui->tvOther->setColumnWidth(3,150);
    ui->tvOther->setShowGrid(false);
    ui->tvOther->verticalHeader()->setVisible(false);
    ui->tvOther->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->btnOtherRefresh->setFont(btnfont);
    ui->btnOtherNew->setFont(btnfont);
    ui->btnOtherDelete->setFont(btnfont);

    //更改管理员账户
    ui->frameTab5->setGeometry(180,45,819,654);
    QImage *imageTab5=new QImage;
    imageTab5->load("img\\a_bgTab5.png");
    ui->labelTab5->setGeometry(0,0,820,655);
    ui->labelTab5->setText("");
    ui->labelTab5->setPixmap(QPixmap::fromImage(*imageTab5));

    ui->labelChangeName->setGeometry(40,65,100,20);
    ui->labelChangeOldpw->setGeometry(40,110,100,20);
    ui->labelChangeNewpw->setGeometry(40,155,100,20);
    ui->labelChangeConfirm->setGeometry(40,200,100,20);
    ui->labelChangeName->setFont(QFont("微软雅黑",10));
    ui->labelChangeOldpw->setFont(QFont("微软雅黑",10));
    ui->labelChangeNewpw->setFont(QFont("微软雅黑",10));
    ui->labelChangeConfirm->setFont(QFont("微软雅黑",10));
    ui->lineEditChangeName->setGeometry(140,65,150,20);
    ui->lineEditChangepw->setGeometry(140,110,150,20);
    ui->lineEditNewpw->setGeometry(140,155,150,20);
    ui->lineEditConfirmpw->setGeometry(140,200,150,20);
    ui->btnChange->setGeometry(100,250,130,30);
    ui->btnChange->setFont(QFont("微软雅黑",10));

    ui->tvGuest->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvOther->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

/*
 * 关闭按钮
 */
void MainWindow::slot_close() {
    this->close();
}

/*
 * 最小化按钮
 */
void MainWindow::slot_min() {
    this->showMinimized();
}

/*
 * 界面切换按钮样式设置
 */
void MainWindow::switchTab(QPushButton *btn) {
    btnTab1->setStyleSheet("QPushButton {"
                           "    background-color: rgba(255,255,255,0);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(255,255,255,0.1);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}");
    btnTab2->setStyleSheet("QPushButton {"
                           "    background-color: rgba(255,255,255,0);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(255,255,255,0.1);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}");
    btnTab3->setStyleSheet("QPushButton {"
                           "    background-color: rgba(255,255,255,0);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(255,255,255,0.1);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}");
    btnTab4->setStyleSheet("QPushButton {"
                           "    background-color: rgba(255,255,255,0);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(255,255,255,0.1);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}");
    btnTab5->setStyleSheet("QPushButton {"
                           "    background-color: rgba(255,255,255,0);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(255,255,255,0.1);"
                           "    border: 1px solid rgba(255,255,255,0);"
                           "}");
    btn->setStyleSheet("QPushButton {"
                       "    background-color: rgba(255,255,255,0.15);"
                       "    border: 1px solid rgba(255,255,255,0);"
                       "}"
                       "QPushButton:hover {"
                       "    background-color: rgba(255,255,255,0.1);"
                       "    border: 1px solid rgba(255,255,255,0);"
                       "}");
}

/*
 * 切换到菜单表
 */
void MainWindow::slot_tab1() {
    on_btnMenuRefresh_clicked();

    switchTab(btnTab1);
    ui->frameTab1->setVisible(true);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
}

/*
 * 切换到餐桌信息表
 */
void MainWindow::slot_tab2() {
    switchTab(btnTab2);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(true);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
}

/*
 * 切换到用户账户信息表
 */
void MainWindow::slot_tab3() {
    switchTab(btnTab3);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(true);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
}

/*
 * 切换到其他账号信息表
 */
void MainWindow::slot_tab4() {
    switchTab(btnTab4);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(true);
    ui->frameTab5->setVisible(false);
}

/*
 * 切换到更改管理员账户界面
 */
void MainWindow::slot_tab5() {
    switchTab(btnTab5);
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(true);
}

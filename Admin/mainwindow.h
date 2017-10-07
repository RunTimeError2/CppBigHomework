/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：主界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "admin.h"
#include "bglabel.h"
#include <QPushButton>
//#include "vlabellist.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPushButton *btnTab1,*btnTab2,*btnTab3,*btnTab4,*btnTab5;
    QString backcolor,bordercolor,hovercolor,selectedcolor;
    void setUpUI();
    void switchTab(QPushButton*);

    void updateMenuLabels();
    void updateTableLabels();
    void updateGuestLabels();
    void updateOtherLabels();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    admin Admin;

private slots:
    void on_btnChange_clicked();

    void on_btnMenuRefresh_clicked();
    void on_btnMenuNew_clicked();
    void on_btnMenuDelete_clicked();

    void on_btnGuestRefresh_clicked();
    void on_btnGuestNew_clicked();
    void on_btnGuestDelete_clicked();

    void on_btnOtherRefresh_clicked();
    void on_btnOtherNew_clicked();
    void on_btnOtherDelete_clicked();

    void on_btnTableRefresh_clicked();
    void on_btnTableNew_clicked();
    void on_btnTableDelete_clicked();

    void slot_close();
    void slot_min();

    void slot_tab1();
    void slot_tab2();
    void slot_tab3();
    void slot_tab4();
    void slot_tab5();

    void findMenuNext();
    void findMenuLast();

private:
    Ui::MainWindow *ui;
    bool tableEdited;
    BGLabel *bgLabel;
};

#endif // MAINWINDOW_H

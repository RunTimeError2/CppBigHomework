/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：服务器端主界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "tcpserver.h"
#include "bglabel.h"
#include "mainlogic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    int port;
    Login *mainLogin;
    TcpServer *server;
    mainLogic *mainlogic;
    void setupUI();
    BGLabel *bgLabel;
    QString getIPAddress();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotCreateServer();
    //void updateServer(QString,int);
    void slot_close();
    void slot_minimize();
    void slot_closeServer();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

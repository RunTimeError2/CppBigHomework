/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：厨师端主界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QStandardItemModel>
#include "cdataprocessing.h"
#include "buttondelegate.h"
#include "bglabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    cDataProcessing *dataprocessing;                //数据处理类
    bool status,isRun;                              //状态，是否已连接到服务器端
    bool state;                                     //true为登录，false为注册
    bool set_feedback,select_feedback;              //负责认领菜品的反馈，显示是否成功
    int set_index,select_index,set_state;           //负责设置菜品状态的反馈
    int port;                                       //端口
    QHostAddress *serverIP;                         //存储输入的服务器端IP
    QString g_username,g_phonenumber,g_password;    //登录成功以后储存手机号、用户名、密码
    QTcpSocket *tcpSocket;                          //联网Socket类
    BGLabel *bgLabel;                               //实现自定义无边框窗口

    void setUpLoginForm();                          //初始化登录界面
    void setUpMainForm();                           //初始化主界面

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void btn_Login();
    void btn_Register();

    void slot_cook_setdishstate(int,int);

    void slot_login_success(); //登录成功
    void slot_login_usernameerror(); //登录失败，用户名不存在
    void slot_login_passworderror(); //登录失败，密码错误
    void slot_register_success(); //注册成功
    void slot_register_usernameerror(); //注册失败，用户名已存在

    void slot_cook_getunstarted(QList<SingleDishOrdered>&);
    void slot_cook_getselected(QList<SingleDishOrdered>&);

    void slot_btnRefreshValidDish();
    void slot_btnSelectdish(int row=-1,int col=-1);
    void slot_btnRefreshSelectedDish();
    void slot_btnToDoing(int row=-1,int col=-1);
    void slot_btnToFinished(int row=-1,int col=-1);

    void slot_btnToLogin();
    void slot_btnToRegister();
    void slot_closeWindow();
    void slot_minWindow();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

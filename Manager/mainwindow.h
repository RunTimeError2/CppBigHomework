/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：经理端主界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "bglabel.h"
#include "mdataprocessing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    mDataProcessing *dataprocessing;                //数据处理类
    bool status;                                    //表示是否联网
    bool state;                                     //true为登录，false为注册
    int port;                                       //端口
    bool isRun;                                     //表示是否成功登录
    QHostAddress *serverIP;                         //存储输入的服务器端IP地址
    QString w_username,w_phonenumber,w_password;    //登录成功以后储存手机号、用户名、密码
    QTcpSocket *tcpSocket;
    BGLabel *bgLabel;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    

    void setupLoginForm();                          //初始化登录界面
    void setupMainForm();                           //初始化主界面

public slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void btn_Login();
    void btn_Register();

    void slot_login_success();                      //登录成功
    void slot_login_usernameerror();                //登录失败，用户名不存在
    void slot_login_passworderror();                //登录失败，密码错误
    void slot_register_success();                   //注册成功
    void slot_register_usernameerror();             //注册失败，用户名已存在

    void manager_displayReport();                   //显示数据报表
    void manager_getreport_request();               //发送获取报表请求
    void tableView_buttonclicked(int,int);          //显示服务员评价的按钮

    void slot_btnToLogin();
    void slot_btnToRegister();
    void slot_closeWindow();
    void slot_minWindow();
    void slot_btnToWaiter();
    void slot_btnToCook();
    void slot_btnToOther();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

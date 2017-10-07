/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：服务员端主界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "wdataprocessing.h"
#include "buttondelegate.h"
#include "bglabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    wDataProcessing *dataprocessing;                //数据处理类
    bool status;                                    //是否联网
    bool state;                                     //true为登录，false为注册
    bool select_feedback;                           //用于实现选桌操作的反馈，即显示是否选桌成功
    int table_feedback;
    int port;                                       //端口
    bool isRun;                                     //是否成功登录
    int msg_target;
    QHostAddress *serverIP;                         //储存用户输入的服务器端IP地址
    QString w_username,w_phonenumber,w_password;    //登录成功以后储存手机号、用户名、密码
    QMap<int,bool> tmp_dishstate;
    QTcpSocket *tcpSocket;
    BGLabel *bgLabelLogin,*bgLabelMain;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void btn_Login();
    void btn_Register();

    void slot_login_success(); //登录成功
    void slot_login_usernameerror(); //登录失败，用户名不存在
    void slot_login_passworderror(); //登录失败，密码错误
    void slot_register_success(); //注册成功
    void slot_register_usernameerror(); //注册失败，用户名已存在

    void slot_waiter_getallinfo();
    void slot_btnSelectTable(int row=-1,int col=-1);

    void slot_tableviewClicked();
    void slot_sendmsg();
    void slot_singlemessage(QString);

    void slot_btnToLogin();
    void slot_btnToRegister();
    void slot_closeWindow();
    void slot_minWindow();
    void InitLoginForm();
    void InitMainForm();
    void slot_btnToTable();
    void slot_btnToDish();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：tcpclientsocket.h
 * 摘要：服务器端网络模块客户端部分
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include "dataprocessing.h"
#include "login.h"
#include "mainlogic.h"

/*
 * 与单个客户端连接的TcpSocket
 */
class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    TcpClientSocket(QObject *parent=0,mainLogic *mlogic=0);
    dataProcessing *ProcessingData;                 //消息编码、解码类
    Login *login;                                   //操作数据库的类
    mainLogic *mainlogic;                           //主逻辑
    int type;                                       //1-顾客，2-厨师，3-服务员，4-经理
    QString c_phonenumber,c_username,c_password;    //存储用户的手机号、用户名、密码

signals:
    void updateClients(QString,int);                //收到消息，发送给服务器类进行处理
    void disconnected(int);                         //断开连接，发送给服务器类进行处理

protected slots:
    void dataReceived();                            //收到消息
    void slotDisconnected();                        //断开连接

public slots:
    /*
     * 顾客部分
     */
    void slot_guest_login(QString,QString);
    void slot_guest_register(QString,QString,QString);
    void slot_guest_getmenu();
    void slot_guest_sendtable();
    void slot_guest_choosetable(int);
    void slot_guest_evaluate(dataStructure::evaluateList);
    void slot_guest_orderfood(dataStructure::orderList);
    void slot_guest_getorderlist();
    void slot_guest_payconfirm();
    void slot_guest_msgtowaiter(QString);
    void slot_guest_getmsgqueue();
    void slot_guest_evaluatewaiter(QString,QString,int);
    void slot_guest_changeaccount(QString,QString,QString);

    /*
     * 厨师部分
     */
    void slot_cook_login(QString,QString);
    void slot_cook_register(QString,QString);
    void slot_cook_getselected();
    void slot_cook_getunstarted();
    void slot_cook_selectdish(int);
    void slot_cook_setdishstate(int,int);
    void slot_cook_getallinfo();

    /*
     * 服务员部分
     */
    void slot_waiter_login(QString,QString);
    void slot_waiter_register(QString,QString);
    void slot_waiter_selecttable(int);
    void slot_waiter_getallinfo();
    void slot_waiter_msgtoguest(QString,int);

    /*
     * 经理部分
     */
    void slot_manager_login(QString,QString);
    void slot_manager_register(QString,QString);
    void slot_manager_report();
};

#endif // TCPCLIENTSOCKET_H
    //void slot_guest_getusername();
    //void slot_guest_pay(float,QString);

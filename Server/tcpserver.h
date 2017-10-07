/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：tcpserver.h
 * 摘要：服务器端网络模块服务器部分
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "tcpclientsocket.h"
#include "login.h"
#include "mainlogic.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    Login *serverLogin;
    mainLogic *mainlogic;
    TcpServer(QObject *parent=0,int port=0,mainLogic *mlogic=0);
    QList<TcpClientSocket*> TcpClientSocketList;        //储存所有客户端类的列表
    void exit();

public slots:
    void slotDisconnected(int);                         //客户端断开连接

    void updateAllOrderInfo();                          //实时更新所有信息
    void updateAllOrderInfoExceptMe(QString);           //实时更新信息，但不发送给修改者以防网路阻塞
    void slot_MessagetoGuest(QByteArray,QString);       //给顾客发消息
    void slot_MessagetoWaiter(QByteArray,QString);      //给服务员发消息

protected:
    void incomingConnection(int socketDescriptor);      //收到连接请求并处理
};

#endif // TCPSERVER_H

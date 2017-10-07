/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：tcpserver.cpp
 * 摘要：服务器端网络模块服务器部分
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "tcpserver.h"
#include <QMessageBox>

TcpServer::TcpServer(QObject *parent,int port,mainLogic *mlogic):QTcpServer(parent) {
    listen(QHostAddress::Any,port); //开始监听
    mainlogic=mlogic;
}

/*
 * 收到连接请求并处理
 */
void TcpServer::incomingConnection(int socketDescriptor) {
    TcpClientSocket *tcpClientSocket=new TcpClientSocket(this,mainlogic);

    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocket->login=serverLogin; //连接到数据库

    TcpClientSocketList.append(tcpClientSocket);
}

/*
 * 客户端断开连接
 */
void TcpServer::slotDisconnected(int descriptor) {
    for(int i=0;i<TcpClientSocketList.count();i++) {
        QTcpSocket *item=TcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor) {
            TcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}

/*
 * 断开和所有客户端的连接
 */
void TcpServer::exit() {
    for(int i=0;i<TcpClientSocketList.count();i++)
        TcpClientSocketList.at(i)->disconnectFromHost();
    TcpClientSocketList.clear();
    this->close();
}

/*
 * 更新所有与点菜及菜品状态有关的信息
 */
void TcpServer::updateAllOrderInfo() {
    for(int i=0;i<TcpClientSocketList.count();i++) {
        switch(TcpClientSocketList.at(i)->type) {
            case 1: { //顾客
                QByteArray msg;
                msg=mainlogic->guest_EncodingOrderlist(TcpClientSocketList.at(i)->c_phonenumber);
                TcpClientSocketList.at(i)->write(msg,msg.length());
                break;
            }
            case 2: { //厨师
                QByteArray msg;
                msg=mainlogic->cook_encodingAll(TcpClientSocketList.at(i)->c_username);
                TcpClientSocketList.at(i)->write(msg,msg.length()); //发送厨师所需所有信息
                break;
            }
            case 3: { //服务员
                TcpClientSocketList.at(i)->slot_waiter_getallinfo();
                break;
            }
            case 4: { //经理

                break;
            }
        }
    }
}

/*
 * 更新所有与点菜及菜品状态有关的信息，但不给指定顾客端发送，以免造成阻塞
 */
void TcpServer::updateAllOrderInfoExceptMe(QString phonenumber) {
    for(int i=0;i<TcpClientSocketList.count();i++) {
        switch(TcpClientSocketList.at(i)->type) {
            case 1: { //顾客
                if(TcpClientSocketList.at(i)->c_phonenumber!=phonenumber) {
                    QByteArray msg;
                    msg=mainlogic->guest_EncodingOrderlist(TcpClientSocketList.at(i)->c_phonenumber);
                    TcpClientSocketList.at(i)->write(msg,msg.length());
                }
                break;
            }
            case 2: { //厨师
                QByteArray msg;
                msg=mainlogic->cook_encodingAll(TcpClientSocketList.at(i)->c_username);
                TcpClientSocketList.at(i)->write(msg,msg.length()); //发送厨师所需所有信息
                break;
            }
            case 3: { //服务员
                TcpClientSocketList.at(i)->slot_waiter_getallinfo();
                break;
            }
            case 4: { //经理
                QByteArray msg=mainlogic->manager_generateReport();
                TcpClientSocketList.at(i)->write(msg,msg.length());
                break;
            }
        }
    }
}

/*
 * 给指定顾客发送消息
 */
void TcpServer::slot_MessagetoGuest(QByteArray msg,QString phonenumber) {
    for(int i=0;i<TcpClientSocketList.count();i++)
        if(TcpClientSocketList.at(i)->type==1 && TcpClientSocketList.at(i)->c_phonenumber==phonenumber)
            TcpClientSocketList.at(i)->write(msg,msg.length());
}

/*
 * 给指定服务员发送消息
 */
void TcpServer::slot_MessagetoWaiter(QByteArray msg,QString username) {
    for(int i=0;i<TcpClientSocketList.count();i++)
        if(TcpClientSocketList.at(i)->type==3 && TcpClientSocketList.at(i)->c_username==username)
            TcpClientSocketList.at(i)->write(msg,msg.length());
}

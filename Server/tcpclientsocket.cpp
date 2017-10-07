/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：tcpclientsocket.cpp
 * 摘要：服务器端网络模块客户端部分
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "tcpclientsocket.h"
#include <QMessageBox>

TcpClientSocket::TcpClientSocket(QObject *parent,mainLogic *mlogic)
{
    mainlogic=mlogic;

    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    ProcessingData=new dataProcessing;

    connect(ProcessingData,SIGNAL(guest_login(QString,QString)),this,SLOT(slot_guest_login(QString,QString)));
    connect(ProcessingData,SIGNAL(guest_register(QString,QString,QString)),this,SLOT(slot_guest_register(QString,QString,QString)));
    connect(ProcessingData,SIGNAL(guest_sendtable()),this,SLOT(slot_guest_sendtable()));
    connect(ProcessingData,SIGNAL(guest_getmenu()),this,SLOT(slot_guest_getmenu()));
    connect(ProcessingData,SIGNAL(guest_choosetable(int)),this,SLOT(slot_guest_choosetable(int)));
    connect(ProcessingData,SIGNAL(guest_evaluate(dataStructure::evaluateList)),this,SLOT(slot_guest_evaluate(dataStructure::evaluateList)));
    connect(ProcessingData,SIGNAL(guest_orderfood(dataStructure::orderList)),this,SLOT(slot_guest_orderfood(dataStructure::orderList)));
    connect(ProcessingData,SIGNAL(guest_getorderlist()),this,SLOT(slot_guest_getorderlist()));
    connect(ProcessingData,SIGNAL(guest_payconfirm()),this,SLOT(slot_guest_payconfirm()));
    connect(ProcessingData,SIGNAL(guest_msgtowaiter(QString)),this,SLOT(slot_guest_msgtowaiter(QString)));
    connect(ProcessingData,SIGNAL(guest_getmsgqueue()),this,SLOT(slot_guest_getmsgqueue()));
    connect(ProcessingData,SIGNAL(guest_evaluatewaiter(QString,QString,int)),this,SLOT(slot_guest_evaluatewaiter(QString,QString,int)));
    connect(ProcessingData,SIGNAL(guest_changeaccount(QString,QString,QString)),this,SLOT(slot_guest_changeaccount(QString,QString,QString)));

    connect(ProcessingData,SIGNAL(cook_login(QString,QString)),this,SLOT(slot_cook_login(QString,QString)));
    connect(ProcessingData,SIGNAL(cook_register(QString,QString)),this,SLOT(slot_cook_register(QString,QString)));
    connect(ProcessingData,SIGNAL(cook_getselected()),this,SLOT(slot_cook_getselected()));
    connect(ProcessingData,SIGNAL(cook_getunstarted()),this,SLOT(slot_cook_getunstarted()));
    connect(ProcessingData,SIGNAL(cook_selectdish(int)),this,SLOT(slot_cook_selectdish(int)));
    connect(ProcessingData,SIGNAL(cook_setdishstate(int,int)),this,SLOT(slot_cook_setdishstate(int,int)));
    connect(ProcessingData,SIGNAL(cook_getallinfo()),this,SLOT(slot_cook_getallinfo()));

    connect(ProcessingData,SIGNAL(waiter_login(QString,QString)),this,SLOT(slot_waiter_login(QString,QString)));
    connect(ProcessingData,SIGNAL(waiter_register(QString,QString)),this,SLOT(slot_waiter_register(QString,QString)));
    connect(ProcessingData,SIGNAL(waiter_selecttable(int)),this,SLOT(slot_waiter_selecttable(int)));
    connect(ProcessingData,SIGNAL(waiter_getallinfo()),this,SLOT(slot_waiter_getallinfo()));
    connect(ProcessingData,SIGNAL(waiter_msgtoguest(QString,int)),this,SLOT(slot_waiter_msgtoguest(QString,int)));

    connect(ProcessingData,SIGNAL(manager_login(QString,QString)),this,SLOT(slot_manager_login(QString,QString)));
    connect(ProcessingData,SIGNAL(manager_register(QString,QString)),this,SLOT(slot_manager_register(QString,QString)));
    connect(ProcessingData,SIGNAL(manager_report()),this,SLOT(slot_manager_report()));
}

/*
 * 收到消息
 */
void TcpClientSocket::dataReceived() {
    while(bytesAvailable()>0) {
        int length=bytesAvailable();
        QByteArray bData;
        bData.resize(length);
        read(bData.data(),length);
        ProcessingData->ProcessData(bData);
    }
}

/*
 * 断开连接
 */
void TcpClientSocket::slotDisconnected() {
    emit disconnected(this->socketDescriptor());

    try {

    switch(type) {
        case 1: { //顾客
            mainlogic->guest_logout(c_phonenumber);
            break;
        }
        case 2: { //厨师
            mainlogic->cook_logout(c_username);
            break;
        }
        case 3: { //服务员
            mainlogic->waiter_logout(c_username);
            break;
        }
        case 4: { //经理
            mainlogic->manager_logout(c_username);
            break;
        }
    }

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 返回客户登录结果
 */
void TcpClientSocket::slot_guest_login(QString phonenumber, QString password) {
    try {

    int result=mainlogic->guest_login(phonenumber,password);
    QByteArray msg;
    msg.append((unsigned char)TOKEN_GUEST_LOGIN);
    msg.append((unsigned char)result);
    this->write(msg,2);
    if(result==1) {
        c_phonenumber=phonenumber;
        c_password=password;
        type=1; //顾客
    }

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 返回客户注册结果
 */
void TcpClientSocket::slot_guest_register(QString phonenumber,QString username,QString password) {
    QByteArray msg;
    msg.append((unsigned char)TOKEN_GUEST_REGISTER);

    try {

    int result=login->guestRegister(phonenumber,username,password);
    if(result==1)
        mainlogic->guest_register(phonenumber,username,password);
    msg.append((unsigned char)result);
    this->write(msg,2);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 返回菜单
 */
void TcpClientSocket::slot_guest_getmenu() {
    try {

    QByteArray msg=login->EncodingMenu();
    QString username=login->guestGetUsername(c_phonenumber,c_password);
    QByteArray tmp=username.toUtf8();
    msg.append((unsigned char)tmp.length());
    msg.append(tmp);

    this->write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客选桌
 */
void TcpClientSocket::slot_guest_choosetable(int index) {
    try {

    bool success=mainlogic->guest_choosetable(index,c_phonenumber);
    QByteArray msg;
    if(success)
        msg.append(TOKEN_GUEST_CHOOSESUCCESS);
    else
        msg.append(TOKEN_GUEST_CHOOSEFAILURE);
    write(msg,1);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师登录
 */
void TcpClientSocket::slot_cook_login(QString username,QString password) {
    try {

    int result=mainlogic->cook_login(username,password);
    QByteArray msg;
    msg.append((unsigned char)TOKEN_COOK_LOGIN);
    msg.append((unsigned char)result);
    this->write(msg,2);
    if(result==1) {
        c_username=username;
        c_password=password;
        type=2; //厨师
    }

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师注册
 */
void TcpClientSocket::slot_cook_register(QString username,QString password) {
    QByteArray msg;
    msg.append((unsigned char)TOKEN_COOK_REGISTER);

    try {

    int result=login->cookRegister(username,password);
    if(result==1)
        mainlogic->cook_register(username,password);
    msg.append((unsigned char)result);
    this->write(msg,2);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 服务员登录
 */
void TcpClientSocket::slot_waiter_login(QString username,QString password) {
    try {

    int result=mainlogic->waiter_login(username,password);
    QByteArray msg;
    msg.append((unsigned char)TOKEN_WAITER_LOGIN);
    msg.append((unsigned char)result);
    this->write(msg,2);
    if(result==1) {
        c_username=username;
        c_password=password;
        type=3; //服务员
    }

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 服务员注册
 */
void TcpClientSocket::slot_waiter_register(QString username,QString password) {
    try {

    QByteArray msg;
    msg.append((unsigned char)TOKEN_WAITER_REGISTER);
    int result=login->waiterRegister(username,password);
    if(result==1)
        mainlogic->waiter_register(username,password);
    msg.append((unsigned char)result);
    this->write(msg,2);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 经理登录
 */
void TcpClientSocket::slot_manager_login(QString username,QString password) {
    try {

    int result=mainlogic->manager_login(username,password);
    QByteArray msg;
    msg.append((unsigned char)TOKEN_MANAGER_LOGIN);
    msg.append((unsigned char)result);
    this->write(msg,2);
    if(result==1) {
        c_username=username;
        c_password=password;
        type=4; //经理
    }

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 经理注册
 */
void TcpClientSocket::slot_manager_register(QString username,QString password) {
    try {

    QByteArray msg;
    msg.append((unsigned char)TOKEN_MANAGER_REGISTER);
    int result=login->managerRegister(username,password);
    if(result==1)
        mainlogic->manager_register(username,password);
    msg.append((unsigned char)result);
    this->write(msg,2);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 发送餐桌信息
 */
void TcpClientSocket::slot_guest_sendtable() {
    try {

    QByteArray msg;
    msg=mainlogic->guest_Encodingtableinfo(c_phonenumber);
    this->write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 根据顾客评价更新评价信息表
 */
void TcpClientSocket::slot_guest_evaluate(dataStructure::evaluateList list) {
    try {

    login->updateEvaluationTable(list);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客下单
 */
void TcpClientSocket::slot_guest_orderfood(dataStructure::orderList list) {
    try {

    mainlogic->guest_order(list);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师认领菜品
 */
void TcpClientSocket::slot_cook_selectdish(int index) {
    try {

    QByteArray msg;
    bool result=mainlogic->cook_choosedish(index,c_username);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师设置菜品状态
 */
void TcpClientSocket::slot_cook_setdishstate(int index,int state) {
    try {

    bool result=mainlogic->cook_setdishstate(index,state,c_username);
    if(result && state==DISH_FINISHED) { //添加厨师工作记录
        if(mainlogic->cookMap.contains(c_username)) {
            int index=mainlogic->cookMap[c_username];
            if(index>=0 && index<mainlogic->cooks.count()) {
                mainlogic->cooks[index]->record++;
                mainlogic->cooks[index]->tmp_record++;
            }
        }
        login->addcookRecord(c_username);
    }

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师获取已认领菜品列表
 */
void TcpClientSocket::slot_cook_getselected() {
    try {

    QByteArray msg;
    msg=mainlogic->cook_getchosendish(c_username);
    this->write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师获取未认领菜品列表
 */
void TcpClientSocket::slot_cook_getunstarted() {
    try {

    QByteArray msg;
    msg=mainlogic->cook_getunstarteddish();
    this->write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 厨师获取所有所需信息
 */
void TcpClientSocket::slot_cook_getallinfo() {
    try {

    QByteArray msg;
    msg=mainlogic->cook_encodingAll(c_username);
    this->write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 服务员选桌
 */
void TcpClientSocket::slot_waiter_selecttable(int index) {
    try {

    mainlogic->waiter_choosetable(index,c_username);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客获取点菜列表
 */
void TcpClientSocket::slot_guest_getorderlist() {
    try {

    QByteArray msg=mainlogic->guest_EncodingOrderlist(c_phonenumber);
    this->write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客确认付款
 */
void TcpClientSocket::slot_guest_payconfirm() {
    try {

    float price=mainlogic->guests.at(mainlogic->guestMap[c_phonenumber])->price;
    login->guestpay(price,c_phonenumber);
    QString waiter_un;
    if(mainlogic->guestMap.contains(c_phonenumber)) { //添加服务员工作记录
        int waiterindex=mainlogic->guests[mainlogic->guestMap[c_phonenumber]]->waiterIndex;
        if(waiterindex>=0 && waiterindex<mainlogic->waiters.count()) {
            mainlogic->waiters[waiterindex]->record++;
            mainlogic->waiters[waiterindex]->tmp_record++;
            waiter_un=mainlogic->waiters.at(waiterindex)->username;
            login->addwaiterRecord(waiter_un);
        }
    }
    mainlogic->guest_pay(c_phonenumber);
    QByteArray msg;
    msg.append(TOKEN_GUEST_PAY);
    write(msg,1);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 服务员获取所有所需信息
 */
void TcpClientSocket::slot_waiter_getallinfo() {
    try {

    QByteArray msg;
    msg=mainlogic->EncodingWaiterInfo(c_username);
    write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 服务员给顾客发信息
 */
void TcpClientSocket::slot_waiter_msgtoguest(QString msg,int target) {
    try {

    mainlogic->waiter_msgtoguest(msg,target,c_username);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客给服务员发信息
 */
void TcpClientSocket::slot_guest_msgtowaiter(QString msg) {
    try {

    mainlogic->guest_msgtowaiter(msg,c_phonenumber);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客获取消息队列请求
 */
void TcpClientSocket::slot_guest_getmsgqueue() {
    try {

    QByteArray msg;
    msg=mainlogic->guest_EncodingMsgQueue(c_phonenumber);
    write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客修改账号信息
 */
void TcpClientSocket::slot_guest_changeaccount(QString phonenumber,QString username,QString password) {
    try {

    login->changeGuestAccount(phonenumber,username,password);
    mainlogic->guest_ChangeAccount(phonenumber,username,password);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 顾客评价服务员
 */
void TcpClientSocket::slot_guest_evaluatewaiter(QString username,QString comment,int score) {
    try {

    login->evaluateWaiter(username,comment,score);
    mainlogic->guest_evaluateWaiter(username,comment,score);

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

/*
 * 向餐厅经理发送报表
 */
void TcpClientSocket::slot_manager_report() {
    try {

    QByteArray msg=mainlogic->manager_generateReport();
    msg.append(login->EncodingPayRecord());
    write(msg,msg.length());

    } catch (QString err) {
        //QMessageBox::critical(this,"Error",err);
    }
}

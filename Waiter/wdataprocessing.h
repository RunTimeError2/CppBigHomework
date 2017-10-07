/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：wdataprocessing.h
 * 摘要：服务员端数据处理模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef WDATAPROCESSING_H
#define WDATAPROCESSING_H
#include <QObject>

//接受到的QByteArray的第一位为TOKEN，代表信息的格式与操作的种类
#define TOKEN_GUEST_LOGIN 1 //顾客登录
#define TOKEN_GUEST_REGISTER 2 //顾客注册
#define TOKEN_GUEST_GETUSERNAME 3 //获取顾客用户名
#define TOKEN_GUEST_GETMENU 4 //将菜单发送给顾客
#define TOKEN_GUEST_ORDERFOOD 5 //顾客点菜，接受顾客选的菜品清单
#define TOKEN_GUEST_CHOOSETABLE 6 //顾客选桌
#define TOKEN_GUEST_PAY 7 //顾客结账
#define TOKEN_GUEST_EVALUATE 8 //顾客评价
#define TOKEN_GUEST_SENDTABLE 9 //向顾客发送餐桌信息
#define TOKEN_GUEST_CHOOSESUCCESS 10 //顾客选桌成功
#define TOKEN_GUEST_CHOOSEFAILURE 11 //顾客选桌失败
#define TOKEN_GUEST_GETORDERLIST 12 //顾客获取已点菜品列表
#define TOKEN_GUEST_PAYCONFIRM 13 //顾客结账确认
#define TOKEN_GUEST_SINGLEMESSAGE 14 //顾客收到单条消息
#define TOKEN_GUEST_MSGTOWAITER 15 //顾客给服务员发一条消息

#define TOKEN_COOK_LOGIN 30 //厨师登录
#define TOKEN_COOK_REGISTER 31 //厨师注册
#define TOKEN_COOK_SELECTDISH 32 //厨师认领菜品
#define TOKEN_COOK_GETUNSTARTED 33 //厨师获取未开始的菜品
#define TOKEN_COOK_GETSELECTED 34 //厨师获取自己已认领的菜品
#define TOKEN_COOK_SETDISHSTATE 35 //厨师设置菜品状态
#define TOKEN_COOK_GETALLINFO 38 //获取厨师所需有信息

#define TOKEN_WAITER_LOGIN 40 //服务员登录
#define TOKEN_WAITER_REGISTER 41 //服务员注册
#define TOKEN_WAITER_CHOOSETABLE 42 //服务员选桌
#define TOKEN_WAITER_GETALLINFO 44 //发送服务员所有所需信息
#define TOKEN_WAITER_SINGLEMESSAGE 45 //服务员收到单条消息
#define TOKEN_WAITER_MSGTOGUEST 46 //服务员给顾客发一条消息

#define TOKEN_MANAGER_LOGIN 50 //经理登录
#define TOKEN_MANAGER_REGISTER 51 //经理注册

#define DISH_UNCHOSEN 101
#define DISH_CHOSEN_UNSTARTED 102
#define DISH_STARTED 103
#define DISH_FINISHED 104
#define DISH_ABORTED 105

struct SingleTable {
    //int index;
    QString type;
    int fitin;
    bool isValid;
    bool isSelected;
    int dishcount;
    bool isMine;
};

struct SingleDish {
    QString name;
    int index;
    int state;
    int tableindex;
};

/*
 * 数据处理类
 */
class wDataProcessing : public QObject
{
    Q_OBJECT

public:
    QList<SingleTable> tableInfo;                           //所有餐桌信息
    QList<SingleDish> dishInfo;                             //认领的餐桌中所有的菜品信息
    QList<QString> tmp_msg;                                 //储存消息的临时列表

    wDataProcessing();
    void ProcessingData(QByteArray);                        //处理收到的数据并发出相应信号

    QByteArray EncodingWaiterLogin(QString,QString);        //编码服务员登陆信息
    QByteArray EncodingWaiterRegister(QString,QString);     //编码服务员注册信息
    void DecodingAllInfo(QByteArray);                       //解码所有所需信息
    QString DecodingSingleMessage(QByteArray);              //解码单条消息

signals:
    void login_success(); //登录成功
    void login_usernameerror(); //登录失败，手机号不存在
    void login_passworderror(); //登录失败，密码错误
    void register_success(); //注册成功
    void register_usernameerror(); //注册失败，手机号已存在
    void waiter_getallinfo();
    void waiter_singlemessage(QString);
};

#endif // WDATAPROCESSING_H

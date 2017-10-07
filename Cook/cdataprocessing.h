/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：cdataprocessing.h
 * 摘要：厨师端数据处理模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef CDATAPROCESSING_H
#define CDATAPROCESSING_H
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
#define TOKEN_WAITER_CHOOSEFEEDBACK 43 //返回服务员选桌结果

#define TOKEN_MANAGER_LOGIN 50 //经理登录
#define TOKEN_MANAGER_REGISTER 51 //经理注册

#define DISH_UNCHOSEN 101
#define DISH_CHOSEN_UNSTARTED 102
#define DISH_STARTED 103
#define DISH_FINISHED 104
#define DISH_ABORTED 105

struct SingleDishOrdered { //被点的单个菜品信息
    QString name; //菜品名称
    int index; //每个菜品都有唯一的编号
    int state; //菜品状态，0为未开始，1为正在做，2为已完成
    int cookIndex; //认领该菜品的厨师
};

/*
 * 数据处理类
 */
class cDataProcessing : public QObject
{
    Q_OBJECT

private:
    QList<SingleDishOrdered> dishAvailable,dishSelected;    //储存可认领菜品、已认领菜品的信息

public:
    cDataProcessing();

    void ProcessingData(QByteArray);                        //处理收到的数据并发出相应信号
    QByteArray EncodingCookLogin(QString,QString);          //编码厨师登陆信息
    QByteArray EncodingCookRegister(QString,QString);       //编码厨师注册信息

signals:
    void login_success();                                   //登录成功
    void login_usernameerror();                             //登录失败，用户名不存在
    void login_passworderror();                             //登录失败，密码错误
    void register_success();                                //注册成功
    void register_usernameerror();                          //注册失败，用户名已存在
    void cook_getunstarted(QList<SingleDishOrdered>&);      //解码得到可认领菜品列表
    void cook_getselected(QList<SingleDishOrdered>&);       //解码得到已认领菜品列表
};

#endif // GDATAPROCESSING_H

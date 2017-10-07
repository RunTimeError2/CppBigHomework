/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：dataprocessing.h
 * 摘要：服务器端数据处理模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef DATEPROCESSING_H
#define DATEPROCESSING_H
#include <QObject>

/*
 * 接受到的QByteArray的第一位为TOKEN，代表信息的格式与操作的种类
 */
#define TOKEN_GUEST_LOGIN 1                 //顾客登录
#define TOKEN_GUEST_REGISTER 2              //顾客注册
#define TOKEN_GUEST_GETUSERNAME 3           //获取顾客用户名
#define TOKEN_GUEST_GETMENU 4               //将菜单发送给顾客
#define TOKEN_GUEST_ORDERFOOD 5             //顾客点菜，接受顾客选的菜品清单
#define TOKEN_GUEST_CHOOSETABLE 6           //顾客选桌
#define TOKEN_GUEST_PAY 7                   //顾客结账
#define TOKEN_GUEST_EVALUATE 8              //顾客评价
#define TOKEN_GUEST_SENDTABLE 9             //向顾客发送餐桌信息
#define TOKEN_GUEST_CHOOSESUCCESS 10        //顾客选桌成功
#define TOKEN_GUEST_CHOOSEFAILURE 11        //顾客选桌失败
#define TOKEN_GUEST_GETORDERLIST 12         //顾客获取已点菜品列表
#define TOKEN_GUEST_PAYCONFIRM 13           //顾客结账确认
#define TOKEN_GUEST_SINGLEMESSAGE 14        //顾客收到单条消息
#define TOKEN_GUEST_MSGTOWAITER 15          //顾客给服务员发一条消息
#define TOKEN_GUEST_MSGQUEUE 16             //获取消息队列
#define TOKEN_GUEST_EVALUATEWAITER 19
#define TOKEN_GUEST_CHANGEACCOUNT 20        //顾客修改账户信息

#define TOKEN_COOK_LOGIN 30                 //厨师登录
#define TOKEN_COOK_REGISTER 31              //厨师注册
#define TOKEN_COOK_SELECTDISH 32            //厨师认领菜品
#define TOKEN_COOK_GETUNSTARTED 33          //厨师获取未开始的菜品
#define TOKEN_COOK_GETSELECTED 34           //厨师获取自己已认领的菜品
#define TOKEN_COOK_SETDISHSTATE 35          //厨师设置菜品状态
#define TOKEN_COOK_GETALLINFO 38            //获取厨师所需有信息

#define TOKEN_WAITER_LOGIN 40               //服务员登录
#define TOKEN_WAITER_REGISTER 41            //服务员注册
#define TOKEN_WAITER_CHOOSETABLE 42         //服务员选桌
#define TOKEN_WAITER_GETALLINFO 44          //发送服务员所有所需信息
#define TOKEN_WAITER_SINGLEMESSAGE 45       //服务员收到单条消息
#define TOKEN_WAITER_MSGTOGUEST 46          //服务员给顾客发一条消息

#define TOKEN_MANAGER_LOGIN 50              //经理登录
#define TOKEN_MANAGER_REGISTER 51           //经理注册
#define TOKEN_MANAGER_REPORT 52             //经理接收数据报表

/*
 * 存储数据所需的基本数据结构
 */
namespace dataStructure {
    /*
     * 顾客点菜列表
     */
    class orderList {
    public:
        QString phonenumber;
        struct dish {
            QString name;
            int num;
        };
        dish *list;
        int count;
    };

    /*
     * 顾客评价列表
     */
    class evaluateList {
    public:
        QString phonenumber;
        struct evaluation {
            QString dishname;
            QString comment;
            int score;
            int e_index;                    //序号，仅用于编码发送给客户端使用，方便客户端删除评价
        };
        evaluation *list;
        int count;
    };

    struct tableinfo {
        int count;
        QString *typelist;
        int *numlist;                       //储存各桌人数
        int *valid;                         //是否空闲
    };
}

/*
 * 处理接受到的信息
 */
class dataProcessing:public QObject
{
    Q_OBJECT

public:
    void ProcessData(QByteArray);                       //输入收到的数据并解码

signals:
    void guest_login(QString,QString);                  //顾客登录
    void guest_register(QString,QString,QString);       //顾客注册
    void guest_getmenu();                               //顾客获取菜单信息
    void guest_orderfood(dataStructure::orderList);     //顾客发送点菜列表
    void guest_choosetable(int);                        //顾客选桌
    void guest_sendtable();                             //顾客获取餐桌信息
    void guest_evaluate(dataStructure::evaluateList);   //顾客发送评价信息
    void guest_getorderlist();                          //顾客获取已点菜品列表
    void guest_payconfirm();                            //顾客确认付款
    void guest_msgtowaiter(QString);                    //顾客给服务员发消息
    void guest_getmsgqueue();                           //顾客获取自己的消息队列
    void guest_evaluatewaiter(QString,QString,int);     //顾客发送对服务员的评价
    void guest_changeaccount(QString,QString,QString);  //顾客修改账号信息

    void cook_login(QString,QString);                   //厨师登录
    void cook_register(QString,QString);                //厨师注册
    void cook_selectdish(int);                          //厨师认领菜品
    void cook_getunstarted();                           //厨师获取可认领菜品列表
    void cook_getselected();                            //厨师获取自己已认领菜品列表
    void cook_setdishstate(int,int);                    //厨师设置菜品状态
    void cook_getallinfo();                             //厨师获取所有所需信息

    void waiter_login(QString,QString);                 //服务员登录
    void waiter_register(QString,QString);              //服务员注册
    void waiter_selecttable(int);                       //服务员认领餐桌
    void waiter_getallinfo();                           //服务员获取所有所需信息
    void waiter_msgtoguest(QString,int);                //服务员给顾客发消息

    void manager_login(QString,QString);                //经理登录
    void manager_register(QString,QString);             //经理注册
    void manager_report();                              //经理获取数据报表
};

#endif // DATEPROCESSING_H
    //void guest_getusername(); //顾客获取自身用户名
    //void guest_pay(float,QString); //顾客付款

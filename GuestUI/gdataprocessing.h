/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：gdataprocessing.h
 * 摘要：顾客端数据处理模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef GDATAPROCESSING_H
#define GDATAPROCESSING_H
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
#define TOKEN_GUEST_MSGQUEUE 16 //获取消息队列
#define TOKEN_GUEST_EVALUATEWAITER 19 //顾客评价服务员
#define TOKEN_GUEST_CHANGEACCOUNT 20 //顾客修改账户信息

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

class orderList {               //顾客点菜列表
public:
    QString phonenumber;
    struct dish {
        QString name;
        int num;
    };
    dish *list;
    int count;
};

struct singledish {             //单个菜品信息
    QString name;
    float price;
    int isvalid;
    int evalutionnum;
    float score;
    QString type;
};
struct menulist {               //菜单表
    int num;
    singledish *list;
};
struct tableinfo {              //餐桌信息表
    int count;
    QString *typelist;
    int *numlist;               //储存各桌人数
    int *valid;                 //是否空闲，1表示是
};
struct SingleDishOrdered {      //被点的单个菜品信息
    QString name;               //菜品名称
    int index;                  //每个菜品都有唯一的编号
    int state;                  //菜品状态
    int cookIndex;              //认领该菜品的厨师
};

/*
 * 数据处理类
 */
class gDataProcessing : public QObject
{
    Q_OBJECT

private:
    menulist Menu;                                              //存储菜单信息
    tableinfo Table;                                            //存储餐桌信息
    int myTable;                                                //自己选的餐桌编号
    float myPrice;                                              //自己所点菜品的总价
    QString my_username;
    QString waiter_name;                                        //认领自己的服务员用户名
    bool ischosen;                                              //是否已被服务员认领
    QList<SingleDishOrdered> dishorderlist;                     //已点菜品的列表

//获取私有成员的引用
public:
    menulist& getMenu() { return Menu; }
    tableinfo& getTable() { return Table; }
    int &getmyTable() { return myTable; }
    float &getmyPrice() { return myPrice; }
    QString &getmy_username() { return my_username; }
    QString &getwaiter_name() { return waiter_name; }
    bool &getischosen() { return ischosen; }
    QList<SingleDishOrdered> &getdishorderlist() { return dishorderlist; }

public:
    gDataProcessing();
    void ProcessingData(QByteArray);                            //处理收到的数据并发出相应信号

    menulist DecodingMenu(QByteArray);                          //将菜单信息解码
    void DecodingTableInfo(QByteArray);                         //将餐桌信息解码
    QByteArray EncodingGuestLogin(QString,QString);             //编码顾客登陆信息
    QByteArray EncodingGuestRegister(QString,QString,QString);  //编码顾客注册信息
    QByteArray EncodingGuestOrder(orderList);                   //编码顾客下单信息
    void DecodingOrderList(QByteArray);                         //解码点菜列表信息
    QString DecodingSingleMessage(QByteArray);                  //解码单条消息
    QList<QString> DecodingMsgQueue(QByteArray);                //解码消息队列
    void DecodingMenuTable(QByteArray);                         //解码菜单表

signals:
    void login_success();                                       //登录成功
    void login_phoneerror();                                    //登录失败，手机号不存在
    void login_passworderror();                                 //登录失败，密码错误
    void register_success();                                    //注册成功
    void register_phoneerror();                                 //注册失败，手机号已存在
    void getusername(QString);                                  //获取用户名
    void getmenu();                                             //获取菜单
    void gettableinfo();                                        //获取餐桌信息
    void order_done();                                          //点菜完成
    void choose_success();                                      //选桌成功
    void choose_fail();                                         //选桌失败
    void pay_done();                                            //完成结账
    void get_dishstate(QList<SingleDishOrdered>&);              //获取所有已点菜品状态
    void guest_singlemessage(QString);                          //收到一条消息
    void guest_msgqueue(QList<QString>);                        //收到消息队列
};

#endif // GDATAPROCESSING_H

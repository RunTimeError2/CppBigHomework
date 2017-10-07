/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainlogic.h
 * 摘要：服务器端主逻辑头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <QObject>
#include <QtSql>
#include "dataprocessing.h"
#include "login.h"

#define DISH_UNCHOSEN 101
#define DISH_CHOSEN_UNSTARTED 102
#define DISH_STARTED 103
#define DISH_FINISHED 104
#define DISH_ABORTED 105

//单条评价信息
struct SingleEvaluation {
    QString comment;                //评价
    int score;                      //评分
    QString dishname;               //菜品名称
    QString phonenumber;            //提供评价的顾客信息
};
//单个菜品信息
struct SingleDishInfo {
    QString name;                   //菜品名称
    float price;                    //单价
    int isValid;                    //是否有售
    QString type;                   //种类
    float averageScore;             //平均评分
    QList<int> evalutionList;       //评价信息列表，为节省内存仅存储评价信息的编号
};
//被点的单个菜品信息
struct SingleDishOrdered {
    QString name;                   //菜品名称
    int index;                      //每个菜品都有唯一的编号
    int state;                      //菜品状态，0为未开始，1为正在做，2为已完成
    int cookIndex;                  //认领该菜品的厨师
};
//餐桌状态
class TableState {
public:
    QString type;                   //餐桌种类
    int fitin;                      //容纳人数
    int GuestIndex;                 //使用该桌的顾客编号
    int WaiterIndex;                //负责该餐桌的服务员编号
    bool isValid;                   //是否空闲
    void pay();                     //顾客结账离开，清空所有餐桌信息
};
//单条消息
struct SingleMessage {
    QString msg;                    //消息的内容
    int type;                       //1：发送给顾客，2：发送给服务员
    QString source;                 //发送消息的用户手机号/服务员用户名
    int sourceTable;                //发送消息的餐桌
};
//单个顾客信息
class SingleGuest {
public:
    bool isOnline;                  //是否登录
    QString phonenumber;            //手机号
    QString username;               //用户名
    QString password;               //密码
    int chooseTable;                //选的餐桌编号
    bool isPaid;                    //是否已结账
    float price;                    //所点菜品的总价
    bool isOrdered;                 //是否已下单
    int waiterIndex;                //对应的服务员编号
    QList<int> evalutionList;       //评价信息列表，为节省内存仅存储评价信息的编号
    QList<int> orderList;           //已点菜品的编号
    QList<SingleMessage> msg;       //接收到的消息队列
    //点菜列表存储在餐桌信息中
};
//单条服务员评价信息
struct waiterEvaluation {
    QString username;               //用户名
    QString comment;                //评价内容
    int score;                      //评分
};
//单个服务员信息
class SingleWaiter {
public:
    bool isOnline;                  //是否登录
    int record;                     //工作记录，认领的餐桌总数
    int tmp_record;                 //从服务器启动到现在的工作记录
    QString username;               //用户名
    QString password;               //密码
    QList<int> chooseGuestTable;    //认领的餐桌编号
    QList<SingleMessage> msg;       //接收到的消息队列
};
//单个厨师信息
class SingleCook {
public:
    bool isOnline;                  //是否登录
    int record;                     //工作记录，完成的菜品总数
    int tmp_record;                 //从服务器启动到现在的工作记录
    QString username;               //用户名
    QString password;               //密码
    QList<int> selectedDish;        //认领的菜品编号
};
//单个经理信息
class SingleManager {
public:
    bool isOnline;                  //是否在线
    QString username;               //用户名
    QString password;               //密码
};

/*******************************
 * 整个服务器端的主数据结构，存储所有所需信息
 *******************************/
class mainLogic : public QObject
{
    Q_OBJECT

public:
    /*
     * 数据结构部分
     */
    QList<SingleGuest*> guests;                     //顾客列表
    QMap<QString,int> guestMap;                     //手机号和顾客编号（数组下标）一一对应

    QList<SingleCook*> cooks;                       //厨师列表
    QMap<QString,int> cookMap;                      //用户名和厨师编号（数组下标）一一对应

    QList<SingleWaiter*> waiters;                   //服务员列表
    QMap<QString,int> waiterMap;                    //用户名和服务员编号（数组下标）一一对应

    QList<SingleManager*> managers;                 //经理列表
    QMap<QString,int> managerMap;                   //用户名和经理编号（数组下标）一一对应

    QList<TableState*> tables;                      //餐桌列表

    QList<SingleDishInfo> menu;                     //菜单
    QMap<QString,int> menuMap;                      //菜品名称和编号（数组下标）一一对应

    QList<SingleEvaluation> allEvaluation;          //所有评价

    QList<SingleDishOrdered> orderlist;             //当前点菜列表

    QList<waiterEvaluation> waiterEvaluationList;   //服务员评价列表

public:
    mainLogic();
    void ReadAllData();                                     //从数据库读取所有信息存入主数据结构

    void updateEvaluationInfo();                            //统计所有菜品评价信息，计算评价数量和平均分数
    void orderlist_updated();                               //更新点菜信息列表

    /*
     * 顾客部分
     */
    void guest_register(QString,QString,QString);           //顾客注册
    int guest_login(QString,QString);                       //顾客登录
    void guest_logout(QString);                             //顾客退出
    void addsingleevaluation(QString,QString,QString,int);  //添加单条评价信息
    void guest_evaluate(dataStructure::evaluateList);       //顾客评价
    void addsingleorderinfo(QString,QString);               //添加单条点菜信息
    void guest_order(dataStructure::orderList);             //顾客点菜
    bool guest_choosetable(int,QString);                    //顾客选桌
    void guest_quittable(int);                              //顾客离开餐桌
    void guest_pay(QString);                                //顾客付款
    QByteArray guest_Encodingtableinfo(QString);            //编码所有餐桌信息
    QByteArray guest_EncodingOrderlist(QString);            //编码点菜列表信息
    void guest_msgtowaiter(QString,QString);                //给服务员发送一条消息
    QByteArray guest_EncodingMsgQueue(QString);             //编码顾客的消息队列
    void guest_evaluateWaiter(QString,QString,int);         //顾客评价服务员
    void guest_ChangeAccount(QString,QString,QString);      //顾客修改账户信息

    /*
     * 服务员部分
     */
    void waiter_register(QString,QString);                  //服务员注册
    int waiter_login(QString,QString);                      //服务员登录
    void waiter_logout(QString);                            //服务员退出
    bool waiter_choosetable(int,QString);                   //服务员认领餐桌
    QByteArray EncodingWaiterInfo(QString);                 //编码服务员信息
    void waiter_msgtoguest(QString,int,QString);            //服务员给顾客发消息

    /*
     * 厨师部分
     */
    void cook_register(QString,QString);                    //厨师注册
    int cook_login(QString,QString);                        //厨师登录
    void cook_logout(QString);                              //厨师退出
    bool cook_choosedish(int,QString);                      //厨师认领菜品
    bool cook_setdishstate(int,int,QString);                //厨师设置菜品状态
    QByteArray cook_getchosendish(QString);                 //编码厨师已认领菜品列表
    QByteArray cook_getunstarteddish();                     //编码厨师可认领菜品列表
    QByteArray cook_encodingAll(QString);                   //编码厨师所需所有信息

    /*
     * 经理部分
     */
    void manager_register(QString,QString);                 //经理注册
    int manager_login(QString,QString);                     //经理登录
    void manager_logout(QString);                           //经理退出
    QByteArray manager_generateReport();                    //编码经理所需报表

signals:
    void updateAllOrderInfo();
    void updateAllOrderInfoExceptMe(QString);
            //在部分改变主数据结构中数据的操作后（比如注册、点菜），将相应信息发送给各种客户端
            //从而实现实时更新数据，也方便多用户系统的运行

    void MessagetoGuest(QByteArray,QString);                //让网络模块给相应顾客发消息
    void MessagetoWaiter(QByteArray,QString);               //让网络模块给相应服务员发消息
};

#endif // MAINLOGIC_H
    //int guest_canceldish(int,QString); //顾客

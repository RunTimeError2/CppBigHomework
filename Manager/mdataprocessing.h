/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mdataprocessing.h
 * 摘要：经理端数据处理模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MDATAPROCESSING_H
#define MDATAPROCESSING_H
#include <QObject>
#include <QMap>

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

struct WorkRecord { //员工工作记录
    QString username;
    int tmp_record;
    int record;

    //仅限服务员
    int evaluationCount;
    int scoreSum;
    float avaerageScore;
};
//单条服务员评价信息
struct SingleWaiterEvaluation {
    QString username;
    QString comment;
    int score;
};
//单条菜品评价信息
struct SingleDishEvaluation {
    QString phonenumber;
    QString dishname;
    QString comment;
    int score;
};
//单条交易记录
struct PayRecord {
    QString phonenumber;
    QString datetime;
    QString price;
};

class managerReport { //经理获取的数据报表
public:
    QList<WorkRecord> cook;
    QMap<QString,int> cookMap;
    QList<WorkRecord> waiter;
    QMap<QString,int> waiterMap;
    QList<SingleWaiterEvaluation> waiterEvaluation;
    QList<SingleDishEvaluation> dishEvaluation;
    QList<PayRecord> payRecordList;

    managerReport() {
        clearAll();
    }
    ~managerReport() {
        clearAll();
    }
    void clearAll() {
        cook.clear();
        waiter.clear();
        waiterEvaluation.clear();
        cookMap.clear();
        waiterMap.clear();
        dishEvaluation.clear();
        payRecordList.clear();
    }
};

/*
 * 数据处理类
 */
class mDataProcessing : public QObject
{
    Q_OBJECT

public:
    mDataProcessing();
    void ProcessingData(QByteArray);                        //处理收到的数据并发出相应信号

    QByteArray EncodingManagerLogin(QString,QString);       //编码经理登陆信息
    QByteArray EncodingManagerRegister(QString,QString);    //编码经理注册信息
    void DecodingManagerReport(QByteArray);                 //解码收到的报表信息

    managerReport *report;

signals:
    void login_success();                                   //登录成功
    void login_usernameerror();                             //登录失败，手机号不存在
    void login_passworderror();                             //登录失败，密码错误
    void register_success();                                //注册成功
    void register_usernameerror();                          //注册失败，手机号已存在
    void manager_getreport();                               //获得报表
};

#endif // MDATAPROCESSING_H

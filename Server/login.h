/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：login.h
 * 摘要：服务器端数据库模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <QtSql>
#include "dataprocessing.h"

/*
 * 处理登录和注册操作，以及有关数据库的信息编码
 */

class Login
{
private:
    QSqlDatabase db;                                            //负责连接数据库的类
    bool dbOpened;                                              //表示数据库是否成功打开

public:
    Login();                                                    //在构造函数中会自动调用setupDataBase()
    ~Login();
    bool setupDataBase();                                       //建立与数据库的连接，并初始化数据库
                                                                //返回值表示是否成功打开数据库

    int guestLogin(QString,QString);                            //检查客户输入的手机号与密码是否正确
    QString guestGetUsername(QString,QString);                  //如果登录成功，根据手机号和密码获取用户名
    int guestRegister(QString,QString,QString);                 //客户注册
    void updateEvaluationTable(dataStructure::evaluateList);    //根据顾客评价信息更新评价表
    void changeGuestAccount(QString,QString,QString);           //顾客修改账号
    void deleteSingleEvaluation(int);                           //根据序号删除评价
    void guestpay(float,QString);                               //顾客结账，加入交易记录
    QByteArray EncodingMenu();                                  //从数据库读取菜单并编码
    void evaluateWaiter(QString,QString,int);                   //将评价服务员的信息加入数据库

    int cookRegister(QString,QString);                          //厨师注册，将账号信息写入数据库

    int waiterRegister(QString,QString);                        //服务员注册，将账号信息写入数据库
    void addwaiterRecord(QString);                              //加入一条服务员工作记录

    int managerRegister(QString,QString);                       //经理注册，将账号信息写入数据库

    void addcookRecord(QString);                                //加入一条厨师工作记录

    void updateEvaluationNumber();                              //根据"评价信息"表，更新"菜单"表中的"评价
                                                                //数量"列和"平均评价分数"列，"客户账户"中
                                                                //的"评价数量"列，防止误改
    QByteArray EncodingPayRecord(); //读取交易记录并编码
};
#endif // LOGIN_H

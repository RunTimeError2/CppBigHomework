/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：admin.h
 * 摘要：管理员端数据库模块头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef ADMIN_H
#define ADMIN_H

#include <QtSql>
#include <QTableView>

class admin {
public:
    QSqlDatabase db;                                //数据库类，实现数据库的打开和关闭
    bool dbOpened;                                  //表示数据库是否成功打开

    admin();
    ~admin();
    bool setupDataBase();                           //创建与数据库的连接，返回值表示操作是否成功
    bool setupDBTables();                           //更新表格，如果表不存在则创建表，返回值表示
                                                        //操作是否成功
    void addAdminAccount();                         //加入管理员账号默认信息，默认用户名admin，
                                                        //密码admin，仅在创建数据库时时使用

    void viewInTableView(QString,QTableView*);      //将指定表格的内容在指定QTableView中显示

    void addMenu();                                 //向“菜单”表中插入一项
    void updateMenuSelectedRow(int,QTableView*);    //将“菜单”表对应的QTableView中的某一行数据
                                                        //写入数据库
    void updateMenuAll(QTableView*);                //将“菜单”表对应的所有信息写入数据库
    void deleteMenuSelectedRow(QTableView*);        //删除“菜单”表中的选中行

    void addGuest();                                //向“顾客账号信息”表中插入一项
    void deleteGuestSelectedRow(QTableView*);       //删除“顾客账号信息”表中的选中行

    void addOther(QTableView*);                     //向“其他账号信息”表中添加信息
    void deleteOtherSelectedRow(QTableView*);       //删除“其他”表中的选中行
    void updateOtherAll(QTableView*);               //将“其他账号信息”表对应的所有信息写入数据库
    void updateOtherSelectedRow(int,QTableView*);   //将“其他账号信息”表对应的QTableView中的某
                                                        //一行数据写入数据库

    void updateEvaluationNumber();                  //根据"评价信息"表，更新"菜单"表中的"评价数量"
                                                        //列和"平均评价分数"列，"客户账户"中的"评
                                                        //价数量"列，防止误改

    void addTable();                                //添加餐桌信息
    void deleteTableSelectedRow(QTableView*);       //删除餐桌信息
};

#endif // ADMIN_H

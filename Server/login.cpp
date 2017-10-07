/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：login.cpp
 * 摘要：服务器端数据库模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "login.h"
#include "dataprocessing.h"
#include <QMessageBox>
#include <QDateTime>

Login::Login() {
    setupDataBase();
}

Login::~Login() {
    db.close();
}

/*
 * 建立与数据库文件间的连接
 */
bool Login::setupDataBase() {
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("restaurantDB.db");
    return (dbOpened=db.open());
}

/*
 * 如果登录成功，根据手机号和密码获取用户名
 */
QString Login::guestGetUsername(QString phonenumber,QString password) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 顾客账号信息 WHERE 手机号='%1' AND 密码='%2'").arg(phonenumber).arg(password));
    if(query.next())
        return query.value(1).toString();
    else {
        throw QString("无法找到该用户");
        return "";
    }
}

/*
 * 客户注册
 * 密码验证、手机号有效性验证均在客户端完成
 */
int Login::guestRegister(QString phonenumber,QString username,QString password) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 顾客账号信息 WHERE 手机号='%1'").arg(phonenumber)); //查找用户是否已存在
    if(!query.next()) {
        query.exec(QString("INSERT INTO 顾客账号信息 VALUES('%1','%2','%3',0)").arg(phonenumber).arg(username).arg(password));
        return 1; //注册成功
    }
    else
        return 0; //注册失败，用户已存在
}

/*
 * 将菜单编码以便发送
 */
QByteArray Login::EncodingMenu() {
    QSqlQuery query;
    query.exec("SELECT * FROM 菜单");
    int count=0;
    if(query.next()) {
        query.last();
        count=query.at()+1;
    }
    if(count==0)
        throw QString("菜单为空！");
    query.exec("SELECT * FROM 菜单");
    QByteArray ans;
    ans.append(TOKEN_GUEST_GETMENU);
    ans.append(count/256);
    ans.append(count%256); //用两位表示菜品数量
    while(query.next()) {
        QString s_tmp=query.value(0).toString(); //名称
        QByteArray b_tmp=s_tmp.toUtf8();
        ans.append((unsigned char)b_tmp.length());
        ans.append(b_tmp);
        int pricetmp=(int)(query.value(1).toFloat()*100); //价格，精确到2位小数
        ans.append((unsigned char)(pricetmp/256));
        ans.append((unsigned char)(pricetmp%256));
        ans.append((unsigned char)query.value(2).toInt()); //是否有售，用0或1表示
        int comments=query.value(3).toInt(); //评价数量
        ans.append((unsigned char)comments/256);
        ans.append((unsigned char)comments%256);
        int score=(int)(query.value(4).toFloat()*10); //评价评分,0~5分，精确到1位小数
        ans.append((unsigned char)score);
        QByteArray type_tmp=query.value(5).toString().toUtf8();
        ans.append((unsigned char)type_tmp.length());
        ans.append(type_tmp);
    }
    return ans;
}

/*
 * 厨师注册
 */
int Login::cookRegister(QString username,QString password) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 其他账号信息 WHERE 种类='厨师' AND 用户名='%1'").arg(username));
    if(!query.next()) {
        query.exec("SELECT * FROM 其他账号信息");
        int count=0;
        if(query.next()) {
            query.last();
            count=query.at()+1;
        }
        query.exec(QString("INSERT INTO 其他账号信息 VALUES('%1','厨师','%2','%3')").arg(count+1).arg(username).arg(password));
        return 1;
    }
    else
        return 0; //用户已存在
}

/*
 * 服务员注册
 */
int Login::waiterRegister(QString username,QString password) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 其他账号信息 WHERE 种类='服务员' AND 用户名='%1'").arg(username));
    if(!query.next()) {
        query.exec("SELECT * FROM 其他账号信息");
        int count=0;
        if(query.next()) {
            query.last();
            count=query.at()+1;
        }

        query.exec(QString("INSERT INTO 其他账号信息 VALUES('%1','服务员','%2','%3')").arg(count+1).arg(username).arg(password));
        return 1;
    }
    else
        return 0; //用户已存在
}

/*
 * 经理注册
 */
int Login::managerRegister(QString username,QString password) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 其他账号信息 WHERE 种类='经理' AND 用户名='%1'").arg(username));
    if(!query.next()) {
        query.exec("SELECT * FROM 其他账号信息");
        int count=0;
        if(query.next()) {
            query.last();
            count=query.at()+1;
        }
        query.exec(QString("INSERT INTO 其他账号信息 VALUES('%1','经理','%2','%3')").arg(count+1).arg(username).arg(password));
        return 1;
    }
    else
        return 0; //用户已存在
}

/*
 * 根据顾客评价信息更新评价表
 */
void Login::updateEvaluationTable(dataStructure::evaluateList list) {
    QSqlQuery query;
    query.exec("SELECT * FROM 评价信息");
    int count=0;
    if(query.next()) {
        query.last();
        count=query.at()+1;
    }
    for(int i=0;i<list.count;i++)
        query.exec(QString("INSERT INTO 评价信息 VALUES(%1,'%2','%3','%4',%5)").arg(++count).arg(list.list[i].dishname).arg(list.phonenumber).arg(list.list[i].comment).arg(list.list[i].score));
}

/*
 * 更改顾客账户信息
 */
void Login::changeGuestAccount(QString phonenumber,QString username,QString password) {
    QSqlQuery query;
    query.exec(QString("UPDATE 顾客账号信息 SET 用户名='%1', 密码='%2' WHERE 手机号='%3'").arg(username).arg(password).arg(phonenumber));
}

/*
 * 评价服务员
 */
void Login::evaluateWaiter(QString username,QString comment,int score) {
    QSqlQuery query;
    query.exec("SELECT * FROM 评价服务员");
    int count=0;
    if(query.next()) {
        query.last();
        count=query.at()+1;
    }
    query.exec(QString("INSERT INTO 评价服务员 VALUES(%1,'%2','%3',%4)").arg(count).arg(username).arg(comment).arg(score));
}

/*
 * 根据序号删除评价
 */
void Login::deleteSingleEvaluation(int e_index) {
    QSqlQuery query;
    query.exec("SELECT * FROM 评价信息");
    if(!query.next())
        return;
    query.last();
    int count=query.at()+1;
    if(count<e_index)
        return;
    query.exec(QString("DELETE FROM 评价信息 WHERE 序号=%1").arg(e_index));
    for(int i=e_index+1;i<=count;i++)
        query.exec(QString("UPDATE 评价信息 SET 序号=%1 WHERE 序号=%2").arg(i-1).arg(i));
}

/*
 * 顾客结账，加入交易记录
 */
void Login::guestpay(float price,QString phonenumber) {
    QSqlQuery query;
    query.exec("SELECT * FROM 交易记录");
    int count=0;
    if(query.next()) {
        query.last();
        count=query.at()+1;
    }
    QDateTime currenttime=QDateTime::currentDateTime();
    QString s_currenttime=currenttime.toString("yyyy-MM-dd hh:mm:dd ddd");
    query.exec(QString("INSERT INTO 交易记录 VALUES(%1,'%2','%3',%4)").arg(count+1).arg(s_currenttime).arg(phonenumber).arg(price));
}

/*
 * 加入一次服务员工作记录
 */
void Login::addwaiterRecord(QString username) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 服务员工作记录 WHERE 用户名='%1'").arg(username));
    if(query.next()) {
        int count=query.value(1).toInt();
        query.exec(QString("UPDATE 服务员工作记录 SET 记录=%1 WHERE 用户名='%2'").arg(count+1).arg(username));
    }
    else {
        query.exec(QString("INSERT INTO 服务员工作记录 VALUES('%1',1)").arg(username));
    }
}

/*
 * 加入一次厨师工作记录
 */
void Login::addcookRecord(QString username) {
    QSqlQuery query;
    query.exec(QString("SELECT * FROM 厨师工作记录 WHERE 用户名='%1'").arg(username));
    if(query.next()) {
        int count=query.value(1).toInt();
        query.exec(QString("UPDATE 厨师工作记录 SET 记录=%1 WHERE 用户名='%2'").arg(count+1).arg(username));
    }
    else
        query.exec(QString("INSERT INTO 厨师工作记录 VALUES('%1',1)").arg(username));
}

/*
 * 根据"评价信息"表，更新"菜单"表中的"评价数量"列和"平均评价分数"列，"客户账户"中的"评价数量"列，防止误改
 */
void Login::updateEvaluationNumber() {
    QSqlQuery query;
    query.exec("SELECT * FROM 菜单");
    QMap<QString,int> mapMenu_num;
    QMap<QString,float> mapMenu_score;
    while(query.next()) {
        mapMenu_num.insert(query.value(0).toString(),0);
        mapMenu_score.insert(query.value(0).toString(),0.0);
    }
    query.exec("SELECGT * FROM 顾客账号信息");
    QMap<QString,int> mapGuest_num;
    while(query.next())
        mapGuest_num.insert(query.value(0).toString(),0);
    query.exec("SELECT * FROM 评价信息");
    QString tmp_name,tmp_phone;
    float tmp_score;
    while(query.next()) {
        tmp_name=query.value(1).toString();
        tmp_phone=query.value(2).toString();
        tmp_score=query.value(4).toFloat();
        mapMenu_num[tmp_name]++;
        mapMenu_score[tmp_name]+=tmp_score;
        mapGuest_num[tmp_phone]++;
    }
    query.exec("SELECT * FROM 菜单");
    QSqlQuery Qupdate;
    while(query.next()) {
        QString name=query.value(0).toString();
        int num=mapMenu_num[name];
        float score=mapMenu_score[name];
        float average=(num>0)?(score/(float)num):0;
        Qupdate.exec(QString("UPDATE 菜单 SET 评价数量=%1, 平均评价分数=%2 WHERE 名称='%3'")
                     .arg(num).arg(average).arg(name));
    }
    query.exec("SELECT * FROM 顾客账号信息");
    while(query.next()) {
        QString phone=query.value(0).toString();
        int num=mapGuest_num[phone];
        Qupdate.exec(QString("UPDATE 顾客账号信息 SET 评价数量=%1 WHERE 手机号='%2'").arg(num).arg(phone));
    }
}

/*
 * 将交易记录编码
 */
QByteArray Login::EncodingPayRecord() {
    QSqlQuery query;
    query.exec("SELECT * FROM 交易记录");
    QByteArray ans;
    ans.append(1);
    ans.append(1);
    int count=0;
    while(query.next()) {
        QByteArray tmp;
        tmp=query.value(1).toString().toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        tmp=query.value(2).toString().toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        tmp=query.value(3).toString().toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        count++;
    }
    ans[0]=(unsigned char)(count/256);
    ans[1]=(unsigned char)(count%256);
    if(count==0)
        throw QString("无交易记录");
    return ans;
}

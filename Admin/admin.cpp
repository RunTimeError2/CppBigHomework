/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：admin.cpp
 * 摘要：管理员端数据库模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "admin.h"
#include <QMessageBox>
#include "combodelegate.h"
#include "spindelegate.h"

admin::admin() {
    dbOpened=false;
}

admin::~admin() {
    db.close();
    dbOpened=false;
}

/*
 * 创建与数据库的连接，返回值表示操作是否成功
 */
bool admin::setupDataBase() {
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("MainComputer");
    db.setDatabaseName("restaurantDB.db");
    db.setUserName("Administrator");
    db.setPassword("admin");
    return (dbOpened=db.open());
}

/*
 * 初始化数据库，如果表不存在则创建表，返回值表示操作是否成功
 */
bool admin::setupDBTables() {
    QSqlQuery query;
    bool isTableExist;
    query.exec("select count(*) from sqlite_master where type='table' and name='菜单'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_menu=(!isTableExist)?(query.exec("CREATE TABLE 菜单"
                                                 "(名称 TEXT PRIMARY KEY NOT NULL,"
                                                 "价格 REAL NOT NULL,"
                                                 "是否有售 INTEGER,"
                                                 "评价数量 INTEGER,"
                                                 "平均评价分数 REAL,"
                                                 "类别 TEXT)"))
                                   :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='评价信息'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_evaluation=(!isTableExist)?(query.exec("CREATE TABLE 评价信息"
                                                       "(序号 INTEGER PRIMARY KEY NOT NULL,"
                                                       "菜品名称 TEXT,"
                                                       "顾客手机号 TEXT NOT NULL,"
                                                       "顾客评价 TEXT,"
                                                       "评分 INTEGER)"))
                                         :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='顾客账号信息'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_guestaccount=(!isTableExist)?(query.exec("CREATE TABLE 顾客账号信息"
                                                         "(手机号 TEXT PRIMARY KEY NOT NULL,"
                                                         "用户名 TEXT NOT NULL,"
                                                         "密码 TEXT NOT NULL,"
                                                         "评价数量 INTEGER)"))
                                           :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='其他账号信息'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_otheraccount=(!isTableExist)?(query.exec("CREATE TABLE 其他账号信息"
                                                         "(序号 TEXT PRIMARY KEY NOT NULL,"
                                                         "种类 TEXT NOT NULL,"
                                                         "用户名 TEXT NOT NULL,"
                                                         "密码 TEXT NOT NULL)"))
                                           :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='管理员账户'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_admin=(!isTableExist)?(query.exec("CREATE TABLE 管理员账户"
                                                   "(账户名 TEXT PRIMARY KEY NOT NULL,"
                                                   "密码 TEXT NOT NULL)"))
                                     :true;
    if(!isTableExist)
        addAdminAccount();
    query.exec("select count(*) from sqlite_master where type='table' and name='交易记录'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_trade=(!isTableExist)?(query.exec("CREATE TABLE 交易记录"
                                                   "(序号 INTEGER PRIMARY KEY NOT NULL,"
                                                   "时间 TEXT,"
                                                   "顾客手机号 TEXT,"
                                                   "总金额 FLOAT)"))
                                     :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='餐桌信息'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_table=(!isTableExist)?(query.exec("CREATE TABLE 餐桌信息"
                                                   "(种类 TEXT PRIMARY KEY NOT NULL,"
                                                   "人数 INTEGER NOT NULL,"
                                                   "数量 INTEGER NOT NULL)"))
                                     :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='评价服务员'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_waiter=(!isTableExist)?(query.exec("CREATE TABLE 评价服务员"
                                                    "(编号 INTEGER PRIMARY KEY NOT NULL,"
                                                    "服务员用户名 TEXT NOT NULL,"
                                                    "评价 TEXT,"
                                                    "评分 INTEGER)"))
                                      :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='厨师工作记录'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_cookwork=(!isTableExist)?(query.exec("CREATE TABLE 厨师工作记录"
                                                      "(用户名 TEXT PRIMARY KEY NOT NULL,"
                                                      "记录 INTEGER NOT NULL)"))
                                        :true;
    query.exec("select count(*) from sqlite_master where type='table' and name='服务员工作记录'");
    isTableExist=(query.next())?(query.value(0).toInt()!=0):false;
    bool success_waiterwork=(!isTableExist)?(query.exec("CREATE TABLE 服务员工作记录"
                                                      "(用户名 TEXT PRIMARY KEY NOT NULL,"
                                                      "记录 INTEGER NOT NULL)"))
                                        :true;

    bool success=(success_menu && success_evaluation && success_guestaccount && success_otheraccount && success_admin && success_trade && success_table
                  && success_waiter && success_cookwork && success_waiterwork);
    dbOpened=dbOpened && success;

    return success;
}

/*
 * 加入管理员账号默认信息，默认用户名admin，密码admin，仅在创建数据库时时使用
 */
void admin::addAdminAccount() {
    QSqlQuery query;
    if(dbOpened) {
        query.exec("INSERT INTO 管理员账户 VALUES('admin','admin')");
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 向“菜单”表中插入一项
 */
void admin::addMenu() {
    QSqlQuery query;
    if(dbOpened) //如果能成功打开数据库与表
        query.exec(QString("INSERT INTO 菜单 VALUES('<未命名>',0,1,0,0,'<未设置>')"));
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 将指定表格的内容在指定QTableView中显示
 */
void admin::viewInTableView(QString tablename,QTableView *widget) {
    if(dbOpened) {
        QSqlQuery query("SELECT tbl_name FROM sqlite_master");
        QString name="";
        while(query.next() && name!=tablename)
            name=query.value(0).toString();
        if(name==tablename) {
            QSqlRelationalTableModel *model=new QSqlRelationalTableModel(NULL,db);
            model->setEditStrategy(QSqlTableModel::OnFieldChange);
            model->setTable(tablename);

            //为一些特殊的表项添加ComboBox,SpinBox代理
            if(tablename=="其他账号信息") { //"其他账号信息"表中的"种类"
                ComboDelegate *combos=new ComboDelegate(widget,1);
                widget->setItemDelegateForColumn(1,combos);
                SpinDelegate *spindele=new SpinDelegate(widget,2);
                widget->setItemDelegateForColumn(0,spindele);
            }
            if(tablename=="菜单") { //"菜单"表中的"种类"
                ComboDelegate *combos2=new ComboDelegate(widget,2);
                widget->setItemDelegateForColumn(5,combos2);
                SpinDelegate *spindele2=new SpinDelegate(widget);
                widget->setItemDelegateForColumn(3,spindele2);
                SpinDelegate *spindele21=new SpinDelegate(widget,1);
                widget->setItemDelegateForColumn(2,spindele21);
                SpinDelegate *spindele=new SpinDelegate(widget,2);
                widget->setItemDelegateForColumn(3,spindele);
                widget->setItemDelegateForColumn(4,spindele);
            }
            if(tablename=="餐桌信息") { //"餐桌信息"表中的"人数"和"餐桌数量"
                SpinDelegate *spindelegate=new SpinDelegate(widget);
                widget->setItemDelegateForColumn(1,spindelegate);
                widget->setItemDelegateForColumn(2,spindelegate);
            }
            if(tablename=="顾客账号信息") {
                SpinDelegate *spindele=new SpinDelegate(widget,2);
                widget->setItemDelegateForColumn(3,spindele);
            }
            model->select();
            widget->setModel(model);
        }
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 将“菜单”表对应的QTableView中的某一行数据写入数据库
 */
void admin::updateMenuSelectedRow(int row,QTableView *widget) {
    QString data[5];
    for(int i=0;i<5;i++)
        data[i]=widget->model()->index(row,i).data().toString();
    if(dbOpened) {
        QSqlQuery query;
        query.exec(QString("UPDATE 菜单 "
                           "SET 名称='%1', 价格=%2, 是否有售=%3, 评价数量=%4, 平均评价分数=%5 "
                           "WHERE 名称='%6'").arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]).arg(data[4]).arg(data[0]));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 删除“菜单”表中的选中行
 */
void admin::deleteMenuSelectedRow(QTableView *widget) {
    int row=widget->currentIndex().row();
    if(row<0 || row>=widget->model()->rowCount()) {
        QMessageBox::critical(NULL,"Error","必须选择一行才能删除！");
        return;
    }
    QString name=widget->model()->index(row,0).data().toString();
    if(dbOpened) {
        QSqlQuery query;
        query.exec(QString("DELETE FROM 菜单 WHERE 名称='%1'").arg(name));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 将“菜单”表对应的所有信息写入数据库
 */
void admin::updateMenuAll(QTableView *widget) {
    if(dbOpened) {
        QSqlQuery query;

        query.exec("select * from 菜单");
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(query);
        int nRecordCount = model->rowCount();

        for(int i=0;i<nRecordCount;i++)
            updateMenuSelectedRow(i,widget);
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 向“顾客账号信息”表中插入一项
 */
void admin::addGuest() {
    QSqlQuery query;
    if(dbOpened)
        query.exec(QString("INSERT INTO 顾客账号信息 VALUES('<未设置>','<未命名>','<未设置>',0)"));
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 删除“顾客账号信息”表中的选中行
 */
void admin::deleteGuestSelectedRow(QTableView *widget) {
    int row=widget->currentIndex().row();
    if(row<0 || row>=widget->model()->rowCount()) {
        QMessageBox::critical(NULL,"Error","必须选择一行才能删除！");
        return;
    }
    QString number=widget->model()->index(row,0).data().toString();
    if(dbOpened) {
        QSqlQuery query;
        query.exec(QString("DELETE FROM 顾客账号信息 WHERE 手机号='%1'").arg(number));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 添加一条餐桌信息
 */
void admin::addTable() {
    QSqlQuery query;
    if(dbOpened)
        query.exec(QString("INSERT INTO 餐桌信息 VALUES('<未设置>',0,0)"));
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 删除一条餐桌信息
 */
void admin::deleteTableSelectedRow(QTableView *widget) {
    int row=widget->currentIndex().row();
    if(row<0 || row>=widget->model()->rowCount()) {
        QMessageBox::critical(NULL,"Error","必须选择一行才能删除！");
        return;
    }
    QString type=widget->model()->index(row,0).data().toString();
    if(dbOpened) {
        QSqlQuery query;
        query.exec(QString("DELETE FROM 餐桌信息 WHERE 种类='%1'").arg(type));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 向“其他账号信息”表中添加信息
 */
void admin::addOther(QTableView *widget) {
    QSqlQuery query;
    if(dbOpened) {
        int row=widget->model()->rowCount()+1;
        query.exec(QString("INSERT INTO 其他账号信息 VALUES('%1','<未设置>','<未设置>','<未设置>')").arg(row));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 删除“其他”表中的选中行
 */
void admin::deleteOtherSelectedRow(QTableView *widget) {
    int row=widget->currentIndex().row();
    if(row<0 || row>=widget->model()->rowCount()) {
        QMessageBox::critical(NULL,"Error","必须选择一行才能删除！");
        return;
    }
    QString index=widget->model()->index(row,0).data().toString();
    if(dbOpened) {
        QSqlQuery query;
        query.exec(QString("DELETE FROM 其他账号信息 WHERE 序号='%1'").arg(index));
        //更新序号
        int row=widget->model()->rowCount()-1;
        QString *type=new QString[row];
        QString *username=new QString[row];
        int len=0;
        query.exec("SELECT * FROM 其他账号信息");
        while(query.next()) {
            type[len]=query.value(1).toString();
            username[len]=query.value(2).toString();
            ++len;
        }
        for(int i=0;i<row;i++)
            query.exec(QString("UPDATE 其他账号信息 SET 序号='%1' WHERE 种类='%2' AND 用户名='%3'").arg(i+1).arg(type[i]).arg(username[i]));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 将“其他账号信息”表对应的所有信息写入数据库
 */
void admin::updateOtherAll(QTableView *widget) {
    if(dbOpened) {
        QSqlQuery query;
        query.exec("select * from 其他账号信息");
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(query);
        int nRecordCount = model->rowCount();
        for(int i=0;i<nRecordCount;i++)
            updateOtherSelectedRow(i,widget);
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 将“其他账号信息”表对应的QTableView中的某一行数据写入数据库
 */
void admin::updateOtherSelectedRow(int row,QTableView *widget) {
    QString data[4];
    for(int i=0;i<4;i++)
        data[i]=widget->model()->index(row,i).data().toString();
    if(dbOpened) {
        QSqlQuery query;
        query.exec(QString("UPDATE 其他账号信息 "
                           "SET 序号='%1', 种类='%2', 用户名='%3', 密码='%4' "
                           "WHERE 名称='%5'").arg(data[0]).arg(data[1]).arg(data[2]).arg(data[3]).arg(data[0]));
    }
    else
        QMessageBox::warning(NULL,"Error","数据库未成功连接",QMessageBox::Ok);
}

/*
 * 根据"评价信息"表，更新"菜单"表中的"评价数量"列和"平均评价分数"列，"客户账户"中的"评价数量"列，防止误改
 */
void admin::updateEvaluationNumber() {
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

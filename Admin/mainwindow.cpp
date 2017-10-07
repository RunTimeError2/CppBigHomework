/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：主界面
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "combodelegate.h"
#include "spindelegate.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->btnMenuRefresh,SIGNAL(clicked()),this,SLOT(on_btnMenuRefresh_clicked()),Qt::UniqueConnection);
    connect(ui->btnMenuNew,SIGNAL(clicked()),this,SLOT(on_btnMenuNew_clicked()),Qt::UniqueConnection);
    connect(ui->btnMenuDelete,SIGNAL(clicked()),this,SLOT(on_btnMenuDelete_clicked()),Qt::UniqueConnection);

    connect(ui->btnGuestRefresh,SIGNAL(clicked()),this,SLOT(on_btnGuestRefresh_clicked()),Qt::UniqueConnection);
    connect(ui->btnGuestNew,SIGNAL(clicked()),this,SLOT(on_btnGuestNew_clicked()),Qt::UniqueConnection);
    connect(ui->btnGuestDelete,SIGNAL(clicked()),this,SLOT(on_btnGuestDelete_clicked()),Qt::UniqueConnection);

    connect(ui->btnOtherRefresh,SIGNAL(clicked()),this,SLOT(on_btnOtherRefresh_clicked()),Qt::UniqueConnection);
    connect(ui->btnOtherNew,SIGNAL(clicked()),this,SLOT(on_btnOtherNew_clicked()),Qt::UniqueConnection);
    connect(ui->btnOtherDelete,SIGNAL(clicked()),this,SLOT(on_btnOtherDelete_clicked()),Qt::UniqueConnection);

    connect(ui->btnTableRefresh,SIGNAL(clicked()),this,SLOT(on_btnTableRefresh_clicked()),Qt::UniqueConnection);
    connect(ui->btnTableNew,SIGNAL(clicked()),this,SLOT(on_btnTableNew_clicked()),Qt::UniqueConnection);
    connect(ui->btnTableDelete,SIGNAL(clicked()),this,SLOT(on_btnTableDelete_clicked()),Qt::UniqueConnection);

    connect(ui->btnChange,SIGNAL(clicked()),this,SLOT(on_btnChange_clicked()),Qt::UniqueConnection);

    connect(ui->btnFindNext,SIGNAL(clicked()),this,SLOT(findMenuNext()));
    connect(ui->btnFindLast,SIGNAL(clicked()),this,SLOT(findMenuLast()));

    Admin.setupDataBase();
    bool success=Admin.setupDBTables();
    if(!success) {
        QMessageBox::critical(NULL,"Error","读取数据库失败！");
        this->close();
    }

    //更新评价信息
    Admin.updateEvaluationNumber();

    //刷新各表格
    on_btnMenuRefresh_clicked();
    on_btnGuestRefresh_clicked();
    on_btnOtherRefresh_clicked();
    on_btnTableRefresh_clicked();

    //获取管理员账户名
    QSqlQuery query;
    query.exec("SELECT * FROM 管理员账户");
    query.next();
    ui->lineEditChangeName->setText(query.value(0).toString());

    tableEdited=false;

    //初始化主界面
    setUpUI();
    slot_tab1();

    //显示管理员账户名
    ui->labelUsername->setText(query.value(0).toString());
    ui->labelUsername->setParent(bgLabel);
    ui->labelUsername->setGeometry(0,160,180,20);
    ui->labelUsername->setAlignment(Qt::AlignCenter);
    ui->labelUsername->setStyleSheet("color: white");
    ui->labelUsername->setFont(QFont("微软雅黑",10));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 更新菜品总数、各种菜品数量的统计
 */
void MainWindow::updateMenuLabels() {
    int count=ui->tvMenu->model()->rowCount();
    int c1=0,c2=0,c3=0,c4=0,c5=0;
    for(int i=0;i<count;i++) {
        if(ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,5)).toString()=="冷菜")
            c1++;
        if(ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,5)).toString()=="热菜")
            c2++;
        if(ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,5)).toString()=="汤")
            c3++;
        if(ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,5)).toString()=="主食")
            c4++;
        if(ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,5)).toString()=="饮料")
            c5++;
    }
    ui->labelMenuTotal->setText("菜品总数："+QString::number(count));
    ui->labelMenuNum1->setText("冷菜："+QString::number(c1));
    ui->labelMenuNum2->setText("热菜："+QString::number(c2));
    ui->labelMenuNum3->setText("汤："+QString::number(c3));
    ui->labelMenuNum4->setText("主食："+QString::number(c4));
    ui->labelMenuNum5->setText("饮料："+QString::number(c5));
}

/*
 * 更新餐桌数与可容纳人数的统计
 */
void MainWindow::updateTableLabels() {
    int count=0,num=0;
    for(int i=0;i<ui->tvTable->model()->rowCount();i++) {
        count+=ui->tvTable->model()->data(ui->tvTable->model()->index(i,2)).toInt();
        num+=ui->tvTable->model()->data(ui->tvTable->model()->index(i,2)).toInt()*(ui->tvTable->model()->data(ui->tvTable->model()->index(i,1)).toInt());
    }
    ui->labelTableCount->setText("餐桌总数："+QString::number(count));
    ui->labelTableNum->setText("可容纳人数："+QString::number(num));
}

/*
 * 更新顾客账户数
 */
void MainWindow::updateGuestLabels() {
    ui->labelGuestCount->setText("顾客账号数："+QString::number(ui->tvGuest->model()->rowCount()));
}

/*
 * 查找上一个
 */
void MainWindow::updateOtherLabels() {
    int count=ui->tvOther->model()->rowCount(),c1=0,c2=0,c3=0;
    for(int i=0;i<count;i++) {
        if(ui->tvOther->model()->data(ui->tvOther->model()->index(i,1)).toString()=="经理")
            c1++;
        if(ui->tvOther->model()->data(ui->tvOther->model()->index(i,1)).toString()=="厨师")
            c2++;
        if(ui->tvOther->model()->data(ui->tvOther->model()->index(i,1)).toString()=="服务员")
            c3++;
    }
    ui->labelOtherCount->setText("员工账号数量："+QString::number(count));
    ui->labelOtherNum1->setText("经理："+QString::number(c1));
    ui->labelOtherNum2->setText("厨师："+QString::number(c2));
    ui->labelOtherNum3->setText("服务员："+QString::number(c3));
}

/*
 * 查找下一个
 */
void MainWindow::findMenuNext() {
    QString name=ui->lineEditFind->text();
    int row=ui->tvMenu->currentIndex().row();
    if(row<0 || row>ui->tvMenu->model()->rowCount())
        row=0;
    if(row==0)
        row=-1;
    if(name=="") {
        QMessageBox::information(this,"Error","必须输入名称");
        return;
    }
    for(int i=row+1;i<ui->tvMenu->model()->rowCount();i++)
        if(i>=0 && i<ui->tvMenu->model()->rowCount()) {
            QString tmp=ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,0)).toString();
            if(tmp.contains(name)) {
                ui->tvMenu->selectRow(i);
                return;
            }
        }
    QMessageBox::information(this,"Error","无法找到'"+name+"'");
}

/*
 * 查找上一个
 */
void MainWindow::findMenuLast() {
    QString name=ui->lineEditFind->text();
    int row=ui->tvMenu->currentIndex().row();
    if(row<0 || row>ui->tvMenu->model()->rowCount())
        row=ui->tvMenu->model()->rowCount();
    if(name=="") {
        QMessageBox::information(this,"Error","必须输入名称");
        return;
    }
    for(int i=row-1;i>=0;i--) {
        if(i>=0 && i<ui->tvMenu->model()->rowCount()) {
            QString tmp=ui->tvMenu->model()->data(ui->tvMenu->model()->index(i,0)).toString();
            if(tmp.contains(name)) {
                ui->tvMenu->selectRow(i);
                return;
            }
        }
    }
    QMessageBox::information(this,"Error","无法找到'"+name+"'");
}

/*
 * 菜单表新建项
 */
void MainWindow::on_btnMenuNew_clicked() {
    Admin.addMenu();
    Admin.viewInTableView("菜单",ui->tvMenu);
    updateMenuLabels();
}

/*
 * 刷新菜单表
 */
void MainWindow::on_btnMenuRefresh_clicked() {
    if(Admin.dbOpened)
        Admin.viewInTableView("菜单",ui->tvMenu);
    updateMenuLabels();
}

/*
 * 菜单表删除项
 */
void MainWindow::on_btnMenuDelete_clicked() {
    Admin.deleteMenuSelectedRow(ui->tvMenu);
    Admin.viewInTableView("菜单",ui->tvMenu);
    updateMenuLabels();
}

/*
 * 刷新顾客账户表
 */
void MainWindow::on_btnGuestRefresh_clicked() {
    if(Admin.dbOpened)
        Admin.viewInTableView("顾客账号信息",ui->tvGuest);
    updateGuestLabels();
}

/*
 * 顾客账户表新建项
 */
void MainWindow::on_btnGuestNew_clicked() {
    Admin.addGuest();
    Admin.viewInTableView("顾客账号信息",ui->tvGuest);
}

/*
 * 顾客账户表删除项
 */
void MainWindow::on_btnGuestDelete_clicked() {
    Admin.deleteGuestSelectedRow(ui->tvGuest);
    Admin.viewInTableView("顾客账号信息",ui->tvGuest);
    updateGuestLabels();
}

/*
 * 刷新其他账户表
 */
void MainWindow::on_btnOtherRefresh_clicked() {
    if(Admin.dbOpened)
        Admin.viewInTableView("其他账号信息",ui->tvOther);
    updateOtherLabels();
}

/*
 * 其他账户表新建项
 */
void MainWindow::on_btnOtherNew_clicked() {
    Admin.addOther(ui->tvOther);
    Admin.viewInTableView("其他账号信息",ui->tvOther);
}

/*
 * 其他账户表删除项
 */
void MainWindow::on_btnOtherDelete_clicked() {
    Admin.deleteOtherSelectedRow(ui->tvOther);
    Admin.viewInTableView("其他账号信息",ui->tvOther);
    updateOtherLabels();
}

/*
 * 更改管理员账户
 */
void MainWindow::on_btnChange_clicked() {
    QString oldName,oldPw;
    QSqlQuery query;
    query.exec("SELECT * FROM 管理员账户");
    query.next();
    oldName=query.value(0).toString();
    oldPw=query.value(1).toString();
    if(ui->lineEditChangepw->text()!=oldPw) {
        QMessageBox::information(this,"Error","旧密码错误");
        return;
    }
    if(ui->lineEditNewpw->text()!=ui->lineEditConfirmpw->text()) {
        QMessageBox::information(this,"Error","请输入相同的密码");
        return;
    }
    if(ui->lineEditNewpw->text().length()<6) {
        QMessageBox::information(this,"Error","密码至少6位");
        return;
    }
    query.exec(QString("UPDATE 管理员账户 SET 账户名='%1', 密码='%2' WHERE 账户名='%3'").arg(ui->lineEditChangeName->text()).arg(ui->lineEditNewpw->text()).arg(oldName));
    QMessageBox::information(this,"Success","修改成功");
    ui->labelUsername->setText(ui->lineEditChangeName->text());
}

/*
 * 菜单表新建项
 */
void MainWindow::on_btnTableNew_clicked() {
    Admin.addTable();
    Admin.viewInTableView("餐桌信息",ui->tvTable);
}

/*
 * 刷新菜单表
 */
void MainWindow::on_btnTableRefresh_clicked() {
    if(Admin.dbOpened)
        Admin.viewInTableView("餐桌信息",ui->tvTable);
    updateTableLabels();
}

/*
 * 菜单表删除项
 */
void MainWindow::on_btnTableDelete_clicked() {
    Admin.deleteTableSelectedRow(ui->tvTable);
    Admin.viewInTableView("餐桌信息",ui->tvTable);
    updateTableLabels();
}

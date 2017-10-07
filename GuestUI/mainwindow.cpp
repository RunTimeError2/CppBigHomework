/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.cpp
 * 摘要：顾客端主界面功能实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myinputbox.h"
#include <QPushButton>
#include <QMessageBox>
#include <QHostInfo>
#include <QStandardItemModel>
#include <QKeyEvent>

//默认使用的端口
#define DEFAULT_PORT 8000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //加载界面，设置为无边框窗口
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    //网络连接部分
    status=false; //设置为登录模式
    port=DEFAULT_PORT; //设置为默认端口
    serverIP=new QHostAddress();
    islogin=false;

    //按钮事件部分
    connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(btn_Login()));                                     //登录按钮
    connect(ui->btnRegister,SIGNAL(clicked()),this,SLOT(btn_Register()));                               //注册按钮
    connect(ui->btnSelectTable,SIGNAL(clicked()),this,SLOT(btn_Choose()));                              //选桌按钮
    connect(ui->btnOrderDone,SIGNAL(clicked()),this,SLOT(btn_Order()));                                 //确认点菜按钮
    connect(ui->btnContinue,SIGNAL(clicked()),this,SLOT(btn_Continue()));                               //继续点菜按钮
    connect(ui->btnConfirmOrder,SIGNAL(clicked()),this,SLOT(slot_order_done()));                        //确认下单按钮
    connect(ui->btnConfirmEvaluate,SIGNAL(clicked()),this,SLOT(btn_ConfirmEvaluate()));                 //提交评价按钮
    connect(ui->btnPayConfirm,SIGNAL(clicked()),this,SLOT(btn_PayConfirm()));
    connect(ui->btnTab6,SIGNAL(clicked()),this,SLOT(slot_getdishstaterequest()));
    connect(ui->btnTab5,SIGNAL(clicked()),this,SLOT(slot_msgqueue_request()));
    connect(ui->btnMsg,SIGNAL(clicked()),this,SLOT(slot_btnMsg()));
    connect(ui->btnClearOrder,SIGNAL(clicked()),this,SLOT(btn_ClearMenuOrderList()));
    connect(ui->btnEvaluateWaiter,SIGNAL(clicked()),this,SLOT(btn_EvaluateWaiter()));
    connect(ui->btnChangeAccount,SIGNAL(clicked()),this,SLOT(slot_btnChangeAccount()));

    //数据处理部分
    dataprocessing=new gDataProcessing;
    connect(dataprocessing,SIGNAL(login_success()),this,SLOT(slot_login_success()));                    //登录成功
    connect(dataprocessing,SIGNAL(login_phoneerror()),this,SLOT(slot_login_phoneerror()));              //登录失败，手机号不存在
    connect(dataprocessing,SIGNAL(login_passworderror()),this,SLOT(slot_login_passworderror()));        //登录失败，密码错误
    connect(dataprocessing,SIGNAL(register_success()),this,SLOT(slot_register_success()));              //注册成功
    connect(dataprocessing,SIGNAL(register_phoneerror()),this,SLOT(slot_register_phoneerror()));        //注册失败，手机号已存在
    connect(dataprocessing,SIGNAL(getmenu()),this,SLOT(slot_getmenu()));                                //获取菜单信息
    connect(dataprocessing,SIGNAL(gettableinfo()),this,SLOT(slot_gettableinfo()));                      //获取餐桌信息
    connect(dataprocessing,SIGNAL(choose_success()),this,SLOT(slot_choose_success()));                  //选桌成功
    connect(dataprocessing,SIGNAL(choose_fail()),this,SLOT(slot_choose_failure()));                     //选桌失败
    connect(dataprocessing,SIGNAL(pay_done()),this,SLOT(slot_pay_done()));                              //完成付款
    connect(dataprocessing,SIGNAL(get_dishstate(QList<SingleDishOrdered>&)),this,SLOT(slot_getdishstate(QList<SingleDishOrdered>&)));
                                                                                                        //获取菜品状态
    connect(dataprocessing,SIGNAL(guest_singlemessage(QString)),this,SLOT(slot_singlemessage(QString)));
                                                                                                        //获取单条消息
    connect(dataprocessing,SIGNAL(guest_msgqueue(QList<QString>)),this,SLOT(slot_msgqueue(QList<QString>)));
                                                                                                        //获取消息队列

    //点击表格时自动更新表格
    connect(ui->tableViewMenu,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_update_menutable()));         //菜单表
    connect(ui->tableViewOrder,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_update_ordertable()));       //已选菜品表
    //设置事件过滤器，对于可编辑的表格，实现在按回车时自动更新表格
    ui->tableViewMenu->installEventFilter(this);
    ui->tableViewOrder->installEventFilter(this);

    //设置各个表格的编辑模式
    ui->tableViewMenu->setSelectionMode(QAbstractItemView::SingleSelection);                            //仅能选择一项
    ui->tableViewTable->setSelectionBehavior(QAbstractItemView::SelectRows);                            //整行选择
    ui->tableViewTable->setSelectionMode(QAbstractItemView::SingleSelection);                           //仅能选择一项
    ui->tableViewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);                             //不可编辑
    ui->tableViewOrder->setSelectionMode(QAbstractItemView::SingleSelection);                           //仅能选择一项
    ui->tableViewEvaluate->setSelectionMode(QAbstractItemView::SingleSelection);                        //仅能选择一项
    ui->tableViewEvaluate->setSelectionBehavior(QAbstractItemView::SelectRows);                         //整行选择
    ui->tableViewState->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewState->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewState->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //显示登录界面
    setLoginForm();

    evaluate_waiterusername="";

    model=new QStandardItemModel;
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal,"点菜数量");
    model->setHeaderData(1,Qt::Horizontal,"菜品名称");
    model->setHeaderData(2,Qt::Horizontal,"菜品类别");
    model->setHeaderData(3,Qt::Horizontal,"是否有售");
    model->setHeaderData(4,Qt::Horizontal,"价格");
    model->setHeaderData(5,Qt::Horizontal,"平均评价分数");
    ui->tableViewMenu->setModel(model);

    model3=new QStandardItemModel;
    model3->setColumnCount(5);
    model3->setHeaderData(0,Qt::Horizontal,"数量");
    model3->setHeaderData(1,Qt::Horizontal,"菜品名称");
    model3->setHeaderData(2,Qt::Horizontal,"菜品类别");
    model3->setHeaderData(3,Qt::Horizontal,"单价");
    model3->setHeaderData(4,Qt::Horizontal,"总价");
    ui->tableViewOrder->setModel(model3);

    model2=new QStandardItemModel;
    model2->setColumnCount(4);
    model2->setHeaderData(0,Qt::Horizontal,"餐桌编号");
    model2->setHeaderData(1,Qt::Horizontal,"餐桌种类");
    model2->setHeaderData(2,Qt::Horizontal,"餐桌容纳人数");
    model2->setHeaderData(3,Qt::Horizontal,"是否空闲");
    ui->tableViewTable->setModel(model2);

    QStandardItemModel *model4=new QStandardItemModel;
    model4->setColumnCount(5);
    model4->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model4->setHeaderData(1,Qt::Horizontal,"菜品种类");
    model4->setHeaderData(2,Qt::Horizontal,"单价");
    model4->setHeaderData(3,Qt::Horizontal,"评价");
    model4->setHeaderData(4,Qt::Horizontal,"评分");
    ui->tableViewEvaluate->setModel(model4);

    this->setStyleSheet("QTableView {"
                        "    background-color: rgba(255,255,255,0.5);"
                        "}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 成功连接到服务器，自动发送登录或注册信息
 */
void MainWindow::slotConnected() {
    status=true;
    if(state) { //将登录信息编码后发给服务器端
        QByteArray loginData=dataprocessing->EncodingGuestLogin(ibLoginPhonenumber->getText(),ibLoginPassword->getText());
        tcpSocket->write(loginData,loginData.length());
    }
    else { //将注册信息编码后发给服务器端
        QByteArray registerData=dataprocessing->EncodingGuestRegister(ibRegisterPhonenumber->getText(),ibRegisterUsername->getText(),ibRegisterPassword->getText());
        tcpSocket->write(registerData,registerData.length());
    }
}

/*
 * 断开连接
 */
void MainWindow::slotDisconnected() {
    if(islogin) {
        QMessageBox::information(this,"Disconnected","与服务器端断开连接！请重新运行客户端！");
        this->close();
    }
}

/*
 * 收到消息
 */
void MainWindow::dataReceived() {
    while(tcpSocket->bytesAvailable()>0) {

        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());

        dataprocessing->ProcessingData(datagram);
    }
}

/*
 * 重置TcpSocket套接字，检验信息是否有效，并连接服务器
 */
void MainWindow::btn_Login() {
    state=true;
    if(status) {
        status=false;
        tcpSocket->close();
    }
    if(!status) {
        QString ip=ibServerIP->getText();
        if(!serverIP->setAddress(ip)) {
            QMessageBox::information(this,tr("Error"),tr("请输入有效的IP地址"));
            return;
        }
        QString phone=ibLoginPhonenumber->getText();
        if(phone.length()!=11) {
            QMessageBox::information(this,tr("Error"),("请输入有效的手机号"));
            return;
        }
        for(int i=0;i<phone.length();i++)
            if(phone.at(i)<'0' || phone.at(i)>'9') {
                QMessageBox::information(this,tr("Error"),tr("请输入有效的手机号"));
                return;
            }
        QString password=ibLoginPassword->getText();
        if(password.length()<6) {
            QMessageBox::information(this,tr("Error"),tr("密码至少6位"));
            return;
        }
        tcpSocket=new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        tcpSocket->connectToHost(*serverIP,port);
    }
}

/*
 * 登录成功，自动发送获取菜单请求和获取餐桌信息请求，并转到主界面
 */
void MainWindow::slot_login_success() {
    QByteArray msg;
    msg.append((unsigned char)TOKEN_GUEST_SENDTABLE);
    tcpSocket->write(msg,1); //发送获取餐桌信息请求

    QByteArray msg2;
    msg2.append((unsigned char)TOKEN_GUEST_GETMENU);
    g_phonenumber=ibLoginPhonenumber->getText();
    g_password=ibLoginPassword->getText();
    QMessageBox::information(this,tr("success"),tr("登录成功")); //必须放在这里，连续两次发送消息之间必须有停顿
    tcpSocket->write(msg2,1); //发送获取菜单信息请求
    islogin=true;
    setMainForm(); //显示主界面
}

/*
 * 登录失败，原因是密码错误
 */
void MainWindow::slot_login_passworderror() {
    QMessageBox::information(this,tr("failure"),tr("密码错误"));
}

/*
 * 登录失败，原因是用户不存在
 */
void MainWindow::slot_login_phoneerror() {
    QMessageBox::information(this,tr("failure"),tr("用户不存在"));
}

/*
 * 注册，检验填写信息是否有效并给服务器端发送注册信息
 */
void MainWindow::btn_Register() {
    state=false;
    if(status) {
        status=false;
        tcpSocket->close();
    }

    if(!status) {
        QString ip=ibServerIP->getText();
        if(!serverIP->setAddress(ip)) {
            QMessageBox::information(this,tr("Error"),tr("请输入有效的IP地址"));
            return;
        }
        QString phone=ibRegisterPhonenumber->getText();
        if(phone.length()!=11) {
            QMessageBox::information(this,tr("Error"),tr("请输入有效的手机号"));
            return;
        }
        QString password=ibRegisterPassword->getText();
        QString pwconfirm=ibConfirmPassword->getText();
        if(pwconfirm!=password) {
            QMessageBox::information(this,tr("Error"),tr("请输入相同的密码"));
            return;
        }
        for(int i=0;i<phone.length();i++)
            if(phone.at(i)<'0' || phone.at(i)>'9') {
                QMessageBox::information(this,tr("Error"),tr("请输入有效的手机号"));
                return;
            }
        if(password.length()<6) {
            QMessageBox::information(this,tr("Error"),tr("密码至少6位"));
            return;
        }
        if(ibRegisterUsername->getText()=="") {
            QMessageBox::information(this,tr("Error"),tr("用户名不能为空"));
            return;
        }
        tcpSocket=new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        tcpSocket->connectToHost(*serverIP,port);
    }
}

/*
 * 注册成功提示
 */
void MainWindow::slot_register_success() {
    QMessageBox::information(this,tr("success"),tr("注册成功"));
}

/*
 * 注册失败，原因是手机号已被注册
 */
void MainWindow::slot_register_phoneerror() {
    QMessageBox::information(this,tr("failure"),tr("手机号已注册"));
}

/*
 * 从服务器端获取用户名
 */
void MainWindow::slot_getusername(QString username) {
    g_username=username;
}

/*
 * 根据从服务器端收到的菜单信息，生成菜单表
 */
void MainWindow::slot_getmenu() {
    model=new QStandardItemModel;
    model->setColumnCount(7);
    model->setHeaderData(0,Qt::Horizontal,"点菜数量");
    model->setHeaderData(1,Qt::Horizontal,"菜品名称");
    model->setHeaderData(2,Qt::Horizontal,"菜品类别");
    model->setHeaderData(3,Qt::Horizontal,"是否有售");
    model->setHeaderData(4,Qt::Horizontal,"价格");
    model->setHeaderData(5,Qt::Horizontal,"平均评价分数");
    model->setHeaderData(6,Qt::Horizontal,"  ");
    ui->tableViewMenu->setModel(model);
    ui->tableViewMenu->setItemDelegateForColumn(0,new SpinDelegate());
    ButtonDelegate *buttonDelegate=new ButtonDelegate(this,"加一份");
    ui->tableViewMenu->setItemDelegateForColumn(6,buttonDelegate);
    connect(buttonDelegate,SIGNAL(buttonClicked(int,int)),this,SLOT(slot_menuPlusone(int,int)));
    for(int i=0;i<dataprocessing->getMenu().num;i++) {
        model->setItem(i,0,new QStandardItem("0"));
        model->setItem(i,1,new QStandardItem(dataprocessing->getMenu().list[i].name));
        model->setItem(i,2,new QStandardItem(dataprocessing->getMenu().list[i].type));
        model->setItem(i,3,new QStandardItem((dataprocessing->getMenu().list[i].isvalid==1)?"是":"否"));
        model->setItem(i,4,new QStandardItem(QString("%1").arg(dataprocessing->getMenu().list[i].price)));
        model->setItem(i,5,new QStandardItem(QString("%1").arg(dataprocessing->getMenu().list[i].score)));
        model->setItem(i,6,new QStandardItem(" "));
        //文字居中
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
        model->item(i,4)->setTextAlignment(Qt::AlignCenter);
        model->item(i,5)->setTextAlignment(Qt::AlignCenter);
        if(dataprocessing->getMenu().list[i].isvalid==0) { //不可点菜
            for(int j=0;j<model->columnCount();j++)
                model->item(i,j)->setBackground(QBrush(QColor::fromRgb(255,0,0)));
            model->item(i,0)->setFlags(model->item(i,0)->flags() & (~Qt::ItemIsEditable));
        }
        //除了"点菜数量"列以外，其他列都不可编辑
        model->item(i,1)->setFlags(model->item(i,1)->flags() & (~Qt::ItemIsEditable));
        model->item(i,2)->setFlags(model->item(i,2)->flags() & (~Qt::ItemIsEditable));
        model->item(i,3)->setFlags(model->item(i,3)->flags() & (~Qt::ItemIsEditable));
        model->item(i,4)->setFlags(model->item(i,4)->flags() & (~Qt::ItemIsEditable));
        model->item(i,5)->setFlags(model->item(i,5)->flags() & (~Qt::ItemIsEditable));
        model->item(i,6)->setFlags(model->item(i,6)->flags() & (~Qt::ItemIsEditable));
    }
    updateLabelList(); //更新菜单上方提示信息
    //设置各列宽度
    ui->tableViewMenu->setColumnWidth(0,80);
    ui->tableViewMenu->setColumnWidth(1,200);
    ui->tableViewMenu->setColumnWidth(2,80);
    ui->tableViewMenu->setColumnWidth(3,80);
    ui->tableViewMenu->setColumnWidth(4,80);
    ui->tableViewMenu->setColumnWidth(5,90);
    //设置表格样式
    ui->tableViewMenu->setStyleSheet( "QTableView{"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    alternate-background-color: rgba(255,255,255,0.5);"
                                      "    gridline-color: rgba(0,0,0,0);"
                                      "}");
    ui->tableViewMenu->setShowGrid(false);
    ui->tableViewMenu->verticalHeader()->setVisible(false);

    g_username=dataprocessing->getmy_username();
    ui->labelUsername->setGeometry(20,320,160,30);
    ui->labelUsername->setText("用户名： "+g_username);
    ui->labelUsername->setFont(QFont("微软雅黑",10));
    ui->labelPhonenumber->setGeometry(20,350,160,30);
    ui->labelPhonenumber->setText("手机号： "+g_phonenumber);
    ui->labelPhonenumber->setFont(QFont("微软雅黑",10));
    ui->labelServer->setGeometry(20,380,160,30);
    ui->labelServer->setText("服务器IP： "+serverIP->toString());
    ui->labelServer->setFont(QFont("微软雅黑",10));
    ui->labelUsername->setStyleSheet("color: white;");
    ui->labelPhonenumber->setStyleSheet("color: white;");
    ui->labelServer->setStyleSheet("color: white;");
    ui->lineEditChangeNew->setText(g_username);
}

/*
 * 下单操作，并给服务器端发送消息
 */
void MainWindow::slot_order_done() {
    if(ui->tableViewOrder->model()->rowCount()>0) {
        orderList list;
        list.count=ui->tableViewOrder->model()->rowCount();
        list.phonenumber=g_phonenumber;
        list.list=new orderList::dish[list.count];
        for(int i=0;i<list.count;i++) {
            list.list[i].num=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            list.list[i].name=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,1)).toString();
        }

        QByteArray msg=dataprocessing->EncodingGuestOrder(list);
        tcpSocket->write(msg,msg.length());

        QMessageBox::information(this,"success","下单成功！");
        slot_tab6();

        for(int i=0;i<ui->tableViewMenu->model()->rowCount();i++)
            if(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt()>0)
                ui->tableViewMenu->model()->setData(ui->tableViewMenu->model()->index(i,0),0);
        slot_update_menutable();
        ui->tableViewOrder->model()->removeRows(0,ui->tableViewOrder->model()->rowCount());
    }
    else
        QMessageBox::information(this,"failure","所选菜品不能为空！");
}

/*
 * 根据已点菜品，自动生成表格并转到该表
 */
void MainWindow::btn_Order() {
    model3=new QStandardItemModel;
    model3->setColumnCount(5);
    model3->setHeaderData(0,Qt::Horizontal,"数量");
    model3->setHeaderData(1,Qt::Horizontal,"菜品名称");
    model3->setHeaderData(2,Qt::Horizontal,"菜品类别");
    model3->setHeaderData(3,Qt::Horizontal,"单价");
    model3->setHeaderData(4,Qt::Horizontal,"总价");
    ui->tableViewOrder->setModel(model3);
    ui->tableViewOrder->setItemDelegateForColumn(0,new SpinDelegate());
    int num=0;
    for(int i=0;i<ui->tableViewMenu->model()->rowCount();i++) {
        if(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toString()!="0") {
            model3->setItem(num,0,new QStandardItem(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toString())); //数量
            model3->setItem(num,1,new QStandardItem(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,1)).toString())); //名称
            model3->setItem(num,2,new QStandardItem(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,2)).toString())); //类别
            model3->setItem(num,3,new QStandardItem(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,4)).toString())); //单价
            float singleprice=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,4)).toFloat();
            int number=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
            model3->setItem(num,4,new QStandardItem(QString("%1").arg((float)number*singleprice))); //总价
            //文字居中
            model3->item(num,0)->setTextAlignment(Qt::AlignCenter);
            model3->item(num,1)->setTextAlignment(Qt::AlignCenter);
            model3->item(num,2)->setTextAlignment(Qt::AlignCenter);
            model3->item(num,3)->setTextAlignment(Qt::AlignCenter);
            model3->item(num,4)->setTextAlignment(Qt::AlignCenter);
            model3->item(num,1)->setFlags(model3->item(num,1)->flags() & (~Qt::ItemIsEditable));
            model3->item(num,2)->setFlags(model3->item(num,2)->flags() & (~Qt::ItemIsEditable));
            model3->item(num,3)->setFlags(model3->item(num,3)->flags() & (~Qt::ItemIsEditable));
            model3->item(num,4)->setFlags(model3->item(num,3)->flags() & (~Qt::ItemIsEditable));
            num++;
        }
    }
    ui->tableViewOrder->setColumnWidth(0,100);
    ui->tableViewOrder->setColumnWidth(1,290);
    ui->tableViewOrder->setColumnWidth(2,100);
    ui->tableViewOrder->setColumnWidth(3,100);
    ui->tableViewOrder->setColumnWidth(4,130);
    QMessageBox::information(this,"done","点菜完成！");
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(true);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    slot_tab3();
    updateOrderLabelList();
    ui->tableViewOrder->setShowGrid(false);
    ui->tableViewOrder->verticalHeader()->setVisible(false);
}

/*
 * 收到服务器端选桌成功提示
 */
void MainWindow::slot_choose_success() {
    dataprocessing->getmyTable()=mytargetTable;
    for(int i=0;i<4;i++)
        model2->item(dataprocessing->getmyTable(),i)->setBackground(QBrush(QColor::fromRgb(128,128,255)));
    ui->tableViewTable->model()->setData(ui->tableViewTable->model()->index(dataprocessing->getmyTable(),3),"已选");
    QMessageBox::information(this,"success","选桌成功");
    ui->btnSelectTable->setEnabled(false);
    slot_tab1();
}

/*
 * 收到服务器端选桌失败提示
 */
void MainWindow::slot_choose_failure() {
    QMessageBox::information(this,"success","选桌失败");
}

/*
 * 收到服务器端确认信息，完成结账
 */
void MainWindow::slot_pay_done() {
    QMessageBox::information(this,"done","结账成功！\n请及时评价菜品，退出程序后将不可评价");
    ready_evaluate();
    ui->tableViewOrder->model()->removeRows(0,ui->tableViewOrder->model()->rowCount());
    ui->tableViewState->model()->removeRows(0,ui->tableViewState->model()->rowCount());
    updateLabelList();
    updateOrderLabelList();
    ui->frameTab1->setVisible(false);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(true);
    ui->frameTab5->setVisible(false);
    slot_tab4();
    dataprocessing->getmyPrice()=0;
    dataprocessing->getmyTable()=-1;
    evaluate_waiterusername=dataprocessing->getwaiter_name();
    dataprocessing->getwaiter_name()="";
    ui->labelWaiterInfo->setText("<未被认领>");
}

/*
 * 确认结账
 */
void MainWindow::btn_PayConfirm() {
    if(dataprocessing->getmyTable()==-1) {
        QMessageBox::information(this,"Error","必须先选桌再结账");
        return;
    }
    if(QMessageBox::information(this,"Confirm",QString("确定结账？\n总价：%1元").arg(dataprocessing->getmyPrice()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)==QMessageBox::Yes) {
        QByteArray msg;
        msg.append(TOKEN_GUEST_PAYCONFIRM);
        tcpSocket->write(msg,1);
    }
}

/*
 * 根据收到的餐桌信息，生成餐桌信息表
 */
void MainWindow::slot_gettableinfo() {
    model2=new QStandardItemModel;
    model2->setColumnCount(4);
    model2->setHeaderData(0,Qt::Horizontal,"餐桌编号");
    model2->setHeaderData(1,Qt::Horizontal,"餐桌种类");
    model2->setHeaderData(2,Qt::Horizontal,"餐桌容纳人数");
    model2->setHeaderData(3,Qt::Horizontal,"是否空闲");
    ui->tableViewTable->setModel(model2);
    for(int i=0;i<dataprocessing->getTable().count;i++) {
        model2->setItem(i,0,new QStandardItem(QString("%1号桌").arg(i+1)));
        model2->setItem(i,1,new QStandardItem(dataprocessing->getTable().typelist[i]));
        model2->setItem(i,2,new QStandardItem(QString("%1").arg(dataprocessing->getTable().numlist[i])));
        model2->setItem(i,3,new QStandardItem((dataprocessing->getTable().valid[i]==1)?"是":"否"));
        //文字居中
        model2->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model2->item(i,3)->setTextAlignment(Qt::AlignCenter);
        if(dataprocessing->getTable().valid[i]==0)
            for(int j=0;j<4;j++)
                model2->item(i,j)->setBackground(QBrush(QColor::fromRgb(255,0,0)));
    }
    ui->tableViewTable->setColumnWidth(0,150);
    ui->tableViewTable->setColumnWidth(0,150);
    ui->tableViewTable->setColumnWidth(0,150);
    ui->tableViewTable->setColumnWidth(0,150);
    ui->tableViewTable->setStyleSheet( "QTableView{"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(0,0,0,0);"
                                      "    alternate-background-color: rgba(255,255,255,0.5);"
                                      "    gridline-color: rgba(0,0,0,0);"
                                      "}");
    ui->tableViewTable->setShowGrid(false);
    ui->tableViewTable->verticalHeader()->setVisible(false);

    if(dataprocessing->getmyTable()!=-1) {
        for(int i=0;i<4;i++)
            model2->item(dataprocessing->getmyTable(),i)->setBackground(QBrush(QColor::fromRgb(128,128,255)));
        ui->tableViewTable->model()->setData(ui->tableViewTable->model()->index(dataprocessing->getmyTable(),3),"已选");
        ui->btnSelectTable->setEnabled(false);
    }

    if((!dataprocessing->getischosen()) && (dataprocessing->getwaiter_name()!="")) {
        dataprocessing->getischosen()=true;
        QMessageBox::information(this,"success","已被服务员"+dataprocessing->getwaiter_name()+"成功认领");
        ui->labelWaiterInfo->setText("被 "+dataprocessing->getwaiter_name()+" 认领");
    }
}

/*
 * 继续点菜，转到菜单表
 */
void MainWindow::btn_Continue() {
    //更新菜单中已点菜品的数量
    for(int i=0;i<ui->tableViewOrder->model()->rowCount();i++) {
        QString name=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,1)).toString();
        int num=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
        for(int j=0;j<ui->tableViewMenu->model()->rowCount();j++)
            if(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(j,1)).toString()==name) {
                ui->tableViewMenu->model()->setData(ui->tableViewMenu->model()->index(j,0),num);
                break;
            }
    }
    slot_update_menutable();
    //转到菜单界面
    ui->frameTab1->setVisible(true);
    ui->frameTab2->setVisible(false);
    ui->frameTab3->setVisible(false);
    ui->frameTab4->setVisible(false);
    ui->frameTab5->setVisible(false);
    slot_tab1();
}

/*
 * 更新菜单列表，将已选菜品、没有出售的菜品行标上颜色，修改不合法的数量
 */
void MainWindow::slot_update_menutable() {
    int count=model->rowCount();
    for(int row=0;row<count;row++) {
        int num=model->item(row,0)->text().toInt();
        if(num<0) {
            model->item(row,0)->setText("0");
            return;
        }
        if(num>0) {
            for(int i=0;i<6;i++)
                model->item(row,i)->setBackground(QBrush(QColor::fromRgb(128,255,128)));
        }
        if(num==0) {
            for(int i=0;i<6;i++)
                model->item(row,i)->setBackground(QBrush(QColor::fromRgb(255,255,255,0.5)));
        }
        if(model->item(row,3)->text()=="否")
            for(int i=0;i<6;i++)
                model->item(row,i)->setBackground(QBrush(QColor::fromRgb(255,0,0)));
    }
    updateLabelList();
}

/*
 * 将选桌信息编码并发送
 */
void MainWindow::btn_Choose() {
    int row=ui->tableViewTable->currentIndex().row();
    if(row>=0 && row<ui->tableViewTable->model()->rowCount()) {
        QByteArray msg;
        msg.append((unsigned char)TOKEN_GUEST_CHOOSETABLE);
        msg.append((unsigned char)(row/256));
        msg.append((unsigned char)(row%256));
        tcpSocket->write(msg,3);
        mytargetTable=row;
    }
    else
        QMessageBox::critical(this,"Error","必须选择一张桌子");
}

/*
 * 根据点菜列表计算总价
 */
float MainWindow::calc_totalprice() {
    float ans=0;
    for(int i=0;i<ui->tableViewOrder->model()->rowCount();i++)
        ans+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,4)).toFloat();
    return ans;
}

/*
 * 更新点菜列表，自动去除数量为0的菜品并计算总价
 */
void MainWindow::slot_update_ordertable() {
    int i=0;
    while(i<ui->tableViewOrder->model()->rowCount()) {
        if(ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toString()=="0") {
            ui->tableViewOrder->model()->removeRow(i);
        }
        else {
            float singleprice=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,3)).toFloat();
            int number=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            ui->tableViewOrder->model()->setData(ui->tableViewOrder->model()->index(i,4),QString("%1").arg((float)number*singleprice));
            i++;
        }
    }
    updateOrderLabelList();
}

/*
 * 初始化评价表
 */
void MainWindow::ready_evaluate() {
    QStandardItemModel *model4=new QStandardItemModel;
    model4->setColumnCount(5);
    model4->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model4->setHeaderData(1,Qt::Horizontal,"菜品种类");
    model4->setHeaderData(2,Qt::Horizontal,"单价");
    model4->setHeaderData(3,Qt::Horizontal,"评价");
    model4->setHeaderData(4,Qt::Horizontal,"评分");
    ui->tableViewEvaluate->setModel(model4);
    QList<QString> tmp_name;
    tmp_name.clear();
    int count=0;
    for(int i=0;i<ui->tableViewState->model()->rowCount();i++) {
        QString name=ui->tableViewState->model()->data(ui->tableViewState->model()->index(i,0)).toString();
        if(!tmp_name.contains(name)) {
            model4->setItem(count,0,new QStandardItem(name));
            for(int j=0;j<ui->tableViewMenu->model()->rowCount();j++)
                if(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(j,1)).toString()==name) {
                    model4->setItem(count,1,new QStandardItem(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(j,2)).toString()));
                    model4->setItem(count,2,new QStandardItem(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(j,4)).toString()));
                    break;
                }
            model4->setItem(count,3,new QStandardItem(""));
            model4->setItem(count,4,new QStandardItem(""));
            model4->item(count,0)->setFlags(model4->item(count,0)->flags() & (~Qt::ItemIsEditable));
            model4->item(count,1)->setFlags(model4->item(count,1)->flags() & (~Qt::ItemIsEditable));
            model4->item(count,2)->setFlags(model4->item(count,2)->flags() & (~Qt::ItemIsEditable));
            tmp_name.append(name);
            count++;
        }
    }
    ui->tableViewEvaluate->setColumnWidth(0,150);
    ui->tableViewEvaluate->setColumnWidth(1,90);
    ui->tableViewEvaluate->setColumnWidth(2,90);
    ui->tableViewEvaluate->setColumnWidth(3,280);
    ui->tableViewEvaluate->setColumnWidth(4,100);
    ui->tableViewEvaluate->setItemDelegateForColumn(4,new SpinDelegate(this,1));
    ui->tableViewEvaluate->setStyleSheet("QTableView{"
                                         "    color: rgb(0,0,0);"
                                         "    background-color: rgba(0,0,0,0);"
                                         "    alternate-background-color: rgba(255,255,255,0.5);"
                                         "    gridline-color: rgba(0,0,0,0);"
                                         "}");
    ui->tableViewEvaluate->setShowGrid(false);
    ui->tableViewEvaluate->verticalHeader()->setVisible(false);
    slot_tab4();
}

/*
 * 将所有评价信息编码以发送
 */
QByteArray MainWindow::EncodingEvaluation() {
    int count=0;
    for(int i=0;i<ui->tableViewEvaluate->model()->rowCount();i++)
        if(ui->tableViewEvaluate->model()->data(ui->tableViewEvaluate->model()->index(i,3)).toString()!="")
            count++;
    QByteArray ans;
    if(count==0) {
        ans.append(250);
        return ans;
    }
    ans.append((unsigned char)TOKEN_GUEST_EVALUATE);
    ans.append((unsigned char)count);
    ans.append(g_phonenumber.toUtf8());
    for(int i=0;i<ui->tableViewEvaluate->model()->rowCount();i++)
        if(ui->tableViewEvaluate->model()->data(ui->tableViewEvaluate->model()->index(i,3)).toString()!="") {
            QByteArray tmp;
            tmp=ui->tableViewEvaluate->model()->data(ui->tableViewEvaluate->model()->index(i,0)).toString().toUtf8();
            if(tmp.length()>255)
                tmp=tmp.mid(0,255);
            ans.append((unsigned char)tmp.length());
            ans.append(tmp);
            tmp=ui->tableViewEvaluate->model()->data(ui->tableViewEvaluate->model()->index(i,3)).toString().toUtf8();
            ans.append((unsigned char)tmp.length());
            ans.append(tmp);
            ans.append((unsigned char)ui->tableViewEvaluate->model()->data(ui->tableViewEvaluate->model()->index(i,4)).toInt());
        }
    return ans;
}

/*
 * 确认并发送评价信息
 */
void MainWindow::btn_ConfirmEvaluate() {
    QByteArray msg=EncodingEvaluation();
    if(msg.at(0)==250) {
        QMessageBox::critical(this,"Error","必须填写至少一项评价才可提交");
        return;
    }
    tcpSocket->write(msg,msg.length());
    QMessageBox::information(this,"Success","提交评价成功");
    ui->tableViewEvaluate->model()->removeRows(0,ui->tableViewEvaluate->model()->rowCount());
    ui->btnConfirmEvaluate->setEnabled(false);
}

/*
 * 将付款信息编码以发送
 */
QByteArray MainWindow::EncodingPayInfo() {
    float price=calc_totalprice();
    int final=(int)(price*100.0);
    QByteArray ans;
    ans.append((unsigned char)TOKEN_GUEST_PAY);
    ans.append((unsigned char)(final/65536));
    ans.append((unsigned char)(final%65535/256));
    ans.append((unsigned char)(final%256));
    ans.append(g_phonenumber.toUtf8());
    return ans;
}

/*
 * 根据点菜情况更新菜单表上方提示信息
 */
void MainWindow::updateLabelList() {
    float totalprice=0;
    int num[5] {0};
    int count=0;
    for(int i=0;i<ui->tableViewMenu->model()->rowCount();i++)
        if(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt()>0) {
            count+=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
            totalprice+=(float)(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt()*ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,4)).toFloat());
            QString type=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,2)).toString();
            if(type=="冷菜")
                num[0]+=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
            if(type=="热菜")
                num[1]+=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
            if(type=="汤")
                num[2]+=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
            if(type=="主食")
                num[3]+=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
            if(type=="饮料")
                num[4]+=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(i,0)).toInt();
        }
    ui->labelDishCount->setText(QString("总价：%1   菜品总数：%2   冷菜：%3   热菜：%4   汤：%5   主食：%6   饮料：%7")
                                .arg(totalprice)
                                .arg(count)
                                .arg(num[0])
                                .arg(num[1])
                                .arg(num[2])
                                .arg(num[3])
                                .arg(num[4]));
}

/*
 * 清空菜单列表中的点菜数量
 */
void MainWindow::btn_ClearMenuOrderList() {
    for(int i=0;i<ui->tableViewMenu->model()->rowCount();i++)
        ui->tableViewMenu->model()->setData(ui->tableViewMenu->model()->index(i,0),"0");
    slot_update_menutable();

}

/*
 * 根据点菜情况更新点菜列表上方提示信息
 */
void MainWindow::updateOrderLabelList() {
    float totalprice=0;
    int num[5] {0};
    int count=0;
    for(int i=0;i<ui->tableViewOrder->model()->rowCount();i++)
        if(ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt()>0) {
            count+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            totalprice+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,4)).toFloat();
            QString type=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,2)).toString();
            if(type=="冷菜")
                num[0]+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            if(type=="热菜")
                num[1]+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            if(type=="汤")
                num[2]+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            if(type=="主食")
                num[3]+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
            if(type=="饮料")
                num[4]+=ui->tableViewOrder->model()->data(ui->tableViewOrder->model()->index(i,0)).toInt();
        }
    ui->labelOrderCount->setText(QString("总价：%1   菜品总数：%2   冷菜：%3   热菜：%4   汤：%5   主食：%6   饮料：%7")
                                 .arg(totalprice)
                                 .arg(count)
                                 .arg(num[0])
                                 .arg(num[1])
                                 .arg(num[2])
                                 .arg(num[3])
                                 .arg(num[4]));
}

/*
 * 事件过滤器
 */
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if(watched==ui->tableViewMenu)
        if(event->type()==QEvent::KeyRelease) {
            QKeyEvent *key=static_cast<QKeyEvent*>(event);
            if(key->key()%256==4 || key->key()%256==5) //按下回车键
                slot_update_menutable();
        }
    if(watched==ui->tableViewOrder)
        if(event->type()==QEvent::KeyRelease) {
            QKeyEvent *key2=static_cast<QKeyEvent*>(event);
            if(key2->key()%256==4 || key2->key()%256==5) //按下回车键
                slot_update_ordertable();
        }
    //仅提取事件，不过滤，不影响按回车键的原来功能（确认表格的编辑）
    return QMainWindow::eventFilter(watched,event);
}

/*
 * 获取所有菜品状态
 */
void MainWindow::slot_getdishstate(QList<SingleDishOrdered> &list) {
    QStandardItemModel *model=new QStandardItemModel;
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,"菜品名称");
    model->setHeaderData(1,Qt::Horizontal,"菜品编号");
    model->setHeaderData(2,Qt::Horizontal,"菜品状态");
    ui->tableViewState->setModel(model);
    for(int i=0;i<list.count();i++) {
        model->setItem(i,0,new QStandardItem(list.at(i).name));
        model->setItem(i,1,new QStandardItem(QString::number(list.at(i).index)));
        if(list.at(i).state==DISH_UNCHOSEN)
            model->setItem(i,2,new QStandardItem("未认领"));
        if(list.at(i).state==DISH_ABORTED)
            model->setItem(i,2,new QStandardItem("已取消"));
        if(list.at(i).state==DISH_CHOSEN_UNSTARTED)
            model->setItem(i,2,new QStandardItem("已认领"));
        if(list.at(i).state==DISH_FINISHED)
            model->setItem(i,2,new QStandardItem("已完成"));
        if(list.at(i).state==DISH_STARTED)
            model->setItem(i,2,new QStandardItem("正在做"));
    }
    ui->tableViewState->setColumnWidth(0,200);
    ui->tableViewState->setColumnWidth(1,150);
    ui->tableViewState->setColumnWidth(2,100);
    ui->tableViewState->setStyleSheet( "QTableView{"
                                      "    color: rgb(0,0,0);"
                                      "    background-color: rgba(255,255,255,0.4);"
                                      "}");
    ui->tableViewState->setShowGrid(false);
    ui->tableViewState->verticalHeader()->setVisible(false);

    ui->labelFinalPrice->setText("总价："+QString::number(dataprocessing->getmyPrice()));


    if((!dataprocessing->getischosen()) && (dataprocessing->getwaiter_name()!="")) {
        dataprocessing->getischosen()=true;
        QMessageBox::information(this,"success","已被服务员"+dataprocessing->getwaiter_name()+"成功认领");
        ui->labelWaiterInfo->setText("被 "+dataprocessing->getwaiter_name()+" 认领");
    }
}

/*
 * 获取已点菜品列表请求
 */
void MainWindow::slot_getdishstaterequest() {
    QByteArray msg;
    msg.append(TOKEN_GUEST_GETORDERLIST);
    tcpSocket->write(msg,1);
}

/*
 * 收到单条消息
 */
void MainWindow::slot_singlemessage(QString msg) {
    ui->listMsg->addItem(msg);
}

/*
 * 收到消息队列
 */
void MainWindow::slot_msgqueue(QList<QString> list) {
    for(int i=0;i<list.count();i++)
        ui->listMsg->addItem(list.at(i));
}

/*
 * 顾客给服务员发消息
 */
void MainWindow::slot_btnMsg() {
    if(dataprocessing->getwaiter_name()=="") {
        QMessageBox::information(this,"Error","还未被服务员认领，无法发送消息");
        return;
    }
    if(ui->lineEditMsg->text()=="")
        return;
    if(ui->lineEditMsg->text().toUtf8().length()>255) {
        QMessageBox::information(this,"Error","消息过长，请分开发送");
        return;
    }
    QByteArray msg,tmp;
    tmp=ui->lineEditMsg->text().toUtf8();
    msg.append(TOKEN_GUEST_MSGTOWAITER);
    msg.append((unsigned char)(tmp.length()/256));
    msg.append((unsigned char)(tmp.length()%256));
    msg.append(tmp);
    ui->listMsg->addItem("[发送消息]"+ui->lineEditMsg->text());
    ui->lineEditMsg->setText("");
    tcpSocket->write(msg,msg.length());
}

/*
 * 发送获取消息队列请求
 */
void MainWindow::slot_msgqueue_request() {
    QByteArray msg;
    msg.append(TOKEN_GUEST_MSGQUEUE);
    tcpSocket->write(msg);
}

/*
 * 顾客评价服务员
 */
void MainWindow::EvaluateWaiter(QString username,QString comment,int score) {
    QByteArray ans;
    ans.append(TOKEN_GUEST_EVALUATEWAITER);
    QByteArray tmp=username.toUtf8();
    ans.append((unsigned char)tmp.length());
    ans.append(tmp);
    tmp=comment.toUtf8();
    ans.append((unsigned char)(tmp.length()/256));
    ans.append((unsigned char)(tmp.length()%256));
    ans.append(tmp);
    ans.append((unsigned char)score);
    tcpSocket->write(ans,ans.length());
}

/*
 * 顾客更改账户信息
 */
void MainWindow::slot_btnChangeAccount() {
    if(ui->lineEditChangeNew->text()=="") {
        QMessageBox::information(this,"Error","新用户名不能为空！");
        return;
    }
    if(ui->lineEditChangeNew->text().toUtf8().length()>255) {
        QMessageBox::information(this,"Error","新用户名过长!");
        return;
    }
    if(ui->lineEditChangeNewpw->text().length()<6) {
        QMessageBox::information(this,"Error","新密码至少6位！");
        return;
    }
    if(ui->lineEditChangeNewpw->text().toUtf8().length()>255) {
        QMessageBox::information(this,"Error","新密码过长!");
        return;
    }
    if(ui->lineEditChangeOldpw->text()!=g_password) {
        QMessageBox::information(this,"Error","旧密码错误！");
        return;
    }
    if(ui->lineEditChangeConfirm->text()!=ui->lineEditChangeNewpw->text()) {
        QMessageBox::information(this,"Error","请两次输入相同的密码！");
        return;
    }
    QByteArray ans;
    ans.append(TOKEN_GUEST_CHANGEACCOUNT);
    QByteArray tmp;
    tmp=g_phonenumber.toUtf8();
    ans.append((unsigned char)tmp.length());
    ans.append(tmp);
    tmp=ui->lineEditChangeNew->text().toUtf8();
    ans.append((unsigned char)tmp.length());
    ans.append(tmp);
    tmp=ui->lineEditChangeNewpw->text().toUtf8();
    ans.append((unsigned char)tmp.length());
    ans.append(tmp);
    tcpSocket->write(ans,ans.length());
    QMessageBox::information(this,"success","修改账户信息成功！");
    ui->labelUsername->setText("用户名： "+ui->lineEditChangeNew->text());
    g_username=ui->lineEditChangeNew->text();
    ui->lineEditChangeConfirm->setText("");
    ui->lineEditChangeNew->setText("");
    ui->lineEditChangeNewpw->setText("");
    ui->lineEditChangeOldpw->setText("");
}

/*
 * 顾客评价服务员
 */
void MainWindow::btn_EvaluateWaiter() {
    if(evaluate_waiterusername!="") {
        QByteArray ans;
        ans.append(TOKEN_GUEST_EVALUATEWAITER);
        QByteArray tmp=evaluate_waiterusername.toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        tmp=ui->lineEditEvaluateWaiter->text().toUtf8();
        ans.append((unsigned char)(tmp.length()/256));
        ans.append((unsigned char)(tmp.length()%256));
        ans.append(tmp);
        ans.append((unsigned char)ui->spinBoxScoreWaiter->value());
        tcpSocket->write(ans);
        QMessageBox::information(this,"success","评价服务员成功！");
        ui->lineEditEvaluateWaiter->setText("");
        ui->btnEvaluateWaiter->setEnabled(false);
    }
    else
        QMessageBox::critical(this,"Error","未结账或还未被服务员认领，无法评价！");
}

/*
 * 菜单表中的"加一份"按钮
 */
void MainWindow::slot_menuPlusone(int row,int) {
    if(ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(row,3)).toString()=="是") {
        int count=ui->tableViewMenu->model()->data(ui->tableViewMenu->model()->index(row,0)).toInt()+1;
        ui->tableViewMenu->model()->setData(ui->tableViewMenu->model()->index(row,0),count);
        slot_update_menutable();
    }
}

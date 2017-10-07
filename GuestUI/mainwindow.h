/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainwindow.h
 * 摘要：顾客端主界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bglabel.h"
#include "myinputbox.h"
#include <QtNetwork>
#include <QStandardItemModel>
#include "gdataprocessing.h"
#include "spindelegate.h"
//#include "labellist.h"
#include "buttondelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    gDataProcessing *dataprocessing;                //数据处理类
    QStandardItemModel *model,*model2,*model3;      //表格模型，供"菜单"、"餐桌信息"、"已点菜"表格使用
    bool status,islogin;                            //存储是否联网、是否登录的状态
    bool state;                                     //true为登录，false为注册
    int port;                                       //端口
    int mytargetTable;                              //准备选的餐桌编号
    QHostAddress *serverIP;
    QString g_username,g_phonenumber,g_password;    //登录成功以后储存客户的手机号、用户名、密码
    QString evaluate_waiterusername;                //认领自己的服务员用户名
    QTcpSocket *tcpSocket;
    float calc_totalprice();                        //计算所点菜品的总价
    void ready_evaluate();                          //准备评价表格
    QByteArray EncodingEvaluation();                //将所有评价编码
    QByteArray EncodingPayInfo();                   //将结账信息编码
    void EvaluateWaiter(QString,QString,int);       //评价服务员

    //自定义输入框，用于实现登录/注册界面
    myInputBox *ibLoginPhonenumber,*ibLoginPassword,*ibServerIP;
    myInputBox *ibRegisterUsername,*ibRegisterPhonenumber,*ibRegisterPassword,*ibConfirmPassword;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setLoginForm(); //显示登录界面
    void setMainForm(); //显示主界面
    void showLogin();
    void showRegister();
    void switchTab(int,QPushButton *btn=0);
    void tabsLayout();
    void updateLabelList();
    void updateOrderLabelList();

public slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void btn_Login();
    void btn_Register();
    void btn_Choose();
    void btn_Order();
    void btn_Continue();
    void btn_ConfirmEvaluate();
    void btn_PayConfirm();
    void btn_ClearMenuOrderList();
    void btn_EvaluateWaiter();

    void slot_login_success(); //登录成功
    void slot_login_phoneerror(); //登录失败，手机号不存在
    void slot_login_passworderror(); //登录失败，密码错误
    void slot_register_success(); //注册成功
    void slot_register_phoneerror(); //注册失败，手机号已存在
    void slot_getusername(QString); //获取用户名
    void slot_getmenu(); //获取菜单
    void slot_update_menutable(); //更新菜单表，高亮显示被点的菜品
    void slot_update_ordertable(); //更新点菜表，删除数量置为0的菜品
    void slot_gettableinfo(); //收到餐桌信息
    void slot_order_done(); //点菜完成
    void slot_choose_success(); //选桌成功
    void slot_choose_failure(); //选桌失败
    void slot_pay_done(); //完成结账
    void slot_getdishstate(QList<SingleDishOrdered>&); //获取已点菜品状态
    void slot_getdishstaterequest(); //获取已点菜品状态请求
    void slot_singlemessage(QString);
    void slot_msgqueue(QList<QString>);
    void slot_btnMsg();
    void slot_msgqueue_request();
    void slot_btnChangeAccount();
    void slot_menuPlusone(int,int);

private:
    Ui::MainWindow *ui;
    BGLabel *bgLabel,*bgLabel2;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void slot_toRegister();
    void slot_toLogin();
    void slot_closeWindow();
    void slot_miniWindow();

    //用于主界面左侧的的切换按钮
    void slot_tab1();
    void slot_tab2();
    void slot_tab3();
    void slot_tab4();
    void slot_tab5();
    void slot_tab6();
};

#endif // MAINWINDOW_H

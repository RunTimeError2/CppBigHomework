/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：loginwindow.h
 * 摘要：登录界面头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "bglabel.h"
#include "myinputbox.h"

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::LoginWindow *ui;
    QMainWindow *target; //由于MainWindow类继承了QMainWindow,可以用于打开主窗口

    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    void setUpUI();

public slots:
    void login();
    void slot_close();
    void slot_minimize();

private:
    QString username,password; //管理员账户
    BGLabel *bgLabel;
    myInputBox *ibUsername,*ibPassword;
};

#endif // LOGINWINDOW_H

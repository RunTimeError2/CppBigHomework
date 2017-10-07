/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：main.cpp
 * 摘要：主函数
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "loginwindow.h"
#include <QPushButton>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainwindow; //加载mainwindow的同时已经连接了数据库文件
    LoginWindow loginwindow;
    loginwindow.target=&mainwindow;
    loginwindow.show();

    return a.exec();
}

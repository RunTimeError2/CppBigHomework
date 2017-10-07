/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：main.cpp
 * 摘要：服务器端主函数
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainwindow.h"
#include "login.h"
#include "dataprocessing.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

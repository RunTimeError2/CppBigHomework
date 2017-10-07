/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：myinputbox.h
 * 摘要：自定义输入框头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef MYINPUTBOX_H
#define MYINPUTBOX_H

#include <QObject>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class myInputBox : public QFrame
{
    Q_OBJECT

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *clearButton;
    int left;
    int margin;

public:
    explicit myInputBox(QWidget *parent=0);
    ~myInputBox();
    void setLabelText(QString); //设置前面的提示信息
    void setText(QString); //设置文本框中文本
    void setLeft(int); //设置文本框左端与控件左端的距离
    void setMargin(int); //设置文本框边框与控件边框之间距离
    QString getText(); //获取文本框中文本
    void update(); //在控件大小变化后，自动调整各子控件的布局
    void setPasswordMode(bool); //设置是否是密码输入框
    void setFont(QFont&); //为所有子控件设置字体

private slots:
    void clearAll();
};

#endif // MYINPUTBOX_H

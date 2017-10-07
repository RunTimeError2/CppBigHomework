/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：myinputbox.cpp
 * 摘要：自定义输入框
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "myinputbox.h"

myInputBox::myInputBox(QWidget *parent)
    :QFrame(parent)
{
    margin=2;
    left=75;

    label=new QLabel(this);
    label->setGeometry(0,0,this->width(),this->height());
    lineEdit=new QLineEdit(label);
    lineEdit->setGeometry(left,margin,this->width()-left-margin,this->height()-margin*2);
    clearButton=new QPushButton(label);
    clearButton->setText("×");
    clearButton->setGeometry(this->width()-2-margin-lineEdit->height(),margin+1,lineEdit->height()-2,lineEdit->height()-2);
    clearButton->setFlat(true);

    connect(clearButton,SIGNAL(clicked(bool)),this,SLOT(clearAll()));
}

myInputBox::~myInputBox() {

}

void myInputBox::setLabelText(QString text) {
    label->setText(text);
}

void myInputBox::setText(QString text) {
    lineEdit->setText(text);
}

QString myInputBox::getText() {
    return lineEdit->text();
}

void myInputBox::setLeft(int l) {
    left=l;
}

void myInputBox::update() {
    label->setGeometry(0,0,this->width(),this->height());
    lineEdit->setGeometry(left,margin,this->width()-left-margin,this->height()-margin*2);
    clearButton->setGeometry(this->width()-2-margin-lineEdit->height(),margin+1,lineEdit->height()-2,lineEdit->height()-2);
}

void myInputBox::clearAll() {
    lineEdit->setText("");
}

void myInputBox::setPasswordMode(bool flag) {
    if(flag)
        lineEdit->setEchoMode(QLineEdit::Password);
    else
        lineEdit->setEchoMode(QLineEdit::Normal);
}

void myInputBox::setFont(QFont &font) {
    label->setFont(font);
    lineEdit->setFont(font);
}

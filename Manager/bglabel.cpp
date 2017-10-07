/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：bglabel.cpp
 * 摘要：自定义窗口
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "bglabel.h"
#include <QMessageBox>

BGLabel::BGLabel(const QString &text,QWidget *parent,QMainWindow *parentWin)
    :QLabel(parent)
{
    setText(text);
    parentWindow=parentWin;
    ispressed=false;
    titleBarHeight=-1;
}

BGLabel::~BGLabel() {

}

void BGLabel::mousePressEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton && event->pos().y()<=titleBarHeight) {
        ispressed=true;
        currentX=event->pos().x();
        currentY=event->pos().y();
    }
}


void BGLabel::mouseMoveEvent(QMouseEvent *event) {
    if(ispressed) {
        int deltaX=event->pos().x()-currentX;
        int deltaY=event->pos().y()-currentY;
        if(parentWindow)
            parentWindow->move(parentWindow->x()+deltaX,parentWindow->y()+deltaY);
    }
}

void BGLabel::mouseReleaseEvent(QMouseEvent *) {
    ispressed=false;
}

void BGLabel::setTitleBarHeight(int height) {
    titleBarHeight=height;
}

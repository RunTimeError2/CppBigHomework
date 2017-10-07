/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：bglabel.h
 * 摘要：自定义窗口实现头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef BGLABEL_H
#define BGLABEL_H

#include <QWidget>
#include <QLabel>
#include <Qt>
#include <QMainWindow>
#include <QMouseEvent>

/*
 * 从QLabel类派生
 * 实现一个可以显示图片，可以拖动（移动主窗体）的控件作为窗体背景
 * 从而实现无边框窗口
 */
class BGLabel : public QLabel {
    Q_OBJECT

public:
    explicit BGLabel(const QString &text="",QWidget *parent=0,QMainWindow *parentWin=0);
    ~BGLabel();
    QMainWindow *parentWindow;      //该控件所在主窗体，是将被拖动的对象
    void setTitleBarHeight(int);    //设置标题栏，也就是可拖动区域的高度

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

private:
    bool ispressed;
    int currentX,currentY;
    int titleBarHeight;             //设置最顶端标题栏的高度，鼠标只有在这个高度之上才可以拖动窗体
};

#endif // BGLABEL_H

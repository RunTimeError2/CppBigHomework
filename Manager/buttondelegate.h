/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：buttondelegate.h
 * 摘要：tableview按钮代理头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QItemDelegate>

class ButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void buttonClicked(int,int) const; //在某个按钮被点击时会发出信号，参数为按钮所在行，列

public slots:

private:
    QMap<QModelIndex, QStyleOptionButton*> m_btns;

};

#endif // BUTTONDELEGATE_H

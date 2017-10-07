/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：spindelegate.h
 * 摘要：tableview的spinbox代理头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef SPINDELEGATE_H
#define SPINDELEGATE_H

#include <QItemDelegate>

class SpinDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    int rangeMode;
    SpinDelegate(QObject *parent=0,int _rangeMode=0);
    QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor,const QModelIndex &index) const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index) const;
};

#endif // SPINDELEGATE_H

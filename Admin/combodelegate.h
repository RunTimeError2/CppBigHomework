/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：combodelegate.h
 * 摘要：tableview组合框代理头文件
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

#include <QItemDelegate>

class ComboDelegate : public QItemDelegate
{
public:
    int ComboType; //指示该ComboBox用于哪一个表，决定ComboBox中添加哪些项
    ComboDelegate(QObject *parent=0,int type=1);
    QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index)const;
    void setEditorData(QWidget *editor,const QModelIndex &index)const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index)const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index)const;
};

#endif // COMBODELEGATE_H

/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：spindelegate.cpp
 * 摘要：tableview的spinbox代理
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "spindelegate.h"
#include <QSpinBox>

SpinDelegate::SpinDelegate(QObject *parent,int _mode):
    QItemDelegate(parent)
{
    mode=_mode;
}

QWidget *SpinDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const {
    QSpinBox *editor=new QSpinBox(parent);
    editor->setRange(0,100000);
    if(mode==1)
        editor->setRange(0,1); //是否有售
    editor->installEventFilter(const_cast<SpinDelegate*>(this));
    return editor;
}

void SpinDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const {
    int value=index.model()->data(index).toInt();
    QSpinBox *box=static_cast<QSpinBox*>(editor);
    box->setValue(value);
}

void SpinDelegate::setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const {
    QSpinBox *box=static_cast<QSpinBox*>(editor);
    int value=box->value();
    if(mode!=2)
        model->setData(index,value);
}

void SpinDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}

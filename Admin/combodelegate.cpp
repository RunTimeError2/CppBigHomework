/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：combodelegate.cpp
 * 摘要：tableview组合框代理
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "combodelegate.h"
#include <QComboBox>

ComboDelegate::ComboDelegate(QObject *parent,int type):
    QItemDelegate(parent)
{
    ComboType=type;
}

QWidget *ComboDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const {
    QComboBox *editor=new QComboBox(parent);
    if(ComboType==1) {
        editor->addItem("服务员");
        editor->addItem("厨师");
        editor->addItem("经理");
    }
    if(ComboType==2) {
        editor->addItem("冷菜");
        editor->addItem("热菜");
        editor->addItem("汤");
        editor->addItem("主食");
        editor->addItem("饮料");
    }
    editor->installEventFilter(const_cast<ComboDelegate*>(this));
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const {
    QString str=index.model()->data(index).toString();
    QComboBox *box=static_cast<QComboBox*>(editor);
    int i=box->findText(str);
    box->setCurrentIndex(i);
}

void ComboDelegate::setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const {
    QComboBox *box=static_cast<QComboBox*>(editor);
    QString str=box->currentText();
    model->setData(index,str);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const {
    editor->setGeometry(option.rect);
}

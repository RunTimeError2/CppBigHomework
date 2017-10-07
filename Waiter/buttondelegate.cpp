/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：buttondelegate.cpp
 * 摘要：tableview按钮代理
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "buttondelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QMessageBox>

ButtonDelegate::ButtonDelegate(QObject *parent,QString _buttonText) :
    QItemDelegate(parent)
{
    buttonText=_buttonText;
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton* button = m_btns.value(index);
    if (!button) {
        button = new QStyleOptionButton();
        button->rect = option.rect.adjusted(4, 4, -4, -4);
        button->text = buttonText;
        button->state |= QStyle::State_Enabled;

        (const_cast<ButtonDelegate *>(this))->m_btns.insert(index, button);
    }
    button->rect=option.rect.adjusted(4,4,-4,-4);
    painter->save();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);


}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* e =(QMouseEvent*)event;
        if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
            m_btns.value(index)->state |= QStyle::State_Sunken;
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;
        if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
            m_btns.value(index)->state &= (~QStyle::State_Sunken);

            emit buttonClicked(index.row(),index.column());
        }
    }
}

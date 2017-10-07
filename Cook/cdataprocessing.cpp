/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：cdataprocessing.cpp
 * 摘要：厨师端数据处理模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "cdataprocessing.h"
#include <QMessageBox>

cDataProcessing::cDataProcessing()
{
    dishAvailable.clear();
    dishSelected.clear();
}

void cDataProcessing::ProcessingData(QByteArray bData) {
    unsigned char Token=bData.at(0);
    switch(Token) {
        case TOKEN_COOK_LOGIN: {
            /*
             * 处理登录反馈信息
             */
            switch(bData.at(1)) {
                case 1:
                    emit login_success();
                    break;
                case 0:
                    emit login_passworderror();
                    break;
                case -1:
                    emit login_usernameerror();
                    break;
            }
            break;
        }
        case TOKEN_COOK_REGISTER: {
            /*
             * 处理注册反馈信息
             */
            if(bData.at(1)==1)
                emit register_success();
            else
                emit register_usernameerror();
            break;
        }
        case TOKEN_COOK_SELECTDISH: {
            /*
             * 厨师认领菜品
             * 由厨师端发出，不会收到
             */
            break;
        }
        case TOKEN_COOK_GETUNSTARTED: {
            /*
             * 接收到可认领菜品列表
             */
            dishAvailable.clear();
            int count=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
            int len,pos=3,index;
            QString tmp;
            for(int i=0;i<count;i++) {
                len=bData.at(pos++);
                tmp=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                index=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
                pos+=2;
                SingleDishOrdered _dish;
                _dish.name=tmp;
                _dish.state=DISH_UNCHOSEN;
                _dish.index=index;
                dishAvailable.append(_dish);
            }
            emit cook_getunstarted(dishAvailable);
            break;
        }
        case TOKEN_COOK_GETSELECTED: {
            /*
             * 接收自己认领的菜品列表
             */
            dishSelected.clear();
            int count=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
            QString tmp;
            int len,pos=3;
            for(int i=0;i<count;i++) {
                SingleDishOrdered _dish;
                len=bData.at(pos++);
                tmp=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                _dish.name=tmp;
                _dish.state=bData.at(pos++);
                _dish.index=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
                pos+=2;
                dishSelected.append(_dish);
            }
            emit cook_getselected(dishSelected);
            break;
        }
        case TOKEN_COOK_GETALLINFO: {
            /*
             * 获取厨师所需所有信息
             */
            dishAvailable.clear();
            int count=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
            int len,pos=3,index;
            QString tmp;
            for(int i=0;i<count;i++) {
                len=bData.at(pos++);
                tmp=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                index=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
                pos+=2;
                SingleDishOrdered _dish;
                _dish.name=tmp;
                _dish.state=DISH_UNCHOSEN;
                _dish.index=index;
                dishAvailable.append(_dish);
            }

            dishSelected.clear();
            count=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
            pos+=2;
            for(int i=0;i<count;i++) {
                SingleDishOrdered _dish;
                len=bData.at(pos++);
                tmp=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                _dish.name=tmp;
                _dish.state=bData.at(pos++);
                _dish.index=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
                pos+=2;
                dishSelected.append(_dish);
            }

            emit cook_getunstarted(dishAvailable);
            emit cook_getselected(dishSelected);
            break;
        }
        case TOKEN_COOK_SETDISHSTATE: {
            /*
             * 厨师设置菜品状态
             * 由厨师端发出，不会收到
             */
            break;
        }
    }
}

/*
 * 编码厨师登陆信息
 */
QByteArray cDataProcessing::EncodingCookLogin(QString username,QString password) {
    QByteArray ans;
    ans.append(TOKEN_COOK_LOGIN);
    QByteArray un=username.toUtf8();
    ans.append((unsigned char)un.length());
    ans.append(un);
    QByteArray pw=password.toUtf8();
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

/*
 * 编码厨师注册信息
 */
QByteArray cDataProcessing::EncodingCookRegister(QString username,QString password) {
    QByteArray ans,un,pw;
    ans.append(TOKEN_COOK_REGISTER);
    un=username.toUtf8();
    pw=password.toUtf8();
    ans.append((unsigned char)un.length());
    ans.append(un);
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

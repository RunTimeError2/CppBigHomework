/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：wdataprocessing.cpp
 * 摘要：服务员端数据处理模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "wdataprocessing.h"
#include <QMessageBox>

wDataProcessing::wDataProcessing()
{

}

void wDataProcessing::ProcessingData(QByteArray bData) {
    unsigned char Token=bData.at(0);
    switch(Token) {
        case TOKEN_WAITER_LOGIN: {
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
        case TOKEN_WAITER_REGISTER: {
            if(bData.at(1)==1)
                emit register_success();
            else
                emit register_usernameerror();
            break;
        }
        case TOKEN_WAITER_GETALLINFO: {
            DecodingAllInfo(bData);
            emit waiter_getallinfo();
            break;
        }
        case TOKEN_WAITER_SINGLEMESSAGE: {
            QString msg=DecodingSingleMessage(bData);
            emit waiter_singlemessage(msg);
            break;
        }
    }
}

/*
 * 编码服务员登陆信息
 */
QByteArray wDataProcessing::EncodingWaiterLogin(QString username,QString password) {
    QByteArray ans;
    ans.append(TOKEN_WAITER_LOGIN);
    QByteArray un=username.toUtf8();
    ans.append((unsigned char)un.length());
    ans.append(un);
    QByteArray pw=password.toUtf8();
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

/*
 * 编码服务员注册信息
 */
QByteArray wDataProcessing::EncodingWaiterRegister(QString username,QString password) {
    QByteArray ans,un,pw;
    ans.append(TOKEN_WAITER_REGISTER);
    un=username.toUtf8();
    pw=password.toUtf8();
    ans.append((unsigned char)un.length());
    ans.append(un);
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

void wDataProcessing::DecodingAllInfo(QByteArray bData) {
    int pos=1;

    //餐桌信息
    int tablecount=((unsigned char)bData.at(pos))*256+((unsigned char)bData.at(pos+1));
    pos+=2;
    tableInfo.clear();
    for(int i=0;i<tablecount;i++) {
        SingleTable _table;
        QString tmp;
        int len=bData.at(pos++);
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _table.type=tmp;
        _table.fitin=bData.at(pos++);
        _table.dishcount=0;
        if(bData.at(pos)==1) {
            _table.isValid=true;
            _table.isSelected=false;
        }
        else
            if(bData.at(pos)==2) {
                _table.isValid=false;
                _table.isSelected=false;
            }
            else {
                _table.isValid=false;
                _table.isSelected=true;
            }
        pos++;
        if(bData.at(pos)==1) {
            _table.dishcount=bData.at(pos+1);
            pos++;
        }
        pos++;
        _table.isMine=false;
        tableInfo.append(_table);
    }

    //菜品信息
    dishInfo.clear();
    int selectedtablecount=(unsigned char)bData.at(pos++);
    for(int i=0;i<selectedtablecount;i++) {
        int tableindex=bData.at(pos++);
        if(tableindex>=0 && tableindex<tableInfo.count())
            tableInfo[tableindex].isMine=true;
        int dishcount=((unsigned char)bData.at(pos))*256+((unsigned char)bData.at(pos+1));
        pos+=2;
        for(int j=0;j<dishcount;j++) {
            SingleDish _dish;
            _dish.index=((unsigned char)bData.at(pos))*256+((unsigned char)bData.at(pos+1));
            pos+=2;
            QString tmp;
            int len=bData.at(pos++);
            tmp=tmp.fromUtf8(bData.mid(pos,len));
            pos+=len;
            _dish.name=tmp;
            _dish.state=bData.at(pos++);
            _dish.tableindex=tableindex;
            dishInfo.append(_dish);
        }
    }

    //消息
    tmp_msg.clear();
    int msgcount=((unsigned char)bData.at(pos))*256+((unsigned char)bData.at(pos+1));
    pos+=2;
    for(int i=0;i<msgcount;i++) {
        int len=bData.at(pos++);
        QString tmp_m;
        tmp_m=tmp_m.fromUtf8(bData.mid(pos,len));
        pos+=len;
        QString tmp_p;
        len=bData.at(pos++);
        tmp_p=tmp_p.fromUtf8(bData.mid(pos,len));
        pos+=len;
        int sourcetable=bData.at(pos++);
        QString _msg=QString("[来自%1号桌顾客的消息]%2").arg(sourcetable+1).arg(tmp_m); //====================================
        tmp_msg.append(_msg);
    }

    emit waiter_getallinfo();
}

/*
 * 解码单条消息
 */
QString wDataProcessing::DecodingSingleMessage(QByteArray bData) {
    QString ans;
    int tableindex,len;
    tableindex=((unsigned char)bData.at(1))*256+(unsigned char)bData.at(2);
    len=((unsigned char)bData.at(3))*256+(unsigned char)bData.at(4);
    QString msg;
    msg=msg.fromUtf8(bData.mid(5,len));
    ans=QString("[来自%1号桌顾客的消息]%2").arg(tableindex+1).arg(msg); //====================================
    return ans;
}

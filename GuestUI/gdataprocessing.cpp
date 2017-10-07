/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：gdataprocessing.cpp
 * 摘要：顾客端数据处理模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "gdataprocessing.h"
#include <QMessageBox>

gDataProcessing::gDataProcessing()
{
    myTable=-1;
    myPrice=0;
    waiter_name="";
    ischosen=false;
}

void gDataProcessing::ProcessingData(QByteArray bData) {
    unsigned char Token=bData.at(0);
    switch(Token) {
        case TOKEN_GUEST_LOGIN: {
            switch(bData.at(1)) {
                case 1:
                    emit login_success();
                    break;
                case 0:
                    emit login_passworderror();
                    break;
                case -1:
                    emit login_phoneerror();
                    break;
            }
            break;
        }
        case TOKEN_GUEST_REGISTER: {
            if(bData.at(1)==1)
                emit register_success();
            else
                emit register_phoneerror();
            break;
        }
        case TOKEN_GUEST_GETUSERNAME: {
            int len=bData.at(1);
            QString uname;
            uname=uname.fromUtf8(bData.mid(2,len));
            emit getusername(uname);
            break;
        }
        case TOKEN_GUEST_GETMENU: {
            Menu=DecodingMenu(bData);
            emit getmenu();
            break;
        }
        case TOKEN_GUEST_SENDTABLE: {
            DecodingTableInfo(bData);
            emit gettableinfo();
            break;
        }
        case TOKEN_GUEST_CHOOSESUCCESS: {
            emit choose_success();
            break;
        }
        case TOKEN_GUEST_CHOOSEFAILURE: {
            emit choose_fail();
            break;
        }
        case TOKEN_GUEST_PAY: {
            emit pay_done();
            break;
        }
        case TOKEN_GUEST_GETORDERLIST: {
            DecodingOrderList(bData);
            emit get_dishstate(dishorderlist);
            break;
        }
        case TOKEN_GUEST_SINGLEMESSAGE: {
            QString msg=DecodingSingleMessage(bData);
            emit guest_singlemessage(msg);
            break;
        }
        case TOKEN_GUEST_MSGQUEUE: {
            QList<QString> list;
            list=DecodingMsgQueue(bData);
            emit guest_msgqueue(list);
            break;
        }
    }
}

/*
 * 将菜单信息解码
 */
menulist gDataProcessing::DecodingMenu(QByteArray bData) {
    int len=(int)(bData.at(1)*256+bData.at(2));
    menulist ans;
    ans.num=len;
    ans.list=new singledish[len];
    int pos=3;
    for(int i=0;i<len;i++) {
        int s_len=bData[pos++];
        QByteArray b_name=bData.mid(pos,s_len);
        pos+=s_len;
        ans.list[i].name=ans.list[i].name.fromUtf8(b_name); //名称
        float price100=(float)((unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1)));
        ans.list[i].price=price100/100.0; //价格
        pos+=2;
        ans.list[i].isvalid=(int)bData[pos++];
        ans.list[i].evalutionnum=(int)(bData[pos]*256+bData[pos+1]);
        pos+=2;
        ans.list[i].score=((float)bData[pos++])/10.0;
        int t_len=bData[pos++];
        QString tmp;
        ans.list[i].type=tmp.fromUtf8(bData.mid(pos,t_len));
        pos+=t_len;
    }
    int len_un=bData.at(pos++);
    QString tmp;
    tmp=tmp.fromUtf8(bData.mid(pos,len_un));
    pos+=len_un;
    my_username=tmp;

    return ans;
}

/*
 * 将餐桌信息解码
 */
void gDataProcessing::DecodingTableInfo(QByteArray bData) {
    myTable=-1;
    int pos=1;
    Table.count=((unsigned char)bData.at(1))*256+(unsigned char)bData.at(2);
    pos+=2;
    Table.valid=new int[Table.count];
    Table.numlist=new int[Table.count];
    Table.typelist=new QString[Table.count];
    QString s_tmp;
    for(int i=0;i<Table.count;i++) {
        int len=bData.at(pos++);
        Table.typelist[i]=s_tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        Table.numlist[i]=bData.at(pos++);
        if(bData.at(pos)==2)
            myTable=i;
        Table.valid[i]=(bData.at(pos)==1);
        pos++;
    }

    if((unsigned char)(bData.at(pos))!=254) {
        pos++;
        int len=bData.at(pos++);
        waiter_name=waiter_name.fromUtf8(bData.mid(pos,len));
        pos+=len;
    }
}

/*
 * 编码顾客登陆信息
 */
QByteArray gDataProcessing::EncodingGuestLogin(QString phonenumber,QString password) {
    QByteArray ans;
    ans.append(TOKEN_GUEST_LOGIN);
    ans.append(phonenumber.toUtf8());
    QByteArray pw=password.toUtf8();
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

/*
 * 编码顾客注册信息
 */
QByteArray gDataProcessing::EncodingGuestRegister(QString phonenumber,QString username,QString password) {
    QByteArray ans,un,pw;
    ans.append(TOKEN_GUEST_REGISTER);
    ans.append(phonenumber.toUtf8());
    un=username.toUtf8();
    pw=password.toUtf8();
    ans.append((unsigned char)un.length());
    ans.append(un);
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

/*
 * 编码顾客下单信息
 */
QByteArray gDataProcessing::EncodingGuestOrder(orderList list) {
    QByteArray ans;
    ans.append(TOKEN_GUEST_ORDERFOOD);
    ans.append((unsigned char)list.count);
    ans.append(list.phonenumber.toUtf8());
    for(int i=0;i<list.count;i++) {
        QByteArray tmp=list.list[i].name.toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        ans.append((unsigned char)list.list[i].num);
    }
    return ans;
}

/*
 * 解码菜品状态信息
 */
void gDataProcessing::DecodingOrderList(QByteArray msg) {
    dishorderlist.clear();
    int count=(unsigned char)(msg.at(1))*256+(unsigned char)(msg.at(2));
    int pos=3,len,index;
    for(int i=0;i<count;i++) {
        SingleDishOrdered _dish;
        QString tmp;
        len=msg.at(pos++);
        tmp=tmp.fromUtf8(msg.mid(pos,len));
        pos+=len;
        _dish.name=tmp;
        index=(unsigned char)(msg.at(pos))*256+(unsigned char)(msg.at(pos+1));
        pos+=2;
        _dish.index=index;
        _dish.state=msg.at(pos++);
        dishorderlist.append(_dish);
    }
    int p=0;
    p+=((unsigned char)msg.at(pos++))*65536;
    p+=((unsigned char)msg.at(pos++))*256;
    p+=((unsigned char)msg.at(pos++));
    myPrice=(float)p/100.0;

    int len_un=msg.at(pos++);
    if(len_un!=254)
        waiter_name=waiter_name.fromUtf8(msg.mid(pos,len_un));
}

/*
 * 解码单条消息
 */
QString gDataProcessing::DecodingSingleMessage(QByteArray bData) {
    QString ans,tmp_un,tmp_msg;
    int len_un,len_msg;
    int pos;
    len_un=bData.at(1);
    pos=2;
    tmp_un=tmp_un.fromUtf8(bData.mid(pos,len_un));
    pos+=len_un;
    len_msg=((unsigned char)bData.at(pos))*256+(unsigned char)bData.at(pos+1);
    pos+=2;
    tmp_msg=tmp_msg.fromUtf8(bData.mid(pos,len_msg));
    ans=QString("[来自服务员%1的消息]%2").arg(tmp_un).arg(tmp_msg);
    return ans;
}

/*
 * 解码顾客消息队列
 */
QList<QString> gDataProcessing::DecodingMsgQueue(QByteArray bData) {
    QList<QString> ans;
    int pos=2;
    int count=bData.at(1);
    for(int i=0;i<count;i++) {
        QString tmp_msg,tmp_un;
        int len;
        len=((unsigned char)bData.at(pos))*256+(unsigned char)bData.at(pos+1);
        pos+=2;
        tmp_msg=tmp_msg.fromUtf8(bData.mid(pos,len));
        pos+=len;
        len=(unsigned char)bData.at(pos++);
        tmp_un=tmp_un.fromUtf8(bData.mid(pos,len));
        pos+=len;
        QString tmp=QString("[来自服务员%1的消息]%2").arg(tmp_un).arg(tmp_msg);
        ans.append(tmp);
    }
    return ans;
}

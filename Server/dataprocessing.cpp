/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：dataprocessing.cpp
 * 摘要：服务器端数据处理模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "dataprocessing.h"
#include <QMessageBox>

void dataProcessing::ProcessData(QByteArray bData) {
    int Token=bData.at(0);
    QString phonenumber,password,username;
    unsigned char unLen,pwLen;
    int pos;
    QString tmp;
    switch(Token) {
        case TOKEN_GUEST_LOGIN: {
            /* 数据格式：
             * 第0位为Token，第1位~第11位为手机号编码，第12位为密码编码长度
             * 之后为密码的Utf8编码
             */
            phonenumber=phonenumber.fromUtf8(bData.mid(1,11));
            pwLen=bData.at(12);
            password=password.fromUtf8(bData.mid(13,pwLen));
            emit guest_login(phonenumber,password);
            break;
        }
        case TOKEN_GUEST_REGISTER: {
            /*  数据格式：
             * 第0位为Token，第1位~第11位为手机号编码
             * 第12位为用户名编码长度，之后为用户名的Utf8编码
             * 之后一位为密码编码长度，之后为密码的Utf8编码
             */
            phonenumber=phonenumber.fromUtf8(bData.mid(1,11));
            unLen=bData.at(12);
            username=username.fromUtf8(bData.mid(13,unLen));
            pwLen=bData.at(13+unLen);
            password=password.fromUtf8(bData.mid(14+unLen,pwLen));
            emit guest_register(phonenumber,username,password);
            break;
        }
        /*case TOKEN_GUEST_GETUSERNAME: {
            //获取用户名请求，无数据
            emit guest_getusername();
            break;
        }*/
        case TOKEN_GUEST_GETMENU: {
            //获取菜单请求，无数据
            emit guest_getmenu();
            break;
        }
        case TOKEN_GUEST_ORDERFOOD: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为点的菜品总数,之后11位为顾客手机号
             * 为防止在点菜时菜单更新，数据中直接包含点的菜名以及数量
             * 格式为：1位表示菜名编码长度，之后为菜名编码，之后1位为数量
             */
            dataStructure::orderList ans_o;
            ans_o.count=(int)bData.at(1);
            ans_o.phonenumber=tmp.fromUtf8(bData.mid(2,11));
            pos=13;
            ans_o.list=new dataStructure::orderList::dish[ans_o.count];
            for(int i=0;i<ans_o.count;i++) {
                int len=bData.at(pos++);
                ans_o.list[i].name=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                ans_o.list[i].num=(int)bData.at(pos++);
            }
            emit guest_orderfood(ans_o);
            break;
        }
        case TOKEN_GUEST_EVALUATE: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为评价数量,之后11位为顾客手机号
             * 之后为菜名编码长度(1位)，菜名编码，评价编码长度(1位)，评价编码，评分
             */
            dataStructure::evaluateList ans_e;
            ans_e.count=bData.at(1);
            pos=13;
            ans_e.phonenumber=tmp.fromUtf8(bData.mid(2,11));
            ans_e.list=new dataStructure::evaluateList::evaluation[ans_e.count];
            for(int i=0;i<ans_e.count;i++) {
                int len=bData.at(pos++);
                ans_e.list[i].dishname=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                len=bData.at(pos++);
                ans_e.list[i].comment=tmp.fromUtf8(bData.mid(pos,len));
                pos+=len;
                ans_e.list[i].score=bData.at(pos++);
            }
            emit guest_evaluate(ans_e);
            break;
        }
        /*case TOKEN_GUEST_PAY: {
            /*
             * 数据格式：
             * 结果是一个float，精确到两位小数
             * 为发送方便，将其乘100后所得整数用3字节表示
             * 之后11位为顾客手机号
             *
            float ans_p=(float)(((unsigned char)bData.at(1))*256*256+((unsigned char)bData.at(2))*256+((unsigned char)bData.at(3)))/100.0;
            QString phonenumber=tmp.fromUtf8(bData.mid(4,11));
            emit guest_pay(ans_p,phonenumber);
            break;
        }*/
        case TOKEN_GUEST_PAYCONFIRM: {
            /*
             * 确认付款信息，无数据
             */
            emit guest_payconfirm();
            break;
        }
        case TOKEN_GUEST_SENDTABLE: {
            /*
             * 获取餐桌信息请求，无数据
             */
            emit guest_sendtable();
            break;
        }
        case TOKEN_GUEST_CHOOSETABLE: {
            /*
             * 顾客选桌，第0位为Token，第1位、第2位为选的桌号，一个客户端仅能选一桌
             */
            int index=((unsigned char)bData.at(1))*256+((unsigned char)bData.at(2));
            emit guest_choosetable(index);
            break;
        }
        case TOKEN_GUEST_GETORDERLIST: {
            /*
             * 获取已点菜品列表，无数据
             */
            emit guest_getorderlist();
            break;
        }
        case TOKEN_GUEST_MSGTOWAITER: {
            /*
             * 数据格式：
             * 第0位为Token，第1，2位为消息编码长度，之后为消息编码
             */
            int len=((unsigned char)bData.at(1))*256+(unsigned char)bData.at(2);
            QString tmp;
            tmp=tmp.fromUtf8(bData.mid(3,len));
            emit guest_msgtowaiter(tmp);
            break;
        }
        case TOKEN_GUEST_MSGQUEUE: {
            /*
             * 获取消息队列请求，无数据
             */
            //QMessageBox::information(NULL,"server","get msgqueue request!"); //============================================================
            emit guest_getmsgqueue();
            break;
        }
        case TOKEN_GUEST_EVALUATEWAITER: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为服务员用户名编码长度，之后为服务员用户名编码
             * 之后2位为评价编码长度，之后为评价编码
             * 之后1位为评分
             */
            QString tmp_un,tmp_comment;
            int score;
            int len;
            int pos=2;
            len=bData.at(1);
            tmp_un=tmp_un.fromUtf8(bData.mid(pos,len));
            pos+=len;
            len=((unsigned char)bData.at(pos))*256+(unsigned char)bData.at(pos+1);
            pos+=2;
            tmp_comment=tmp_comment.fromUtf8(bData.mid(pos,len));
            pos+=len;
            score=(unsigned char)(bData.at(pos));
            emit guest_evaluatewaiter(tmp_un,tmp_comment,score); //=====================================================================
            break;
        }
        case TOKEN_GUEST_CHANGEACCOUNT: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为手机号编码长度，之后为手机号编码
             * 之后1位为用户名编码长度，之后为用户名编码
             * 之后1位为密码编码长度，之后为密码编码
             */
            int len,pos;
            QString tmp_ph,tmp_un,tmp_pw;
            len=bData.at(1);
            pos=2;
            tmp_ph=tmp_ph.fromUtf8(bData.mid(pos,len));
            pos+=len;
            len=bData.at(pos++);
            tmp_un=tmp_un.fromUtf8(bData.mid(pos,len));
            pos+=len;
            len=bData.at(pos++);
            tmp_pw=tmp_pw.fromUtf8(bData.mid(pos,len));
            pos+=len;
            emit guest_changeaccount(tmp_ph,tmp_un,tmp_pw);
            break;
        }
        case TOKEN_COOK_LOGIN: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为用户名编码长度，之后为用户名编码
             * 之后为密码编码长度，之后为密码编码
             */
            unLen=bData.at(1);
            username=username.fromUtf8(bData.mid(2,unLen));
            pwLen=bData.at(2+unLen);
            password=password.fromUtf8(bData.mid(3+unLen,pwLen));
            emit cook_login(username,password);
            break;
        }
        case TOKEN_COOK_REGISTER: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为用户名编码长度，之后为用户名编码
             * 之后为密码编码长度，之后为密码编码
             */
            unLen=bData.at(1);
            username=username.fromUtf8(bData.mid(2,unLen));
            pwLen=bData.at(2+unLen);
            password=password.fromUtf8(bData.mid(3+unLen,pwLen));
            emit cook_register(username,password);
            break;
        }
        case TOKEN_COOK_SELECTDISH: {
            /*
             * 数据格式：
             * 第0位为Token,第1、2位为所选菜品的编号
             */
            int index;
            index=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
            emit cook_selectdish(index);
            break;
        }
        case TOKEN_COOK_GETSELECTED: {
            /*
             * 获取已认领菜品请求，无数据
             */
            emit cook_getselected();
            break;
        }
        case TOKEN_COOK_GETUNSTARTED: {
            /*
             * 获取未认领菜品请求，无数据
             */
            emit cook_getunstarted();
            break;
        }
        case TOKEN_COOK_SETDISHSTATE: {
            /*
             * 数据格式：
             * 第0位为Token，第1、2位为菜品编号
             * 第3为为想要设置的状态
             */
            int index,state;
            index=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
            state=bData.at(3);
            emit cook_setdishstate(index,state);
            break;
        }
        case TOKEN_COOK_GETALLINFO: {
            /*
             * 获取厨师所需信息（可认领菜品，已认领菜品），无数据
             */
            emit cook_getallinfo();
            break;
        }
        case TOKEN_WAITER_LOGIN: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为用户名编码长度，之后为用户名编码
             * 之后为密码编码长度，之后为密码编码
             */
            unLen=bData.at(1);
            username=username.fromUtf8(bData.mid(2,unLen));
            pwLen=bData.at(2+unLen);
            password=password.fromUtf8(bData.mid(3+unLen,pwLen));
            emit waiter_login(username,password);
            break;
        }
        case TOKEN_WAITER_REGISTER: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为用户名编码长度，之后为用户名编码
             * 之后为密码编码长度，之后为密码编码
             */
            unLen=bData.at(1);
            username=username.fromUtf8(bData.mid(2,unLen));
            pwLen=bData.at(2+unLen);
            password=password.fromUtf8(bData.mid(3+unLen,pwLen));
            emit waiter_register(username,password);
            break;
        }
        case TOKEN_WAITER_CHOOSETABLE: {
            /*
             * 数据格式：
             * 第0位为Token，第1、2位为餐桌编号
             */
            int index;
            index=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
            emit waiter_selecttable(index);
            break;
        }
        case TOKEN_WAITER_GETALLINFO: {
            /*
             * 获取所有数据请求，无数据
             */
            emit waiter_getallinfo();
            break;
        }
        case TOKEN_MANAGER_LOGIN: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为用户名编码长度，之后为用户名编码
             * 之后为密码编码长度，之后为密码编码
             */
            unLen=bData.at(1);
            username=username.fromUtf8(bData.mid(2,unLen));
            pwLen=bData.at(2+unLen);
            password=password.fromUtf8(bData.mid(3+unLen,pwLen));
            emit manager_login(username,password);
            break;
        }
        case TOKEN_MANAGER_REGISTER: {
            /*
             * 数据格式：
             * 第0位为Token，第1位为用户名编码长度，之后为用户名编码
             * 之后为密码编码长度，之后为密码编码
             */
            unLen=bData.at(1);
            username=username.fromUtf8(bData.mid(2,unLen));
            pwLen=bData.at(2+unLen);
            password=password.fromUtf8(bData.mid(3+unLen,pwLen));
            emit manager_register(username,password);
            break;
        }
        case TOKEN_WAITER_MSGTOGUEST: {
            /*
             * 数据格式：
             * 第0位为Token，第1，2位为目标餐桌，第3，4位为消息长度，之后为消息编码
             */
            //QMessageBox::information(NULL,"server","decoding msgtoguest!"); //==============================================
            int target=((unsigned char)bData.at(1))*256+(unsigned char)bData.at(2);
            int len=((unsigned char)bData.at(3))*256+(unsigned char)bData.at(4);
            QString tmp;
            tmp=tmp.fromUtf8(bData.mid(5,len));
            emit waiter_msgtoguest(tmp,target);
            break;
        }
        case TOKEN_MANAGER_REPORT: {
            /*
             * 经理获取报表请求，无数据
             */
            emit manager_report();
            break;
        }
        default:
            break;
    }
}

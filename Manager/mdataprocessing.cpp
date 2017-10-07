/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mdataprocessing.cpp
 * 摘要：经理端数据处理模块
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mdataprocessing.h"
#include <QMessageBox>

mDataProcessing::mDataProcessing() {
    report=new managerReport;
}

void mDataProcessing::ProcessingData(QByteArray bData) {
    unsigned char Token=bData.at(0);
    switch(Token) {
        case TOKEN_MANAGER_LOGIN: {
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
        case TOKEN_MANAGER_REGISTER: {
            if(bData.at(1)==1)
                emit register_success();
            else
                emit register_usernameerror();
            break;
        }
        case TOKEN_MANAGER_REPORT: {
            DecodingManagerReport(bData);
            emit manager_getreport();
            break;
        }
    }
}

/*
 * 编码服务员登陆信息
 */
QByteArray mDataProcessing::EncodingManagerLogin(QString username,QString password) {
    QByteArray ans;
    ans.append(TOKEN_MANAGER_LOGIN);
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
QByteArray mDataProcessing::EncodingManagerRegister(QString username,QString password) {
    QByteArray ans,un,pw;
    ans.append(TOKEN_MANAGER_REGISTER);
    un=username.toUtf8();
    pw=password.toUtf8();
    ans.append((unsigned char)un.length());
    ans.append(un);
    ans.append((unsigned char)pw.length());
    ans.append(pw);
    return ans;
}

/*
 * 解码数据报表
 */
void mDataProcessing::DecodingManagerReport(QByteArray bData) {
    report->clearAll();
    int pos;
    int count_waiterWork=(unsigned char)(bData.at(1))*256+(unsigned char)(bData.at(2));
    pos=3;
    for(int i=0;i<count_waiterWork;i++) {
        WorkRecord _record;
        int len=bData.at(pos++);
        QString tmp;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _record.username=tmp;
        int x=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
        pos+=2;
        _record.tmp_record=x;
        x=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
        pos+=2;
        _record.record=x;
        _record.avaerageScore=0;
        _record.evaluationCount=0;
        _record.scoreSum=0;
        report->waiter.append(_record);
        report->waiterMap.insert(_record.username,report->waiter.count()-1);
    }

    int count_cookWork=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
    pos+=2;
    for(int i=0;i<count_cookWork;i++) {
        WorkRecord _record;
        int len=bData.at(pos++);
        QString tmp;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _record.username=tmp;
        int x=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
        pos+=2;
        _record.tmp_record=x;
        x=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
        pos+=2;
        _record.record=x;
        report->cook.append(_record);
        report->cookMap.insert(_record.username,report->cook.count()-1);
    }

    int count_evaluation=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
    pos+=2;
    for(int i=0;i<count_evaluation;i++) {
        SingleWaiterEvaluation _evaluation;
        int len=(unsigned char)(bData.at(pos++));
        QString tmp;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _evaluation.username=tmp;
        len=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
        pos+=2;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _evaluation.comment=tmp;
        _evaluation.score=(unsigned char)bData.at(pos++);
        report->waiterEvaluation.append(_evaluation);

        if(report->waiterMap.contains(_evaluation.username)) {
            int index=report->waiterMap[_evaluation.username];
            if(index>=0 && index<report->waiter.count()) {
                report->waiter[index].evaluationCount++;
                report->waiter[index].scoreSum+=_evaluation.score;
            }
        }
    }

    for(int i=0;i<report->waiter.count();i++)
        if(report->waiter.at(i).evaluationCount>0)
            report->waiter[i].avaerageScore=(float(report->waiter[i].scoreSum))/(float(report->waiter[i].evaluationCount));

    int count_dishevaluation=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
    pos+=2;
    for(int i=0;i<count_dishevaluation;i++) {
        SingleDishEvaluation _dishevaluation;
        int len=(unsigned char)(bData.at(pos++));
        QString tmp;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _dishevaluation.phonenumber=tmp;
        len=(unsigned char)(bData.at(pos++));
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _dishevaluation.dishname=tmp;
        len=(unsigned char)(bData.at(pos))*256+(unsigned char)bData.at(pos+1);
        pos+=2;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _dishevaluation.comment=tmp;
        _dishevaluation.score=(unsigned char)bData.at(pos++);
        report->dishEvaluation.append(_dishevaluation);
    }

    int count_payrecord=(unsigned char)(bData.at(pos))*256+(unsigned char)(bData.at(pos+1));
    pos+=2;
    for(int i=0;i<count_payrecord;i++) {
        PayRecord _record;
        int len=(unsigned char)bData.at(pos++);
        QString tmp;
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _record.datetime=tmp;
        len=(unsigned char)bData.at(pos++);
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _record.phonenumber=tmp;
        len=(unsigned char)bData.at(pos++);
        tmp=tmp.fromUtf8(bData.mid(pos,len));
        pos+=len;
        _record.price=tmp;
        report->payRecordList.append(_record);
    }
}

/*
 * Copyright 2017, Song Jiahao
 * All rights reserved.
 *
 * 文件名：mainlogic.cpp
 * 摘要：服务器端主逻辑实现
 *
 * 作者：宋嘉昊 2016011501
 * 时间：2017-9-20
 */

#include "mainlogic.h"
#include <QMessageBox>

mainLogic::mainLogic()
{
    guests.clear();
    waiters.clear();
    cooks.clear();
    managers.clear();

    guestMap.clear();
    waiterMap.clear();
    cookMap.clear();
    managerMap.clear();

    menu.clear();
    orderlist.clear();
    tables.clear();
    allEvaluation.clear();
    waiterEvaluationList.clear();
}

void mainLogic::ReadAllData() {
    QSqlQuery query;

    //读入菜单信息
    query.exec("SELECT * FROM 菜单");
    int i=0;
    while(query.next()) {
        SingleDishInfo _dish;
        _dish.name=query.value(0).toString();
        _dish.price=query.value(1).toFloat();
        _dish.isValid=query.value(2).toInt();
        _dish.averageScore=query.value(4).toFloat();
        _dish.type=query.value(5).toString();
        menuMap.insert(_dish.name,i);
        ++i;
        menu.append(_dish);
    }

    //读入餐桌信息
    query.exec("SELECT * FROM 餐桌信息");
    while(query.next())
        for(i=0;i<query.value(2).toInt();i++) {
            TableState *_table=new TableState();
            _table->fitin=query.value(1).toInt();
            _table->type=query.value(0).toString();
            _table->GuestIndex=-1; //没有顾客
            _table->isValid=true;
            _table->WaiterIndex=-1; //没有服务员
            tables.append(_table);
        }

    //读入顾客账号
    query.exec("SELECT * FROM 顾客账号信息");
    i=0;
    while(query.next()) {
        SingleGuest *_guest=new SingleGuest();
        _guest->chooseTable=-1; //没有选桌
        _guest->evalutionList.clear();
        _guest->isOnline=false;
        _guest->isPaid=false;
        _guest->price=0.0;
        _guest->isOrdered=false;
        _guest->msg.clear();
        _guest->password=query.value(2).toString();
        _guest->phonenumber=query.value(0).toString();
        _guest->username=query.value(1).toString();
        _guest->waiterIndex=-1; //没有服务员
        guestMap.insert(_guest->phonenumber,i);
        ++i;
        guests.append(_guest);
    }

    //读入服务员账号
    query.exec("SELECT * FROM 其他账号信息 WHERE 种类='服务员'");
    i=0;
    while(query.next()) {
        SingleWaiter *_waiter=new SingleWaiter();
        _waiter->chooseGuestTable.clear();
        _waiter->isOnline=false;
        _waiter->msg.clear();
        _waiter->record=0;
        _waiter->tmp_record=0;
        _waiter->password=query.value(3).toString();
        _waiter->username=query.value(2).toString();
        waiterMap.insert(_waiter->username,i);
        ++i;
        waiters.append(_waiter);
    }

    //读入厨师账号
    query.exec("SELECT * FROM 其他账号信息 WHERE 种类='厨师'");
    i=0;
    while(query.next()) {
        SingleCook *_cook=new SingleCook();
        _cook->isOnline=false;
        _cook->password=query.value(3).toString();
        _cook->selectedDish.clear();
        _cook->record=0;
        _cook->tmp_record=0;
        _cook->username=query.value(2).toString();
        cookMap.insert(_cook->username,i);
        ++i;
        cooks.append(_cook);
    }

    //读入经理账号
    query.exec("SELECT * FROM 其他账号信息 WHERE 种类='经理'");
    i=0;
    while(query.next()) {
        SingleManager *_manager=new SingleManager;
        _manager->isOnline=false;
        _manager->password=query.value(3).toString();
        _manager->username=query.value(2).toString();
        managerMap.insert(_manager->username,i);
        ++i;
        managers.append(_manager);
    }

    //读入所有评价信息
    query.exec("SELECT * FROM 评价信息");
    i=0;
    while(query.next()) {
        SingleEvaluation _evaluation;
        _evaluation.comment=query.value(3).toString();
        _evaluation.dishname=query.value(1).toString();
        _evaluation.phonenumber=query.value(2).toString();
        _evaluation.score=query.value(4).toInt();
        allEvaluation.append(_evaluation);
        if(guestMap.contains(_evaluation.phonenumber))
            guests[guestMap[_evaluation.phonenumber]]->evalutionList.append(i); //将评价信息添加到顾客信息中
        if(menuMap.contains(_evaluation.dishname))
            menu[menuMap[_evaluation.dishname]].evalutionList.append(i); //将评价信息添加到菜单信息中
        ++i;
    }

    //读入所有服务员评价信息
    waiterEvaluationList.clear();
    query.exec("SELECT * FROM 评价服务员");
    while(query.next()) {
        waiterEvaluation _e;
        _e.username=query.value(1).toString();
        _e.comment=query.value(2).toString();
        _e.score=query.value(3).toInt();
        waiterEvaluationList.append(_e);
    }

    //读入所有服务员工作记录
    query.exec("SELECT * FROM 服务员工作记录");
    while(query.next()) {
        QString username=query.value(0).toString();
        int record=query.value(1).toInt();
        if(waiterMap.contains(username)) {
            waiters[waiterMap[username]]->record=record;
        }
    }

    //读入所有厨师工作记录
    query.exec("SELECT * FROM 厨师工作记录");
    while(query.next()) {
        QString username=query.value(0).toString();
        int record=query.value(1).toInt();
        if(cookMap.contains(username)) {
            cooks[cookMap[username]]->record=record;
        }
    }

    updateEvaluationInfo();
}

/*
 * 统计所有评价信息，计算评价数量和平均分数，并更新数据库
 */
void mainLogic::updateEvaluationInfo() {
    QSqlQuery query;
    for(int i=0;i<menu.count();i++) {
        float sum=0;
        for(int j=0;j<menu[i].evalutionList.count();j++)
            sum+=allEvaluation[menu[i].evalutionList[j]].score;
        menu[i].averageScore=sum/(float)menu[i].evalutionList.count();
        query.exec(QString("UPDATE 菜单 SET 评价数量=%1, 平均评价分数=%2 WHERE 名称='%3'").arg(menu[i].evalutionList.count()).arg(menu[i].averageScore).arg(menu[i].name));
    }
}

/*
 * 顾客注册，将顾客信息加入主数据结构
 */
void mainLogic::guest_register(QString phonenumber,QString username,QString password) {
    SingleGuest *_guest=new SingleGuest();
    _guest->chooseTable=-1; //没有选桌
    _guest->evalutionList.clear();
    _guest->isOnline=false;
    _guest->isPaid=false;
    _guest->msg.clear();
    _guest->password=password;
    _guest->phonenumber=phonenumber;
    _guest->username=username;
    _guest->waiterIndex=-1; //没有服务员
    guestMap.insert(_guest->phonenumber,guests.count());
    guests.append(_guest);
}

/*
 * 添加单条服务员评价信息
 */
void mainLogic::guest_evaluateWaiter(QString username,QString comment,int score) {
    waiterEvaluation _e;
    _e.comment=comment;
    _e.username=username;
    _e.score=score;
    waiterEvaluationList.append(_e);
}

/*
 * 顾客登录，检查是否登录成功
 */
int mainLogic::guest_login(QString phonenumber,QString password) {
    if(guestMap.contains(phonenumber)) {
        int index=guestMap[phonenumber];
        if(guests.at(index)->password==password) { //登录成功
            guests.at(index)->isOnline=true;
            return 1;
        }
        else
            return 0;
    }
    return -1;
}

/*
 * 顾客选桌，返回是否成功
 */
bool mainLogic::guest_choosetable(int index,QString phonenumber) {
    if(guestMap.contains(phonenumber) && tables.at(index)->isValid) {
        tables[index]->isValid=false;
        tables[index]->GuestIndex=guestMap[phonenumber];
        guests.at(guestMap[phonenumber])->chooseTable=index;
        emit updateAllOrderInfoExceptMe(phonenumber);
        return true;
    }
    else
        return false;
}

/*
 * 顾客离开餐桌
 */
void mainLogic::guest_quittable(int index) {
    if(index>=0 && index<tables.count()) {
        int windex=tables.at(index)->WaiterIndex;
        tables[index]->isValid=true;
        tables[index]->GuestIndex=-1;
        tables[index]->WaiterIndex=-1;
        if(windex>=0 && windex<waiters.count())
            if(waiters.at(windex)->chooseGuestTable.contains(index))
                waiters[windex]->chooseGuestTable.removeOne(index);
    }
}

/*
 * 顾客下线
 */
void mainLogic::guest_logout(QString phonenumber) {
    if(guestMap.contains(phonenumber))
        guests.at(guestMap[phonenumber])->isOnline=false;
}

/*
 * 将所有餐桌信息编码以便发送
 */
QByteArray mainLogic::guest_Encodingtableinfo(QString phonenumber) {
    QByteArray ans;
    if(guestMap.contains(phonenumber)) {

    ans.append(TOKEN_GUEST_SENDTABLE);
    ans.append((unsigned char)tables.count()/256);
    ans.append((unsigned char)tables.count()%256);
    for(int i=0;i<tables.count();i++) {
        QByteArray b_type=tables.at(i)->type.toUtf8();
        ans.append((unsigned char)b_type.length());
        ans.append(b_type);
        ans.append((unsigned char)tables.at(i)->fitin);
        if(guestMap.contains(phonenumber) && guests.at(guestMap[phonenumber])->chooseTable==i)
            ans.append(2);
        else
            ans.append((tables.at(i)->isValid)?1:0);
    }
    int windex=guests[guestMap[phonenumber]]->waiterIndex;
    if(windex==-1)
        ans.append(254);
    else {
        ans.append(windex);
        QByteArray tmp=waiters.at(windex)->username.toUtf8();
        ans.append(tmp.length());
        ans.append(tmp);
    }

    }
    return ans;
}

/*
 * 服务员注册，将服务员信息加入主数据结构
 */
void mainLogic::waiter_register(QString username,QString password) {
    SingleWaiter *_waiter=new SingleWaiter();
    _waiter->chooseGuestTable.clear();
    _waiter->isOnline=false;
    _waiter->msg.clear();
    _waiter->password=password;
    _waiter->username=username;
    waiterMap.insert(_waiter->username,waiters.count());
    waiters.append(_waiter);
}

/*
 * 服务员登录
 */
int mainLogic::waiter_login(QString username,QString password) {
    if(waiterMap.contains(username)) {
        if(waiters[waiterMap[username]]->password==password) {
            waiters[waiterMap[username]]->isOnline=true;
            return 1;
        }
        else
            return 0;
    }
    return -1;
}

/*
 * 服务员下线
 */
void mainLogic::waiter_logout(QString username) {
    if(waiterMap.contains(username))
        waiters[waiterMap[username]]->isOnline=false;
}

/*
 * 厨师注册，将厨师信息加入主数据结构
 */
void mainLogic::cook_register(QString username,QString password) {
    SingleCook *_cook=new SingleCook();
    _cook->isOnline=false;
    _cook->password=password;
    _cook->selectedDish.clear();
    _cook->username=username;
    cookMap.insert(_cook->username,cooks.count());
    cooks.append(_cook);
}

/*
 * 厨师登录
 */
int mainLogic::cook_login(QString username,QString password) {
    if(cookMap.contains(username)) {
        if(cooks[cookMap[username]]->password==password) {
            cooks[cookMap[username]]->isOnline=true;
            return 1;
        }
        else
            return 0;
    }
    return -1;
}

/*
 * 厨师下线
 */
void mainLogic::cook_logout(QString username) {
    if(cookMap.contains(username))
        cooks[cookMap[username]]->isOnline=false;
}

/*
 * 经理注册
 */
void mainLogic::manager_register(QString username,QString password) {
    SingleManager *_manager=new SingleManager;
    _manager->isOnline=false;
    _manager->username=username;
    _manager->password=password;
    managerMap.insert(_manager->username,managers.count());
    managers.append(_manager);
}

/*
 * 经理登录
 */
int mainLogic::manager_login(QString username,QString password) {
    if(managerMap.contains(username)) {
        if(managers[managerMap[username]]->password==password) {
            managers[managerMap[username]]->isOnline=true;
            return 1;
        }
        else
            return 0;
    }
    return -1;
}

/*
 * 经理下线
 */
void mainLogic::manager_logout(QString username) {
    if(managerMap.contains(username))
        managers[managerMap[username]]->isOnline=false;
}

/*
 * 添加一条评价信息，并做相应的更新操作
 */
void mainLogic::addsingleevaluation(QString phonenumber,QString dishname,QString comment,int score) {
    SingleEvaluation _evaluation;
    _evaluation.comment=comment;
    _evaluation.phonenumber=phonenumber;
    _evaluation.dishname=dishname;
    _evaluation.score=score;
    int i=allEvaluation.count();
    allEvaluation.append(_evaluation);
    guests[guestMap[phonenumber]]->evalutionList.append(i);
    menu[menuMap[dishname]].evalutionList.append(i);
    updateEvaluationInfo();
}

/*
 * 添加来自顾客的一组评价信息
 */
void mainLogic::guest_evaluate(dataStructure::evaluateList list) {
    for(int i=0;i<list.count;i++)
        addsingleevaluation(list.phonenumber,list.list[i].dishname,list.list[i].comment,list.list[i].score);
}

/*
 * 添加一条点菜信息
 */
void mainLogic::addsingleorderinfo(QString dishname,QString phonenumber) {
    SingleDishOrdered _order;
    _order.name=dishname;
    _order.index=orderlist.count();
    _order.state=DISH_UNCHOSEN; //无人认领
    _order.cookIndex=-1;
    orderlist.append(_order);
    guests[guestMap[phonenumber]]->orderList.append(_order.index);
    guests[guestMap[phonenumber]]->price+=menu.at(menuMap[dishname]).price;
}

/*
 * 添加来自顾客的一组点菜信息
 */
void mainLogic::guest_order(dataStructure::orderList list) {
    if(guestMap.contains(list.phonenumber)) {
        for(int i=0;i<list.count;i++)
            for(int j=0;j<list.list[i].num;j++)
                addsingleorderinfo(list.list[i].name,list.phonenumber);
        guests.at(guestMap[list.phonenumber])->isOrdered=true;
        updateAllOrderInfo();
    }
    if(!guestMap.contains(list.phonenumber))
        throw QString("用户不存在，无法添加点菜信息");
}

/*
 * 顾客结账
 */
void mainLogic::guest_pay(QString phonenumber) {
    if(guestMap.contains(phonenumber)) {
        int i=guestMap[phonenumber];
        guests[i]->isPaid=true;
        guests[i]->isOrdered=false;
        guests[i]->price=0.0;
        guests[i]->msg.clear();
        guests[i]->waiterIndex=-1; //？？？？
        guest_quittable(guests.at(i)->chooseTable);
        guests[i]->chooseTable=-1;
        guests[i]->orderList.clear();

        updateAllOrderInfoExceptMe(phonenumber);
    }
    if(!guestMap.contains(phonenumber))
        throw QString("用户不存在，无法结账");
}

/*
 * 服务员选桌
 */
bool mainLogic::waiter_choosetable(int index,QString username) {
    if(tables.at(index)->isValid)
        return false; //空桌不可选
    if(waiterMap.contains(username)) {
        if(tables.at(index)->WaiterIndex==-1) {
            tables.at(index)->WaiterIndex=waiterMap[username];
            waiters[waiterMap[username]]->chooseGuestTable.append(index);
            int guest_index=tables.at(index)->GuestIndex;
            if(guest_index>=0 && guest_index<=guests.count())
                guests[guest_index]->waiterIndex=waiterMap[username];
            updateAllOrderInfo();
            return true; //选桌成功
        }
        else
            return false; //已被选
    }
    return false;
}

/*
 * 厨师认领菜品
 */
bool mainLogic::cook_choosedish(int index,QString username) {
    if(orderlist.at(index).state!=DISH_UNCHOSEN)
        return false; //不是还未被认领的菜品，无法认领
    if(orderlist.at(index).cookIndex!=-1)
        return false; //已被认领
    if(cookMap.contains(username)) {
        orderlist[index].cookIndex=cookMap[username];
        cooks.at(cookMap[username])->selectedDish.append(index);
        orderlist[index].state=DISH_CHOSEN_UNSTARTED;
        updateAllOrderInfo();
        return true;
    }
    return true;
}

/*
 * 厨师设置菜品状态
 */
bool mainLogic::cook_setdishstate(int index,int state,QString username) {
    if(!cookMap.contains(username))
        return false;
    int i=cookMap[username];
    if(orderlist.at(index).cookIndex!=i)
        return false; //该菜品不是自己认领的，无法改变状态
    if(state!=DISH_STARTED && state!=DISH_FINISHED)
        return false; //只能设置为正在做、已完成
    orderlist[index].state=state;
    updateAllOrderInfo();
    return true;
}

/*
 * 将所有未开始做、无人认领的菜品编码，以便发送
 */
QByteArray mainLogic::cook_getunstarteddish() {
    QByteArray ans;
    ans.append(TOKEN_COOK_GETUNSTARTED);
    int count=0;
    ans.append(1);
    ans.append(1);
    for(int i=0;i<orderlist.count();i++)
        if(orderlist.at(i).state==DISH_UNCHOSEN && orderlist.at(i).cookIndex==-1) {
            QByteArray b_tmp=orderlist.at(i).name.toUtf8();
            ans.append((unsigned char)b_tmp.length());
            ans.append(b_tmp);
            ans.append((unsigned char)(orderlist.at(i).index/256));
            ans.append((unsigned char)(orderlist.at(i).index%256));
            count++;
        }
    ans[1]=(unsigned char)(count/256);
    ans[2]=(unsigned char)(count%256);
    return ans;
}

/*
 * 编码顾客所有已点菜品信息
 */
QByteArray mainLogic::guest_EncodingOrderlist(QString phonenumber) {
    QByteArray ans;
    ans.append(TOKEN_GUEST_GETORDERLIST);
    if(guestMap.contains(phonenumber)) {
        int guestindex=guestMap[phonenumber];
        int count=guests.at(guestindex)->orderList.count();
        ans.append((unsigned char)(count/256));
        ans.append((unsigned char)(count%256));
        for(int i=0;i<count;i++) {
            QByteArray tmp=orderlist.at(guests.at(guestindex)->orderList.at(i)).name.toUtf8();
            ans.append((unsigned char)tmp.length());
            int index=orderlist.at(guests.at(guestindex)->orderList.at(i)).index;
            int state=orderlist.at(guests.at(guestindex)->orderList.at(i)).state;
            ans.append(tmp);
            ans.append((unsigned char)(index/256));
            ans.append((unsigned char)(index%256));
            ans.append((unsigned char)state);
        }
    }
    float price=guests.at(guestMap[phonenumber])->price;
    int p=(int)(price*100.0);
    ans.append((unsigned char)(p/65536));
    ans.append((unsigned char)(p%65536/256));
    ans.append((unsigned char)(p%256));

    //加入服务员用户名
    if(guestMap.contains(phonenumber)) {
        int waiterindex=guests.at(guestMap[phonenumber])->waiterIndex;
        if(waiterindex==-1)
            ans.append(254);
        else {
            QByteArray tmp=waiters.at(waiterindex)->username.toUtf8();
            ans.append(tmp.length());
            ans.append(tmp);
        }
    }

    return ans;
}

/*
 * 将厨师所有所需信息编码并发送
 */
QByteArray mainLogic::cook_encodingAll(QString username) {
    QByteArray ans;
    ans.append(TOKEN_COOK_GETALLINFO);
    int count=0;
    ans.append(1);
    ans.append(1);
    for(int i=0;i<orderlist.count();i++)
        if(orderlist.at(i).state==DISH_UNCHOSEN && orderlist.at(i).cookIndex==-1) {
            QByteArray b_tmp=orderlist.at(i).name.toUtf8();
            ans.append((unsigned char)b_tmp.length());
            ans.append(b_tmp);
            ans.append((unsigned char)(orderlist.at(i).index/256));
            ans.append((unsigned char)(orderlist.at(i).index%256));
            count++;
        }
    ans[1]=(unsigned char)(count/256);
    ans[2]=(unsigned char)(count%256);

    if(cookMap.contains(username)) {
        int c_index=cookMap[username];
        count=cooks.at(c_index)->selectedDish.count();
        ans.append((unsigned char)(count/256));
        ans.append((unsigned char)(count%256));
        for(int i=0;i<count;i++) {
            QByteArray b_tmp=orderlist.at(cooks.at(c_index)->selectedDish.at(i)).name.toUtf8();
            ans.append((unsigned char)b_tmp.length());
            ans.append(b_tmp);
            ans.append((unsigned char)(orderlist.at(cooks.at(c_index)->selectedDish.at(i)).state));
            ans.append((unsigned char)(orderlist.at(cooks.at(c_index)->selectedDish.at(i)).index/256));
            ans.append((unsigned char)(orderlist.at(cooks.at(c_index)->selectedDish.at(i)).index%256));
        }
    }
    return ans;
}

/*
 * 将自己已选菜品编码，以便发送
 */
QByteArray mainLogic::cook_getchosendish(QString username) {
    QByteArray ans;
    if(cookMap.contains(username)) {
        ans.append(TOKEN_COOK_GETSELECTED);
        int c_index=cookMap[username];
        int count=cooks.at(c_index)->selectedDish.count();
        ans.append((unsigned char)(count/256));
        ans.append((unsigned char)(count%256));
        for(int i=0;i<count;i++) {
            QByteArray b_tmp=orderlist.at(cooks.at(c_index)->selectedDish.at(i)).name.toUtf8();
            ans.append((unsigned char)b_tmp.length());
            ans.append(b_tmp);
            ans.append((unsigned char)(orderlist.at(cooks.at(c_index)->selectedDish.at(i)).state));
            ans.append((unsigned char)(orderlist.at(cooks.at(c_index)->selectedDish.at(i)).index/256));
            ans.append((unsigned char)(orderlist.at(cooks.at(c_index)->selectedDish.at(i)).index%256));
        }
    }
    return ans;
}

/*
 * 当点菜列表更新（有新加入菜品或取消菜品）时，通知所有相关客户端
 */
void mainLogic::orderlist_updated() {
    emit updateAllOrderInfo();
}

/*
 * 编码所有服务员所需信息，包括餐桌信息，所认领餐桌的菜品信息，消息
 */
QByteArray mainLogic::EncodingWaiterInfo(QString username) {
    QByteArray ans;
    if(waiterMap.contains(username)) {
        int index=waiterMap[username];
        ans.append(TOKEN_WAITER_GETALLINFO);

        //餐桌信息
        ans.append((unsigned char)(tables.count()/256));
        ans.append((unsigned char)(tables.count()%256));
        for(int i=0;i<tables.count();i++) {
            QByteArray tmp;
            tmp=tables.at(i)->type.toUtf8();
            ans.append((unsigned char)tmp.length()); //餐桌类别
            ans.append(tmp);
            ans.append((unsigned char)tables.at(i)->fitin); //餐桌容纳人数
            if(tables.at(i)->isValid)
                ans.append(1); //无人
            else
                if((!tables.at(i)->isValid) && tables.at(i)->WaiterIndex==-1)
                    ans.append(2); //有人，未被认领
                else
                    ans.append(3); //有人，已被认领
            if(tables.at(i)->GuestIndex!=-1) { //有人
                ans.append(1);
                ans.append((unsigned char)guests.at(tables.at(i)->GuestIndex)->orderList.count()); //点菜数量
            }
            else
                ans.append(2);
        }

        //菜品信息
        int tablecount=waiters.at(index)->chooseGuestTable.count();
        ans.append((unsigned char)tablecount); //所选桌数
        for(int i=0;i<tablecount;i++) {
            int guestindex=tables.at(waiters.at(index)->chooseGuestTable.at(i))->GuestIndex; //桌号对应的顾客编号
            ans.append((unsigned char)waiters.at(index)->chooseGuestTable.at(i)); //所选桌号
            int dishcount=guests.at(guestindex)->orderList.count(); //顾客点菜数量
            ans.append((unsigned char)(dishcount/256));
            ans.append((unsigned char)(dishcount%256));
            for(int j=0;j<dishcount;j++) {
                int tmp_index=guests.at(guestindex)->orderList.at(j);
                ans.append((unsigned char)(tmp_index/256)); //菜品编号
                ans.append((unsigned char)(tmp_index%256));
                QByteArray tmp;
                tmp=orderlist.at(tmp_index).name.toUtf8();
                ans.append((unsigned char)tmp.length()); //菜品名称
                ans.append(tmp);
                ans.append(orderlist.at(tmp_index).state); //菜品状态
            }
        }

        //消息队列中的消息
        int msgcount=waiters.at(index)->msg.count();
        ans.append((unsigned char)(msgcount/256));
        ans.append((unsigned char)(msgcount%256));
        for(int i=0;i<msgcount;i++) {
            QByteArray tmp=waiters.at(index)->msg.at(i).msg.toUtf8();
            ans.append((unsigned char)tmp.length()); //消息
            ans.append(tmp);
            tmp=waiters.at(index)->msg.at(i).source.toUtf8();
            ans.append((unsigned char)tmp.length()); //发送消息的顾客手机号
            ans.append(tmp);
            ans.append((unsigned char)waiters.at(index)->msg.at(i).sourceTable); //发送消息的桌号
        }
    }
    return ans;
}

/*
 * 服务员给指定餐桌发消息
 */
void mainLogic::waiter_msgtoguest(QString msg,int target,QString username) {
    if(waiterMap.contains(username)) {
        int index=waiterMap[username];
        if(index>=0 && index<waiters.count())
            if(target>=0 && target<tables.count() && (!tables.at(target)->isValid)) {
                int guestindex=tables.at(target)->GuestIndex;
                if(guestindex>=0 && guestindex<guests.count()) {
                    if(guests.at(guestindex)->isOnline) { //客户在线，就直接发送消息
                        QByteArray ans,tmp_un,tmp_msg;
                        ans.append(TOKEN_GUEST_SINGLEMESSAGE);
                        tmp_un=username.toUtf8();
                        ans.append((unsigned char)tmp_un.length());
                        ans.append(tmp_un);
                        tmp_msg=msg.toUtf8();
                        ans.append((unsigned char)(tmp_msg.length()/256));
                        ans.append((unsigned char)(tmp_msg.length()%256));
                        ans.append(tmp_msg);
                        QString phone=guests.at(guestindex)->phonenumber;
                        emit MessagetoGuest(ans,phone);
                    }
                    else { //客户不在线，就加入客户消息队列
                        SingleMessage _msg;
                        _msg.msg=msg;
                        _msg.source=username;
                        _msg.sourceTable=target;
                        _msg.type=1;
                        guests[guestindex]->msg.append(_msg);
                    }
                }
            }
    }
}

/*
 * 顾客给其服务员发消息
 */
void mainLogic::guest_msgtowaiter(QString msg,QString phonenumber) {
    if(guestMap.contains(phonenumber)) {
        int guestindex=guestMap[phonenumber];
        if(guestindex>=0 && guestindex<guests.count()) {
            int waiterindex=guests.at(guestindex)->waiterIndex;
            int tableindex=guests.at(guestindex)->chooseTable;
            if(waiterindex>=0 && waiterindex<waiters.count() && tableindex>=0 && tableindex<tables.count()) {
                if(waiters.at(waiterindex)->isOnline) { //服务员在线，就直接发送消息
                    QString waiter_un=waiters.at(waiterindex)->username;
                    QByteArray ans,tmp_msg;
                    ans.append(TOKEN_WAITER_SINGLEMESSAGE);
                    ans.append((unsigned char)(tableindex/256));
                    ans.append((unsigned char)(tableindex%256));
                    tmp_msg=msg.toUtf8();
                    ans.append((unsigned char)(tmp_msg.length()/256));
                    ans.append((unsigned char)(tmp_msg.length()%256));
                    ans.append(tmp_msg);
                    emit MessagetoWaiter(ans,waiter_un);
                }
                else { //服务员不在线，就加入服务员消息队列
                    SingleMessage _msg;
                    _msg.msg=msg;
                    _msg.source=phonenumber;
                    _msg.sourceTable=tableindex;
                    _msg.type=2;
                    waiters[waiterindex]->msg.append(_msg);
                }
            }
        }
    }
}

/*
 * 编码顾客消息队列
 */
QByteArray mainLogic::guest_EncodingMsgQueue(QString phonenumber) {
    QByteArray ans;
    if(guestMap.contains(phonenumber)) {
        int guestindex=guestMap[phonenumber];
        if(guestindex>=0 && guestindex<guests.count()) {
            ans.append(TOKEN_GUEST_MSGQUEUE);
            int count=guests.at(guestindex)->msg.count();
            count=(count>255)?255:count;
            ans.append((unsigned char)count);
            for(int i=0;i<count;i++) {
                QByteArray tmp=guests.at(guestindex)->msg.at(i).msg.toUtf8();
                ans.append((unsigned char)(tmp.length()/256));
                ans.append((unsigned char)(tmp.length()%256));
                ans.append(tmp);
                tmp=guests.at(guestindex)->msg.at(i).source.toUtf8();
                ans.append((unsigned char)(tmp.length()));
                ans.append(tmp);
            }
        }
    }
    return ans;
}

/*
 * 生成数据报表
 */
QByteArray mainLogic::manager_generateReport() {
    QByteArray ans;
    ans.append(TOKEN_MANAGER_REPORT);

    //服务员工作记录
    ans.append((unsigned char)(waiters.count()/256));
    ans.append((unsigned char)(waiters.count()%256));
    for(int i=0;i<waiters.count();i++) {
        QByteArray tmp;
        tmp=waiters.at(i)->username.toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        ans.append((unsigned char)(waiters.at(i)->tmp_record/256));
        ans.append((unsigned char)(waiters.at(i)->tmp_record%256));
        ans.append((unsigned char)(waiters.at(i)->record/256));
        ans.append((unsigned char)(waiters.at(i)->record%256));
    }

    //厨师工作记录
    ans.append((unsigned char)(cooks.count()/256));
    ans.append((unsigned char)(cooks.count()%256));
    for(int i=0;i<cooks.count();i++) {
        QByteArray tmp;
        tmp=cooks.at(i)->username.toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        ans.append((unsigned char)(cooks.at(i)->tmp_record/256));
        ans.append((unsigned char)(cooks.at(i)->tmp_record%256));
        ans.append((unsigned char)(cooks.at(i)->record/256));
        ans.append((unsigned char)(cooks.at(i)->record%256));
    }

    //服务员评价
    ans.append((unsigned char)(waiterEvaluationList.count()/256));
    ans.append((unsigned char)(waiterEvaluationList.count()%256));
    for(int i=0;i<waiterEvaluationList.count();i++) {
        QByteArray tmp;
        tmp=waiterEvaluationList.at(i).username.toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        tmp=waiterEvaluationList.at(i).comment.toUtf8();
        ans.append((unsigned char)(tmp.length()/256));
        ans.append((unsigned char)(tmp.length()%256));
        ans.append(tmp);
        ans.append((unsigned char)waiterEvaluationList.at(i).score);
    }

    //菜品评价
    ans.append((unsigned char)(allEvaluation.count()/256));
    ans.append((unsigned char)(allEvaluation.count()%256));
    for(int i=0;i<allEvaluation.count();i++) {
        QByteArray tmp;
        tmp=allEvaluation.at(i).phonenumber.toUtf8();
        ans.append((unsigned char)(tmp.length()));
        ans.append(tmp);
        tmp=allEvaluation.at(i).dishname.toUtf8();
        ans.append((unsigned char)tmp.length());
        ans.append(tmp);
        tmp=allEvaluation.at(i).comment.toUtf8();
        ans.append((unsigned char)(tmp.length()/256));
        ans.append((unsigned char)(tmp.length()%256));
        ans.append(tmp);
        ans.append((unsigned char)allEvaluation.at(i).score);
    }

    return ans;
}

/*
 * 顾客更改账户信息
 */
void mainLogic::guest_ChangeAccount(QString phonenumber,QString new_username,QString new_password) {
    if(guestMap.contains(phonenumber)) {
        int index=guestMap[phonenumber];
        if(index>=0 && index<guests.count()) {
            guests[index]->username=new_username;
            guests[index]->password=new_password;
        }
    }
    if(!guestMap.contains(phonenumber))
        throw QString("用户%1不存在，无法更改账户信息").arg(phonenumber);
}

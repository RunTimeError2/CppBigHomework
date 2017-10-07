#include "login.h"

GuestLogin::GuestLogin()
{
    setupDataBase();
}

/*
 * 连接到数据库，返回值表示是否成功
 */
bool GuestLogin::setupDataBase() {
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("MainComputer");
    db.setDatabaseName("restaurantDB.db");
    db.setUserName("Administrator");
    db.setPassword("admin");
    return (dbOpened=db.open());
}


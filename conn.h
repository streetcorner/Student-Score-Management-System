//测试数据库连接
#ifndef CONN_H
#define CONN_H
#include <QSqlDatabase>
#include <QDebug>

bool connectiondatabase()
{
       QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
       db.setHostName("localhost");
       db.setDatabaseName("sms");
       db.setUserName("root");
       db.setPassword("123456");
       if(db.open())
       {
           qDebug()<<"database is established!";
       }
       else
       {
           qDebug()<<"build error!";
           return false;
       }
       return true;
}
#endif // CONN_H

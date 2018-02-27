#include "mainwindow.h"
#include <QApplication>
#include "conn.h" //测试数据库的头文件
#include <QTextCodec>  //使用中文

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QApplication a(argc, argv);
    connectiondatabase(); //连接数据库
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);  //去掉边框
    w.show();
    return a.exec();
}

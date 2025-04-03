#include "mainwindow.h"
#include <QApplication>
#include<login.h>
#include<QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings *set=new QSettings("user.ini",QSettings::IniFormat);
    QString user_name=set->value("/user/username").toString();
    QString user_pwd=set->value("/user/username").toString();
    MainWindow w;
    if(user_name.isEmpty()&&user_pwd.isEmpty()){
        Login *L=new Login;
    }else {
        emit w.sendValue(user_name);
        w.show();
    }


    return a.exec();
}

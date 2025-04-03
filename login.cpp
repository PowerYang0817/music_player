#include "login.h"
#include"dbmanager.h"
#include<QDebug>
#include"mainwindow.h"
#include"register.h"
#include<QSettings>//写入和读取ini键值对
Login::Login(QObject *parent) : QObject(parent)
{
    engine.load(QUrl("qrc:/Login.qml"));
    //获取发送信号的操作
    QObject* root=engine.rootObjects().first();
    loginWindow = qobject_cast<QWindow*>(root);
    //旧写法，qt4
    connect(root,SIGNAL(loginMessage(QString,QString)),this,SLOT(slot_recv_login_qml_info(QString,QString)));

    connect(root,SIGNAL(newregister(QString,QString)),this,SLOT(slot_recv_register_qml_info(QString,QString)));
}

void Login::slot_recv_register_qml_info(QString n,QString p)
{
    qDebug()<<n<<p;
//    loginWindow->close();
    Register *r=new Register();
    connect(r,&Register::sendRegisterInfo,this,&Login::slot_revcUserInfo);
}

void Login::slot_revcUserInfo(QString name, QString pwd)
{
    QObject* root=engine.rootObjects().first();
    QObject *q1=root->findChild<QObject*>("username_input");
    QObject *q2=root->findChild<QObject*>("userpwd_input");
    q1->setProperty("text",name);
    q2->setProperty("text",pwd);
}
void Login::slot_recv_login_qml_info(QString name_in, QString pwd_in)
{
    qDebug()<<name_in<<pwd_in;
    if(DbManager::getDbManager()->login(UserInfo(name_in,pwd_in))){
        qDebug()<<"登录成功";
        MainWindow *m=new MainWindow;
        //信号传值
        QString name(name_in);
        emit m->sendValue(name);
        m->show();
        //保存ini文件
        QSettings *set=new QSettings("user.ini",QSettings::IniFormat);
        set->setValue("user/username",name_in);
        set->setValue("user/password",pwd_in);
        //关闭当前窗口
        engine.rootObjects().first()->setProperty("visible",false);
    }else {
        qDebug()<<"登录失败";
    }
}

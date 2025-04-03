#include "register.h"
#include"dbmanager.h"
#include<QDebug>
#include"mainwindow.h"
Register::Register(QObject *parent) : QObject(parent)
{
    engine.load(QUrl("qrc:/register.qml"));
    //获取发送信号的操作
    rootObject=engine.rootObjects().first();
    //旧写法，qt4
    connect(rootObject,SIGNAL(registerMessage(QString,QString)),this,SLOT(slot_recv_register_qml_info(QString,QString)));
}
QObject *root;
void Register::slot_recv_register_qml_info(QString name_in, QString pwd_in)
{
    qDebug()<<name_in<<pwd_in;
    engine2.load(QUrl("qrc:/answer.qml"));
    root=engine2.rootObjects().first();
    tip=root->findChild<QObject*>("tip");
    if(name_in==""||pwd_in==""){
        qDebug()<<"不能为空";
        //找文本控件
        tip->setProperty("text","账号密码不能为空");
        return;
    }
    if(DbManager::getDbManager()->register_my(UserInfo(name_in,pwd_in))){
        qDebug()<<"注册成功";
        emit sendRegisterInfo(name_in,pwd_in);
    }else {
        qDebug()<<"注册失败";
        tip->setProperty("text","用户已存在");
    }

}

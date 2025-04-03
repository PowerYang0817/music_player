#ifndef REGISTER_H
#define REGISTER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
class Register : public QObject
{
    Q_OBJECT
public:
    explicit Register(QObject *parent = nullptr);

signals:
    void sendRegisterInfo(QString name,QString pwd);
private:
    QQmlApplicationEngine engine;//引擎
    QQmlApplicationEngine engine2;//引擎
    QObject *rootObject;
    QObject *tip;
private slots:
     void slot_recv_register_qml_info(QString name_in,QString pwd_in);
};

#endif // REGISTER_H

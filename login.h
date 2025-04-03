#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWindow>
class Login : public QObject
{
    Q_OBJECT
public:
    explicit Login(QObject *parent = nullptr);

signals:

private:
    QQmlApplicationEngine engine;//引擎
    QWindow *loginWindow; // 新增窗口对象指针
private slots:
    void slot_recv_login_qml_info(QString name_in,QString pwd_in);
    void slot_recv_register_qml_info(QString n,QString p);
    void slot_revcUserInfo(QString name,QString pwd);
};

#endif // LOGIN_H

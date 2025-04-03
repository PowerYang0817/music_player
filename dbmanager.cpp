#include "dbmanager.h"
#include<QSqlRecord>
DbManager * DbManager:: dbptr=new DbManager;
DbManager::DbManager()
{
    db=QSqlDatabase::addDatabase("QSQLITE");

    //2.创建数据库文件
    db.setDatabaseName("music.db");
    //3.打开数据库
    db.open();
    //创建历史记录表
    createTable();
}
void DbManager::createTable(){
    QString sql="CREATE TABLE if not exists history("
  "id INTEGER NOT NULL,"
  "userid INTEGER NOT NULL,"
  "music_name text NOT NULL,"
  "listen_last_time DATETIME NOT NULL,"
  "PRIMARY KEY (id));";
    QSqlQuery query;
    qDebug()<<query.exec(sql);
    sql="CREATE TABLE if not exists user_info("
          "user_id INTEGER NOT NULL,"
          "user_name text NOT NULL,"
          "user_pwd text NOT NULL,"
          "PRIMARY KEY (user_id));";
    qDebug()<<query.exec(sql);
}

bool DbManager::login(UserInfo info)//登录检测
{
    QString sql="select * from user_info where user_name=? and user_pwd=?;";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,info.user_name);
    query.bindValue(1,info.user_pwd);
    query.exec();
    if(query.next()){
        return true;
    }
    return false;
}

bool DbManager::register_my(UserInfo info)//注册检测
{
    QString sql="select * from user_info where user_name=?;";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,info.user_name);
    query.exec();
    if(query.next()){
        return false;
    }
    sql="insert into user_info (user_name,user_pwd) values (?,?); ";
    query.prepare(sql);
    query.bindValue(0,info.user_name);
    query.bindValue(1,info.user_pwd);
    if(query.exec()){
        return true;
    }
    return false;
}

int DbManager::nameToId(QString userName)
{
    QString sql="select user_id from user_info where user_name=?;";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,userName);
    query.exec();
    if(query.next()){
        return query.record().value(0).toInt();
    }else {
        return -1;
    }
}

void DbManager::setInit(set<MusicInfo> &set, int userId)//根据用户返回历史记录
{
    QString sql="select music_name,listen_last_time from history where userid=?;";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,userId);
    query.exec();
    //
    while (query.next()) {
        MusicInfo info;
        info.user_id=userId;
        info.music_name=query.record().value(0).toString();
        info.listen_last_time=query.record().value(1).toDateTime();//.toString("yyyy-MM-dd hh:mm:ss");
        set.insert(info);
    }
}

void DbManager::updateTime(MusicInfo info)
{
    QString sql="update history set listen_last_time=datetime('now','localtime') where userid=? and music_name=?";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,info.user_id);
    query.bindValue(1,info.music_name);
    query.exec();
}
DbManager::DbManager(const DbManager &other)
{

}

DbManager::~DbManager()
{

}

DbManager *DbManager::getDbManager()
{
    return dbptr;
}

bool DbManager::insertHistory(MusicInfo info)
{
    QString sql="insert into history (userid,music_name,listen_last_time) values(?,?,datetime('now','localtime')); ";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,info.user_id);
    query.bindValue(1,info.music_name);
    bool ok=query.exec();
    if(ok){
        qDebug()<<"插入成功";
        return true;
    }else {
        qDebug()<<"插入失败";
        return false;
    }
}

UserInfo::UserInfo()
{

}

UserInfo::UserInfo(QString name, QString pwd):user_name(name),user_pwd(pwd)
{

}

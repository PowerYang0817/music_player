#ifndef DBMANAGER_H
#define DBMANAGER_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QDateTime>
#include<set>
using std::set;

struct MusicInfo{//歌曲信息
    int id;
    int user_id;
    QString music_name;
    QDateTime listen_last_time;
    bool operator<(const MusicInfo &other)const{
        return  music_name<other.music_name;
    }
};
struct UserInfo{
    int user_id;
    QString user_name;
    QString user_pwd;
    UserInfo();
    UserInfo(QString name,QString pwd);
};

//单例
class DbManager
{
    DbManager();
    DbManager(const DbManager& other);
    ~DbManager();
    static DbManager * dbptr;
    QSqlDatabase db;
public:
    static DbManager *getDbManager();
    bool insertHistory(MusicInfo info);
    void setInit(set<MusicInfo>& set,int userId);//历史记录容器初始化
    void updateTime(MusicInfo info);//数据库音乐时间更新
    bool login(UserInfo info);//登陆检测
    bool register_my(UserInfo info);
    int nameToId(QString userName);
private:
    void createTable();//插入历史记录

};


#endif // DBMANAGER_H

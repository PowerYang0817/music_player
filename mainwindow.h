#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDir>
#include<QLabel>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include <QListWidgetItem>//表示 QListWidget 里条目的类
#include<dbmanager.h>//数据库头文件
#include<login.h>
#include<set>
#include<QSettings>
using  std::set;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //换歌名
    void changeMusicName();
signals:
    void sendValue(const QString& value);
private slots:
    void on_pushButton_chooseMusicList_clicked();

    void on_pushButton_startAndEnd_clicked();

    void on_pushButton_nextMusic_clicked();

    void on_pushButton_priMusic_clicked();

    void on_horizontalSlider_sound_valueChanged(int value);

    void on_horizontalSlider_time_valueChanged(int value);
//音乐时长
    void onMediaDurationChanged(qint64 duration);
    void onMediaPositionChanged(qint64 position);
//实时显示当前音乐名
    void on_playlist_currentIndexChanged(int index);
    void on_comboBox_c_currentIndexChanged(int index);

    void on_horizontalSlider_time_sliderPressed();

    void on_horizontalSlider_time_sliderReleased();

    void on_listWidget_musicList_itemDoubleClicked(QListWidgetItem *item);
    void receiveValue(QString username);//接收用户名
    void on_pushButton_changeUser_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    QString userName;
    int userId;
    QLabel *backgroundLabel; // 用于显示背景GIF的QLabel
    QMovie *backgroundMovie; // 用于背景的QMovie
    DbManager *dbptr;//数据库
    set<MusicInfo> historySet;//历史记录容器
    void setGifBackground(const QString &gifPath);//设置背景gif
    void resizeEvent(QResizeEvent *event) override; // 声明重写的 resizeEvent 函数--gif背景
    bool isSliderDragging;//区分进度条是否被按下
    void updateTimeShow(qint64 position);//更新音乐时长显示
    void updatePlayingItemBackground(int index); // 更新播放项的背景
    void addToMusicHistory(int index);//添加历史列表
    bool isMusicPlaying; // 新增标志位，用于判断是否正在播放音乐,解决第一首被直接添加问题
    void setInit();

};
#endif // MAINWINDOW_H

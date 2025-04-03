#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QMovie>
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player=new QMediaPlayer(this);
    playList=new QMediaPlaylist(this);
    //创建数据库类
    dbptr=DbManager::getDbManager();
    //同步音量
    ui->horizontalSlider_sound->setValue(player->volume());
    //时间进度条是否被按下设置初值
    isSliderDragging = false;
    //是否开始播放音乐设置初值
    isMusicPlaying=false;
    // 连接信号和槽 实现音乐进度条绑定
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::onMediaDurationChanged);//媒体文件的总时长发生改变时被发射
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::onMediaPositionChanged);//媒体文件的当前播放位置发生改变时
    // 连接播放列表的 currentIndexChanged 信号到槽函数
    connect(playList, &QMediaPlaylist::currentIndexChanged, this, &MainWindow::on_playlist_currentIndexChanged);
    //传递登录用户名
    connect(this, &MainWindow::sendValue, this, &MainWindow::receiveValue);
    setGifBackground("D:\\code\\worksapce_QT\\QT_0328\\Test_0328_mediaPlayer\\picture\\gif\\j1pbp-l5bw2.gif");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setGifBackground(const QString &gifPath)//背景设置
{
    backgroundMovie = new QMovie(gifPath, QByteArray(), this);//创建gif
    if (backgroundMovie->isValid()) {
        backgroundLabel = new QLabel(this);
        backgroundLabel->setMovie(backgroundMovie);//将 QMovie 对象 backgroundMovie 关联到 QLabel 上
        backgroundLabel->setScaledContents(true);//设置自动缩放其内容以适应 QLabel 的大小。
        backgroundLabel->setGeometry(0, 0, this->width(), this->height());//全部覆盖
        backgroundLabel->lower();//置为底层
        backgroundMovie->setSpeed(180);
        backgroundMovie->start();
    } else {
        qDebug() << "Invalid GIF file: " << gifPath;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)//重写信号函数，当窗口大小改变时，会自动调用该函数
{
    if (backgroundLabel) {
        backgroundLabel->setGeometry(0, 0, this->width(), this->height());
    }
    QMainWindow::resizeEvent(event);//调用基类 QMainWindow 的 resizeEvent 函数，以确保基类对窗口大小改变事件的默认处理逻辑能够正常执行。
}
void MainWindow::receiveValue(QString username){
    userName=username;
    userId=dbptr->nameToId(username);
    qDebug()<<"name="<<username<<"id="<<userId;
    ui->label_user->setText("当前用户："+userName);
}
void MainWindow::setInit()//初始化容器
{
    dbptr->setInit(historySet,userId);
    for(MusicInfo info :historySet){
        ui->listWidget_musicHistory->addItem(info.music_name);
    }

}
void MainWindow::on_pushButton_chooseMusicList_clicked()//1，选择播放列表
{
    QString dirPath=QFileDialog::getExistingDirectory(this,"选择本地音乐库","D:\\华清远见\\临时文件\\music");
//    QString dirPath=QFileDialog::getExistingDirectory(this,"选择本地音乐库","D:");
    if(dirPath==""){
        return;
    }
    //打开文件
    QDir dir(dirPath,"*.mp3");
    QStringList musicList=dir.entryList(QDir::Files,QDir::Name);
    //插入list
    for(auto musicPath:musicList){
//        qDebug()<<dirPath+musicPath;
//        playList->addMedia(QUrl(dirPath+"/"+musicPath));//注意路径拼接
            playList->addMedia(QUrl::fromLocalFile(dirPath+"/"+musicPath));//注意路径拼接
            qDebug()<<QUrl::fromLocalFile(dirPath+"/"+musicPath);
//        QString fullPath = dir.filePath(musicPath); // 使用 QDir::filePath 方法拼接路径
//        playList->addMedia(QUrl::fromLocalFile(fullPath)); // 使用 QUrl::fromLocalFile 构造 QUrl

        //直接使用 QUrl 的构造函数来处理本地文件路径可能会出现问题，
        //因为 QUrl 的构造函数会尝试解析输入的字符串为标准的 URL 格式，
        //而本地文件路径可能不符合这种格式。QUrl::fromLocalFile 方法会正确处理本地文件路径，将其转换为符合 QUrl 规范的对象。
    }
    player->setPlaylist(playList);
    playList->setCurrentIndex(0);
    playList->setPlaybackMode(QMediaPlaylist::Loop);//初始为列表循环
//打印音乐列表
    ui->listWidget_musicList->clear();
    ui->listWidget_musicList->addItems(musicList);
    //初始化容器
    setInit();
    isMusicPlaying = true; // 开始播放，设置标志位为 true
}

void MainWindow::on_pushButton_startAndEnd_clicked()//开始/暂停
{
    addToMusicHistory(playList->currentIndex());//添加历史
    if(player->state()==QMediaPlayer::PlayingState){
        player->pause();
        ui->pushButton_startAndEnd->setIcon(QIcon(":/picture/play.png"));
    }else {
        player->play();
        ui->pushButton_startAndEnd->setIcon(QIcon(":/picture/pause.png"));
        isMusicPlaying = true; // 开始播放，设置标志位为 true
        updatePlayingItemBackground(playList->currentIndex());//改背景
    }
}

void MainWindow::on_pushButton_nextMusic_clicked()//下一曲
{
    if (playList->nextIndex() != -1) {
        ui->pushButton_startAndEnd->setIcon(QIcon(":/picture/pause.png"));
//        addToMusicHistory(playList->currentIndex());//添加历史
        playList->next();
        player->play();
    }
}

void MainWindow::on_pushButton_priMusic_clicked()//上一曲
{
    if (playList->previousIndex() != -1) {
        ui->pushButton_startAndEnd->setIcon(QIcon(":/picture/pause.png"));
//        addToMusicHistory(playList->currentIndex());//添加历史
        playList->previous();
        player->play();
    }
}

void MainWindow::on_horizontalSlider_sound_valueChanged(int value)//音量
{
    player->setVolume(value);
}
void MainWindow::onMediaDurationChanged(qint64 duration)//设置滑块长度
{
    // 根据音乐总时长设置滑块的最大值
    ui->horizontalSlider_time->setMaximum(static_cast<int>(duration));
}

void MainWindow::onMediaPositionChanged(qint64 position)//更新进度条
{
    if (!isSliderDragging) {
        // 阻止滑块的信号触发，避免循环调用
        ui->horizontalSlider_time->blockSignals(true);
        // 根据当前播放位置更新滑块的值
        ui->horizontalSlider_time->setValue(static_cast<int>(position));
        updateTimeShow(position);//更新音乐显示
        ui->horizontalSlider_time->blockSignals(false);
    }
}
void MainWindow::on_horizontalSlider_time_valueChanged(int value)//根据进度条修改音乐位置
{
//    player->setPosition(value);
    if (!isSliderDragging) {
        player->setPosition(value);
    }
}
////////////////////////////
void MainWindow::on_playlist_currentIndexChanged(int index)//改变音乐
{
    if (index != -1&& playList->mediaCount() > index) {
        qDebug()<<"检测到音乐切换";
        // 获取当前播放音乐的媒体对象
        QMediaContent currentMedia = playList->media(index);
        // 获取音乐文件的路径
        QString filePath = currentMedia.request().url().toLocalFile();
        // 从文件路径中提取文件名
        QString musicName = QFileInfo(filePath).fileName();

        qDebug()<<filePath<<musicName;
        // 更新标签的文本
        ui->label_musicName->setText("歌名"+musicName);
        // 更新播放项的背景
        updatePlayingItemBackground(index);
        // 只有在真正播放音乐时才添加到历史记录
        if (isMusicPlaying) {
            addToMusicHistory(index);
        }
    }
}
void MainWindow::updateTimeShow(qint64 position)//更新显示歌曲时长
{
    //post->传参->歌曲当前位置
    int musicLength=player->duration();//获取音乐总时长
    QString str=QString("[%1:%2   /   %3:%4]").arg(position/1000/60).arg(position/1000%60).arg(musicLength/1000/60).arg(musicLength/1000%60);
//    qDebug()<<str;
    ui->label_musicTime->setText("歌曲时长                   :"+str);
}

void MainWindow::on_comboBox_c_currentIndexChanged(int index)//检测下拉框是否被修改
{
    switch (index) {
    case 0: // 顺序播放
//        playList->setPlaybackMode(QMediaPlaylist::Sequential);
        playList->setPlaybackMode(QMediaPlaylist::Loop);
        qDebug() << "设置为顺序播放";
        break;
    case 1: // 随机播放
        playList->setPlaybackMode(QMediaPlaylist::Random);
        qDebug() << "设置为随机播放";
        break;
    case 2: // 循环播放
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        qDebug() << "设置为单曲循环";
        break;
    default:
        break;
    }
}

void MainWindow::on_horizontalSlider_time_sliderPressed()//按下进度条
{
     isSliderDragging = true;
}

void MainWindow::on_horizontalSlider_time_sliderReleased()//松开进度条
{
    isSliderDragging = false;
    player->setPosition(ui->horizontalSlider_time->value());
}

void MainWindow::on_listWidget_musicList_itemDoubleClicked(QListWidgetItem *item)//双击播放功能
{

    // 获取双击项在列表中的索引
    int index=ui->listWidget_musicList->row(item);
    if (index != -1 && index < playList->mediaCount()) {
        addToMusicHistory(index);//添加历史
        // 设置播放列表的当前项为双击项对应的索引
        playList->setCurrentIndex(index);
        // 开始播放音乐
        player->play();
        ui->pushButton_startAndEnd->setIcon(QIcon(":/picture/pause.png"));
    }

}

void MainWindow::updatePlayingItemBackground(int index)//更新背景
{
    // 所有列表项的背景恢复默认
    for (int i = 0; i < ui->listWidget_musicList->count(); ++i) {
        QListWidgetItem *item = ui->listWidget_musicList->item(i);
        item->setBackground(Qt::transparent);
    }

    // 当前播放项的背景设置为深色
    if (index >= 0 && index < ui->listWidget_musicList->count()) {
        QListWidgetItem *currentItem = ui->listWidget_musicList->item(index);
        currentItem->setBackground(QColor(200, 200, 200)); // 深色
    }
}

void MainWindow::addToMusicHistory(int index)//添加历史记录
{
    qDebug()<<"index"<<index;
    if (index >= 0 && index < playList->mediaCount()) {
        QMediaContent currentMedia = playList->media(index);//当前项
        QString filePath = currentMedia.request().url().toLocalFile();
        QString musicName = QFileInfo(filePath).fileName();//歌名

        // 检查是否已经存在该音乐
//        bool exists = false;
//        for (int i = 0; i < ui->listWidget_musicHistory->count(); ++i) {
//            if (ui->listWidget_musicHistory->item(i)->text() == musicName) {
//                exists = true;
//                break;
//            }
//        }
//        if (!exists) {
//            //添加到历史列表
//            ui->listWidget_musicHistory->addItem(musicName);
//        }
        MusicInfo info;
        info.user_id=userId;
        info.music_name=musicName;
        qDebug()<<"count::"<<historySet.count(info);
        if(historySet.count(info)==0){
            bool ok=dbptr->insertHistory(info);//插入数据库
            if(ok){
                ui->listWidget_musicHistory->addItem(musicName);//页面插入
                historySet.insert(info);
            }
        }else{
            dbptr->updateTime(info);
        }
    }
}





void MainWindow::on_pushButton_changeUser_clicked()//切换用户
{
    QSettings *set=new QSettings("user.ini",QSettings::IniFormat);
    set->setValue("user/username","");
    set->setValue("user/password","");
    this->close();
    Login *l=new Login;
}

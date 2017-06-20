#include "soundsplayer.h"
#include "ui_soundsplayer.h"


SoundsPlayer::SoundsPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SoundsPlayer)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//实现透明效果
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/res/ic.jpg"));
    this->setWindowTitle("音乐播放界面");
    PlayState = false;
    playMode = 0;//用于播放模式
    player = new QMediaPlayer(this);
    songlist = new QMediaPlaylist;
    songlist->setPlaybackMode((QMediaPlaylist::Loop));//项目的顺序。
    player->setPlaylist(songlist);
    player->setVolume(60);
    curr_path = "";
    connect(player,SIGNAL(metaDataChanged()),this,SLOT(songMetaDataChanged()));
}

SoundsPlayer::~SoundsPlayer()
{
    delete ui;
}


void SoundsPlayer::mousePressEvent(QMouseEvent *event)//只针对Windows平台，用Windows下的标准方法，模拟标题栏移动消息，既简单又高效
{
    if (ReleaseCapture()) SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

void SoundsPlayer::recvData(int code,QString data){
    switch(code){
    case 0xF1:
        startPlayer();
        break;
    case 0xF2:
        stopPlayer();
        break;
    case 0xF3:
        songlist->setCurrentIndex(data.toInt(0,10));
        PlayState = true;
        player->play();
        ui->play_curr->setIcon(QIcon(":/icon/res/pause_hover1.png"));
        emit sendData(0xF1,"play");
        break;
    case 0xF4://添加歌曲
        songlist->addMedia(QUrl::fromLocalFile(data));
        break;
    case 0xF5:
        songlist->removeMedia(data.toInt());
        break;
    case 0xF6://上一首
        if((songlist->currentIndex()-1)<0){
            songlist->setCurrentIndex(songlist->mediaCount()-1);
        }else{
            songlist->setCurrentIndex(songlist->currentIndex()-1);
        }
        break;
    case 0xF7://下一首
        if((songlist->currentIndex()+1)==songlist->mediaCount()){
            songlist->setCurrentIndex(0);
        }else{
            songlist->setCurrentIndex(songlist->currentIndex()+1);
        }
        break;
    default:
        return;
    }
}
void SoundsPlayer::startPlayer(){
    player->play();
    ui->play_curr->setIcon(QIcon(":/icon/res/pause_hover1.png"));
}
void SoundsPlayer::stopPlayer(){
    player->pause();
    ui->play_curr->setIcon(QIcon(":/icon/res/play1_hover1.png"));
}

void SoundsPlayer::signalRegister(QWidget *parent){//信号寄存器
    connect(this,SIGNAL(sendData(int,QString)),parent,SLOT(recvData(int,QString)));
}


//初始化和更新歌曲信息(歌手和歌名)
void SoundsPlayer::songMetaDataChanged()
{
    QStringList info;
    QString author = player->metaData("Author").toString();
    if (!author.isEmpty())
        info += author;
    QString title = player->metaData("Title").toString();
    if (!title.isEmpty())
        info += title;
    if (!info.isEmpty())
        ui->lbl_song->setText(info.join(tr(" - ")));
}

void SoundsPlayer::on_btn_close_clicked()//关闭右边的界面
{
    emit sendData(0xFF,"window hidden");
    this->hide();
}


void SoundsPlayer::on_play_curr_clicked()
{
    if(PlayState){
        stopPlayer();
        emit sendData(0xF2,"pause");
    }else{
        startPlayer();
        emit sendData(0xF1,"play");
    }
}

void SoundsPlayer::on_play_next_clicked()//下一首
{
    if((songlist->currentIndex()+1)==songlist->mediaCount()){
        songlist->setCurrentIndex(0);
    }else{
        songlist->setCurrentIndex(songlist->currentIndex()+1);
    }
}

void SoundsPlayer::on_play_prev_clicked()//上一首
{
    if((songlist->currentIndex()-1)<0){
        songlist->setCurrentIndex(songlist->mediaCount()-1);
    }else{
        songlist->setCurrentIndex(songlist->currentIndex()-1);
    }
}

void SoundsPlayer::on_play_mode_clicked()//选择模式
{
    playMode++;
    switch(playMode){
    case 1:
        songlist->setPlaybackMode(QMediaPlaylist::Sequential);
        ui->play_mode->setText("顺序");
        break;
    case 2:
        songlist->setPlaybackMode((QMediaPlaylist::CurrentItemInLoop));
        ui->play_mode->setText("单曲");
        break;
    case 3:
        songlist->setPlaybackMode((QMediaPlaylist::Random));
        ui->play_mode->setText("随机");
        break;
    default:
        playMode = 0;
        songlist->setPlaybackMode((QMediaPlaylist::Loop));
        ui->play_mode->setText("列表");
    }
}


void SoundsPlayer::playerExit(){
    count++;
    if(count>0)
        exit(0);
}

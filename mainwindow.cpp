#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowIcon(QIcon(":/icon/res/111timg.jpg"));
    this->setWindowTitle("音乐播放列表");

    playlist = ui->playlist;

    initData(true);
    initList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

////把左右两个同步
void MainWindow::initData(bool first){
    if(first){
        spw = new SoundsPlayer();
        connect(this,SIGNAL(sendData(int,QString)),spw,SLOT(recvData(int,QString)));
    }
}


void MainWindow::initList(){
    QFile f("songlist.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    QTextStream reader(&f);
    QString path;
    while(!reader.atEnd()){
        path = reader.readLine();
        if(!path.isEmpty())  {
            QString tmp = path.split("/").last();
            tmp = tmp.split(".").first();
            playlist->addItem(tmp);
            templist << path;
            emit sendData(0xF4,path);
        }
    }
    f.close();
}

//保存列表
void MainWindow::saveList(){
    QFile f("songlist.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    QTextStream writer(&f);
    int count = templist.count();
    for(int i = 0;i<count;i++)
        writer<<templist.at(i)<<endl;
    f.close();
}

//删除功能
void MainWindow::deleList(){
    int r = playlist->currentIndex().row();
    if(r<0)
        return;
    templist.removeAt(r);
    playlist->takeItem(r);
    saveList();
    emit sendData(0xF5,QString::number(r));
}
void MainWindow::on_btn_close_clicked()
{
    if(isCreated)
        spw->close();
    exit(0);
}
void MainWindow::on_playlist_doubleClicked(const QModelIndex &index)
{
        if(spw->isHidden()){
            initData();
            spw->show();
            ui->playstate->setText("隐藏播放界面");
             ui->play_curr->setIcon(QIcon(":/icon/res/gtk-media-pause.png"));
        }
    else{
        initData();
        ui->playstate->setText("隐藏播放界面");
        ui->play_curr->setIcon(QIcon(":/icon/res/gtk-media-pause.png"));
    }
    emit sendData(0xF3,QString::number(index.row()));
}

void MainWindow::mousePressEvent(QMouseEvent *event)  //用鼠标移动界面
{
    if (ReleaseCapture()) SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (ReleaseCapture()){ SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event){//一个自带的类，右键出现一个菜单
            QMenu* popMenu = new QMenu(this);
            popMenu->setStyleSheet("border:1px solid rgb(160,160,160);color:rgb(80,80,80);");
            QAction *add = new QAction("添加", this);
            QAction *del = new QAction("删除", this);
            connect(add, SIGNAL(triggered(bool)), this, SLOT(on_additem_clicked()));
            connect(del, SIGNAL(triggered(bool)), this, SLOT(deleList()));
            popMenu->addAction(add);
            popMenu->addAction(del);
            popMenu->exec(QCursor::pos());//QAction * QMenu :: exec（）
}
void MainWindow::recvData(int code, QString data){
    switch(code){
    case 0xF1:
        PlayState = true;
        ui->play_curr->setIcon(QIcon(":/icon/res/gtk-media-pause.png"));
        break;
    case 0xF2:
        PlayState = false;
        ui->play_curr->setIcon(QIcon(":/icon/res/gtk-media-play-ltr.png"));
        break;
    case 0xFF:
        ui->playstate->setText("显示播放界面");
        break;

    default:
        return;
    }
}
void MainWindow::on_playstate_clicked()
{
        if(spw->isHidden()){
            spw->show();
            ui->playstate->setText("隐藏播放界面");
        }else{
            spw->hide();
            ui->playstate->setText("显示播放界面");
        }
}
void MainWindow::on_play_curr_clicked()
{
    if(PlayState){
        PlayState = false;
        ui->play_curr->setIcon(QIcon(":/icon/res/gtk-media-play-ltr.png"));
        emit sendData(0xF2,"pause");
    }else{
        PlayState = true;
        ui->play_curr->setIcon(QIcon(":/icon/res/gtk-media-pause.png"));
        emit sendData(0xF1,"play");
    }
}


void MainWindow::on_play_prev_clicked()
{
    emit sendData(0xF6,"prev");
}
void MainWindow::on_play_next_clicked()
{
    emit sendData(0xF7,"next");
}
void MainWindow::on_additem_clicked()
{
    QString DirName = QDir::currentPath();
    QStringList pathList = QFileDialog::getOpenFileNames(this, tr("选择文件"), DirName, tr("*.mp3 *.wma *.m4a *.ogg *.aac *.ape"));
    for(int i=0; i<pathList.size(); i++) {
        QString path = pathList.at(i);
        if(!path.isEmpty())  {
            QString tmp = path.split("/").last();
            tmp = tmp.split(".").first();
            playlist->addItem(tmp);
            templist << path;
            emit sendData(0xF4,path);
        }
    }
    saveList();
    int count = ui->playlist->count();
    for(int i = 0; i < count; i++) {
        QListWidgetItem *item = ui->playlist->item(i);
    }
}


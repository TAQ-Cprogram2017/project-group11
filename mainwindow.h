#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "soundsplayer.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <windows.h>
#include <QListWidget>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void on_btn_close_clicked();
    void on_playstate_clicked();
    void recvData(int code,QString data);
    void on_playlist_doubleClicked(const QModelIndex &index);
    void on_play_curr_clicked();
    void on_play_prev_clicked();
    void on_play_next_clicked();
    void on_additem_clicked();
    void deleList();


signals:
    void sendData(int code,QString data);

private:
    void initData(bool first = false);
    void initList();
    void saveList();
    Ui::MainWindow *ui;
    QListWidget *playlist;

    SoundsPlayer *spw;
    bool isCreated;
    bool PlayState;
    QStringList templist;
};

#endif // MAINWINDOW_H

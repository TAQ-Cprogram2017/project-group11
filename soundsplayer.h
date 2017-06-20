#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include <QMainWindow>
#include <QMouseEvent>
#include <windows.h>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QInputDialog>

namespace Ui {
class SoundsPlayer;
}

class SoundsPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit SoundsPlayer(QWidget *parent = 0);
    ~SoundsPlayer();
    void signalRegister(QWidget *parent);

    void mousePressEvent(QMouseEvent *event);

public slots:
    void recvData(int code,QString data);
    void on_btn_close_clicked();
    void on_play_curr_clicked();
    void on_slider_progress_sliderMoved(int position);

    void songMetaDataChanged();
    void on_play_next_clicked();
    void on_play_prev_clicked();

    void playerExit();

    void on_play_mode_clicked();


signals:
    void sendData(int code,QString data);

private:
    Ui::SoundsPlayer *ui;
    bool PlayState;
    QMediaPlayer *player;
    QMediaPlaylist *songlist;//The QMediaPlaylist class provides a list of media content to play.
    int position;
    int playMode;
    int count;
    QString curr_path;
    void startPlayer();
    void stopPlayer();
};

#endif // SOUNDSPLAYER_H

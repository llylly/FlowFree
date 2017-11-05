#include "selectwindow.h"
#include "record.h"
#include <QApplication>
#include <ctime>
#include <QMediaPlayer>
#include <QMediaPlaylist>

record *Record;
QMediaPlayer *backPlayer;


int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    QMediaPlaylist *playList = new QMediaPlaylist();
    playList->addMedia(QUrl::fromLocalFile("sound/backMusic_2.mp3"));
    playList->addMedia(QUrl::fromLocalFile("sound/backMusic_new.mp3"));
    playList->setCurrentIndex(1);
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    backPlayer = new QMediaPlayer();
    backPlayer->setVolume(50);
    backPlayer->setPlaylist(playList);
    backPlayer->play();

    Record = new record;
    SelectWindow w;
    w.show();

    return a.exec();
}

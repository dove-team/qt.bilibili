#pragma once
#include <QImage>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QVector>
#include <QList>
#include <QMenu>
#include <QFileDialog>
#include <QWidget>
#include <QPainter>
#include <QCloseEvent>
#include <QMainWindow>
#include <QRegularExpression>
#include "videodecoder.h"
#include "ui_FfmpegView.h"

class FfmpegView : public QWidget
{
    Q_OBJECT
public:
    FfmpegView(QWidget* parent = nullptr);
    ~FfmpegView();
    void showControl(bool show);
    void selectedVideoFile(QString file, QString type);
    void stopVideo();
    void pauseVideo();

private:
    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    bool eventFilter(QObject* obj, QEvent* event);
    QString fileType(QString file);
    void addPathVideoToList(QString path);
    void playVideo(QString file);
    void playNext();
    void playPreview();
    void showPlayMenu();
    void setHide(QWidget* widget);
    inline QString getFilenameFromPath(QString path);
    Ui::FfmpegViewClass* ui;
    VideoDecoder* decoder;
    QList<QString> playList;
    QString currentPlay;
    QString currentPlayType;
    QTimer* menuTimer;
    QTimer* progressTimer;
    bool menuIsVisible;
    bool isKeepAspectRatio;
    QImage image;
    bool autoPlay;
    bool loopPlay;
    bool closeNotExit;
    VideoDecoder::PlayState playState;
    QVector<QWidget*> hideVector;
    qint64 timeTotal;
    int seekInterval;
private slots:
    void buttonClickSlot();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void timerSlot();
    void seekProgress(int value);
    void videoTime(qint64 time);
    void playStateChanged(VideoDecoder::PlayState state);
    void setFullScreen();
    void setKeepRatio();
    void setAutoPlay();
    void setLoopPlay();
    void saveCurrentFrame();
    void showVideo(QImage);
};

#ifndef DECODER_H
#define DECODER_H
#include <QThread>
#include <QImage>
#include "qt.player_global.h"
extern "C"
{
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/pixfmt.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libavutil/imgutils.h"
}
#include "audiodecoder.h"

class QT_PLAYER_EXPORT VideoDecoder : public QThread
{
    Q_OBJECT
public:
    enum PlayState {
        STOP,
        PAUSE,
        PLAYING,
        FINISH
    };
    explicit VideoDecoder();
    ~VideoDecoder();
    double getCurrentTime();
    void seekProgress(qint64 pos);
    int getVolume();
    void setVolume(int volume);
private:
    void run();
    void clearData();
    void setPlayState(VideoDecoder::PlayState state);
    void displayVideo(QImage image);
    static int videoThread(void* arg);
    double synchronize(AVFrame* frame, double pts);
    bool isRealtime(AVFormatContext* pFormatCtx);
    int initFilter();
    int fileType;
    int videoIndex;
    int audioIndex;
    int subtitleIndex;
    QString currentFile;
    QString currentType;
    qint64 timeTotal;
    AVPacket seekPacket;
    qint64 seekPos;
    double seekTime;
    PlayState playState;
    bool isStop;
    bool gotStop;
    bool isPause;
    bool isSeek;
    bool isReadFinished;
    bool isDecodeFinished;
    AVFormatContext* pFormatCtx;
    AVCodecContext* pCodecCtx;
    AvPacketQueue videoQueue;
    AvPacketQueue subtitleQueue;
    AVStream* videoStream;
    double videoClk;
    AudioDecoder* audioDecoder;
    AVFilterGraph* filterGraph;
    AVFilterContext* filterSinkCxt;
    AVFilterContext* filterSrcCxt;
public slots:
    void decoderFile(QString file, QString type);
    void stopVideo();
    void pauseVideo();
    void audioFinished();
signals:
    void readFinished();
    void gotVideo(QImage image);
    void gotVideoTime(qint64 time);
    void playStateChanged(VideoDecoder::PlayState state);
};
#endif 
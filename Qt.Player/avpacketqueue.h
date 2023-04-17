#ifndef AVPACKETQUEUE_H
#define AVPACKETQUEUE_H
#include <QQueue>
#include "qt.player_global.h"
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}
#include "SDL2/SDL.h"

class QT_PLAYER_EXPORT AvPacketQueue
{
public:
    explicit AvPacketQueue();
    void enqueue(AVPacket* packet);
    void dequeue(AVPacket* packet, bool isBlock);
    bool isEmpty();
    void empty();
    int queueSize();

private:
    SDL_mutex* mutex;
    SDL_cond* cond;

    QQueue<AVPacket> queue;
};

#endif // AVPACKETQUEUE_H
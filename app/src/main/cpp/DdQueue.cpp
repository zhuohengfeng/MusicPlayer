//
// Created by hengfeng zhuo on 2019/3/28.
//

#include "DdQueue.h"
#include "AndroidLog.h"

DdQueue::DdQueue(DdPlayStatus* status) {
    this->pPlayStatus = status;
    pthread_mutex_init(&this->mutexPacket, NULL);
    pthread_cond_init(&this->condPacket, NULL);
}

DdQueue::~DdQueue() {

}

/**
 * 添加packet
 * @return
 */
int DdQueue::putAvPacket(AVPacket *packet_in) {
    pthread_mutex_lock(&this->mutexPacket);
    this->queuePacket.push(packet_in);
    LOGD("放入一个AVpacket到队里里面， 个数为：%d", this->queuePacket.size());
    pthread_cond_signal(&this->condPacket);
    pthread_mutex_unlock(&this->mutexPacket);
    return 0;
}

/**
 * 获取队列中的首个packet
 * @return
 */
int DdQueue::getAvPacket(AVPacket *packet_out) {
    pthread_mutex_lock(&this->mutexPacket);

    while(this->pPlayStatus != NULL && !this->pPlayStatus->exit) {
        if (this->queuePacket.size() > 0) {
            AVPacket* packet = this->queuePacket.front();
            if (av_packet_ref(packet_out, packet) == 0) {
                this->queuePacket.pop();
            }
            av_packet_free(&packet);
            av_free(packet);
            packet = NULL;
            LOGD("从队列里面取出一个AVpacket，还剩下 %d 个", this->queuePacket.size());
            break;
        }
        else {
            pthread_cond_wait(&this->condPacket, &this->mutexPacket);
        }
    }

    pthread_mutex_unlock(&this->mutexPacket);
    return 0;
}

/**
 * 返回队列中的AvPacket的大小
 * @return
 */
int DdQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&this->mutexPacket);
    size = this->queuePacket.size();
    pthread_mutex_unlock(&this->mutexPacket);
    return size;
}

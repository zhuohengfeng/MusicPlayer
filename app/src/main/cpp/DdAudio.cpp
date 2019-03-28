//
// Created by hengfeng zhuo on 2019/3/27.
//

#include "DdAudio.h"

DdAudio::DdAudio(DdPlayStatus* status) {
    this->pPlayStatus = status;
    this->pQueue = new DdQueue(status);
}

DdAudio::~DdAudio() {

}

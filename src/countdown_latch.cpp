//
// Created by 杨雪敏 on 2018/10/30.
//

#include "countdown_latch.h"


void CountdownLatch::Await() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_)
        cv_.wait(lock);
}

void CountdownLatch::Countdown() {
    std::unique_lock<std::mutex> lock(mutex_);
    count_--;
    if (count_ <= 0) {
        cv_.notify_all();
    }
}


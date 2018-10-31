//
// Created by 胡昊坤 on 2018/10/24.
//

#include <thread>
#include "time_wheel.h"

using namespace std;

TimeWheel::TimeWheel(size_t wheelSize, long wheelIntervalMicroSeconds): wheelSize_(wheelSize),
                                                                     wheelIntervalMicroSeconds_(wheelIntervalMicroSeconds),
                                                                     isRunning_(true), loopThread_(&TimeWheel::Loop, this){
    slots_.resize(wheelSize);
}

TimeWheel::~TimeWheel() {
    isRunning_ = false;
    if (loopThread_.joinable()) {
        loopThread_.join();
    }
}

bool TimeWheel::Add(const std::shared_ptr<TimerTask>& pTask) {
    if (pTask->expiredTime > wheelIntervalMicroSeconds_ * wheelSize_) {
        LOG(ERROR) << "time range overflow, max timestamp in microseconds: " << wheelIntervalMicroSeconds_ * wheelSize_
                   << " , got " << pTask->expiredTime;
        return false;
    } else if (pTask->IsCanceled()) {
        VLOG(1) << "the task is canceled";
        return false;
    }

    std::unique_lock<std::mutex> lock(mutex_);
    int slotNum = int (pTask->expiredTime / wheelIntervalMicroSeconds_ + cursor_) % int(slots_.size());

    assert(slotNum < slots_.size());
    slots_[slotNum].push_back(pTask);
    return true;
}


void TimeWheel::Loop() {
    while (isRunning_) {
        auto nextRound = std::chrono::system_clock::now() + std::chrono::microseconds(wheelIntervalMicroSeconds_);

        {
            std::unique_lock<std::mutex> lock(mutex_);
            for (auto& taskPtr: slots_[cursor_]) {
                taskPtr->onTrigger();
            }
            slots_[cursor_].clear();
            cursor_ ++;
        }

        std::this_thread::sleep_until(nextRound);
    }
}


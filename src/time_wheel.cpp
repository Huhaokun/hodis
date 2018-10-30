//
// Created by 胡昊坤 on 2018/10/24.
//

#include <thread>
#include "time_wheel.h"

using namespace std;

TimeWheel::TimeWheel(size_t wheelSize, long wheelIntervalMicroSeconds): wheelSize_(wheelSize_),
                                                                     wheelIntervalMicroSeconds_(wheelIntervalMicroSeconds) {
    slots_.resize(wheelSize);
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
    int slotNum = int (pTask->expiredTime / wheelIntervalMicroSeconds_);

    assert(slotNum < slots_.size());
    slots_[slotNum].push_back(pTask);
    return true;
}

void TimeWheel::Run() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(wheelIntervalMicroSeconds_ * 1000));

        cursor_++;
    }
}

//
// Created by 胡昊坤 on 2018/10/24.
//

#ifndef HODIS_TIME_WHEEL_H
#define HODIS_TIME_WHEEL_H

#include <chrono>
#include <vector>
#include <functional>
#include <list>
#include <mutex>
#include <map>
#include <atomic>
#include <glog/logging.h>

struct TimerTask {
public:
    TimerTask(long expiredTime, std::function<void()>&& f): expiredTime(expiredTime), onTrigger(std::move(f)),
                                                            isCancel_(false) {LOG(INFO) << "TimerTask constructing..."; }
    ~TimerTask() {LOG(INFO) << "TimerTask destructing...";}

    void Cancel() {isCancel_ = true;}

    bool IsCanceled() {return isCancel_;}

    long expiredTime;
    std::function<void()> onTrigger;
private:
    std::atomic<bool> isCancel_;

};
// a simple single layer time wheel implement
class TimeWheel {
public:
    TimeWheel(size_t wheelSize, long wheelIntervalMicroSeconds);

    bool Add(const std::shared_ptr<TimerTask>& pTask);

    void Run();

private:
    std::mutex mutex_;
    std::vector<std::list<std::shared_ptr<TimerTask>>> slots_;
    long wheelIntervalMicroSeconds_;
    size_t wheelSize_;
    size_t cursor_ = 0;
};


#endif //HODIS_TIME_WHEEL_H

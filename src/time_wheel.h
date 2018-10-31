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
#include <thread>
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

    ~TimeWheel();

    // add a timer task to the wheel
    // @return true if add success
    bool Add(const std::shared_ptr<TimerTask>& pTask);

    void Start();

    void Stop();

private:
    void Loop();


    std::mutex mutex_;
    std::vector<std::list<std::shared_ptr<TimerTask>>> slots_;
    size_t wheelSize_;
    long wheelIntervalMicroSeconds_;
    std::atomic_bool isRunning_;
    size_t cursor_ = 0;

    std::thread loopThread_;

};


#endif //HODIS_TIME_WHEEL_H

//
// Created by 胡昊坤 on 2018/10/19.
//

#ifndef HODIS_DELAYQUEUE_H
#define HODIS_DELAYQUEUE_H

#include <queue>
#include <mutex>

template <typename T>
class DelayQueue {
private:
    std::priority_queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread::id leader_;
    bool isLeaderSet_;
public:
    DelayQueue() {}
    ~DelayQueue() {}

    bool Offer(T&& element) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(std::move(element));
        lock.unlock();
        cv_.notify_one();
        return true;
    }

    T Take() {
        std::unique_lock<std::mutex> lock(mutex_);
        while(true) {
            while (queue_.empty()) {
                cv_.wait(lock);
            }

            if (!lock.owns_lock()) {
                lock.lock();
                LOG(INFO) << "Lock again";
            }

            T element = queue_.top();
            long long delay = element.DelayTime();
            if (delay <= 0) {
                // this element expire
                queue_.pop();
                if (!isLeaderSet_ && !queue_.empty())
                    cv_.notify_one();
                return element;
            } else {
                // leader is waiting for head element
                if (isLeaderSet_) {
                    cv_.wait(lock);
                } else {
                    isLeaderSet_ = true;
                    leader_ = std::this_thread::get_id();
                    cv_.wait_for(lock, std::chrono::microseconds(delay));
                    if (leader_ == std::this_thread::get_id())
                        isLeaderSet_ = false;
                }
            }
        }
    }

};

#endif //HODIS_DELAYQUEUE_H

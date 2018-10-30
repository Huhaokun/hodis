//
// Created by Haokun.hu on 2018/10/30.
//

#ifndef HODIS_COUNTDOWNLATCH_H
#define HODIS_COUNTDOWNLATCH_H


#include <mutex>

class CountdownLatch {
public:
    CountdownLatch(int count): count_(count) {}

    void Await();

    void Countdown();

private:
    int count_;

    std::mutex mutex_;

    std::condition_variable cv_;
};

typedef std::shared_ptr<CountdownLatch> CountdownLatchptr;


#endif //HODIS_COUNTDOWNLATCH_H

//
// Created by 胡昊坤 on 2018/10/19.
//
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include <thread>

#include "delay_queue.hpp"

using namespace std;

class DelayedInt {
public:
    DelayedInt(int i, long long bornNanoSecond, long long expiredDuration): i_(i),
    bornNanoSecond_(bornNanoSecond), expiredDuration_(expiredDuration) {}

    long long DelayTime() {
        return bornNanoSecond_ + expiredDuration_ - std::chrono::system_clock::now().time_since_epoch().count();
    }

    bool operator < (const DelayedInt& delayInt) const {
        return bornNanoSecond_ + expiredDuration_ > delayInt.bornNanoSecond_ + delayInt.expiredDuration_;
    }

    int Value() {
        return i_;
    }

private:
    int i_;
    long long bornNanoSecond_;
    long long expiredDuration_;
};

TEST(DELAY_QUEUE, SINGLE_THREAD) {
    DelayQueue<DelayedInt> delayQueue;
    for (int i = 0; i < 10; i++) {
        LOG(INFO) << std::chrono::system_clock::now().time_since_epoch().count();
        DelayedInt delayedInt(i, std::chrono::system_clock::now().time_since_epoch().count(), 3000000);
        delayQueue.Offer(std::move(delayedInt));

        DelayedInt delayedInt2 = delayQueue.Take();

        LOG(INFO) << "value: " << delayedInt2.Value();
    }
}

TEST(DELAY_QUEUE, MULTI_THREAD) {
    DelayQueue<DelayedInt> delayQueue;

    std::thread t([&]() {
        for (int i = 0; i < 10; i++) {
            DelayedInt delayedInt = delayQueue.Take();
            LOG(INFO) << "value: " << delayedInt.Value();
        }
    });

    sleep(1);

    for (int i = 0; i < 10; i++) {
        DelayedInt delayedInt(i, std::chrono::system_clock::now().time_since_epoch().count(), 1000000 * i);
        delayQueue.Offer(std::move(delayedInt));
    }

    if (t.joinable())
        t.join();
}

TEST(DELAY_QUEUE, MULTI_CONSUME) {
    DelayQueue<DelayedInt> delayQueue;

    std::thread t1([&]() {
        for (int i = 0; i < 5; i++) {
            DelayedInt delayedInt = delayQueue.Take();
            LOG(INFO) << "thread 1 value: " << delayedInt.Value();
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 5; i++) {
            DelayedInt delayedInt = delayQueue.Take();
            LOG(INFO) << "thread 2 value: " << delayedInt.Value();
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
        }
    });

    sleep(1);
    for (int i = 0; i < 10; i++) {
        DelayedInt delayedInt(i, std::chrono::system_clock::now().time_since_epoch().count(), 1000000 * i);
        delayQueue.Offer(std::move(delayedInt));
    }

    t1.join();
    t2.join();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);

    return RUN_ALL_TESTS();
}
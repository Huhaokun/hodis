//
// Created by Haokun.hu on 2018/10/30.
//

#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include <thread>

#include "countdown_latch.h"

TEST(COUNTDOWNLATCH_TEST, BASE) {
    CountdownLatch latch(5);

    for (int i = 0; i < 5; i++) {
        std::thread([&latch, i]() {
            LOG(INFO) << "I am thread number " << i ;
            latch.Countdown();
        }).detach();
    }

    latch.Await();
    LOG(INFO) << "Main thread end wait";
}

TEST(COUNTDOWNLATCH_TEST, MULTI_WAIT) {
    CountdownLatch latch(5);

    for (int i = 0; i < 5; i++) {
        std::thread([&latch, i]() {
            LOG(INFO) << "I am thread number " << i ;
            sleep(i);
            LOG(INFO) << "thread number "<< i << " finish sleep";
            latch.Countdown();
            latch.Await();
            LOG(INFO) << "thread num " << i << " end wait";
        }).detach();
    }

    latch.Await();
    sleep(1);

}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);

    return RUN_ALL_TESTS();
}

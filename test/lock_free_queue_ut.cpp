//
// Created by 胡昊坤 on 2018/11/1.
//


#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <glog/logging.h>

#include "lock_free_queue.hpp"

TEST(LOCK_FREE_QUEUE, BASE) {
    LockFreeQueue<int> lockFreeQueue;
    for (int i = 0; i < 100; i++) {
        lockFreeQueue.EnQueue(i);
    }

    for (int i = 0; i < 100; i++) {
        int j = lockFreeQueue.DeQueue();
        EXPECT_EQ(j, i);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);

    return RUN_ALL_TESTS();
}
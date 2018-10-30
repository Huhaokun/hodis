set(TEST_LIB
        Gflags
        Gtest
        Glog)

add_executable(delay_queue_ut delay_queue_ut.cpp)
target_link_libraries(delay_queue_ut ${TEST_LIB})
#target_link_libraries(delay_queue_ut common_lib ${TEST_LIB})
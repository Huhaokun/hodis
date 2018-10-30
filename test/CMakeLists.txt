set(TEST_LIB
        Gflags
        Gtest)

add_executable(delay_queue_ut delay_queue_ut.cpp)
target_link_libraries(delay_queue_ut ${TEST_LIB})


add_executable(countdown_latch_ut countdown_latch_ut.cpp)
target_link_libraries(countdown_latch_ut common_lib ${TEST_LIB})

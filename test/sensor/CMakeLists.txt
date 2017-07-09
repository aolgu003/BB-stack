add_executable(imuTest imu/imuTest.cpp)
target_link_libraries(imuTest ${GTEST_BOTH_LIBRARIES} gtest_main)
target_link_libraries(imuTest imu)
target_include_directories(imuTest PUBLIC imu/)

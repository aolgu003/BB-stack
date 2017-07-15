add_executable(imuTest imu/imuTest.cpp)
target_link_libraries(imuTest ${GTEST_BOTH_LIBRARIES} gtest_main gmock_main)
target_link_libraries(imuTest mpu9250 )
target_include_directories(imuTest PUBLIC imu/)

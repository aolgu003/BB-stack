add_executable( ahrsTest "main.cpp")


target_link_libraries( ahrsTest BlackLibWrapper)
target_link_libraries( ahrsTest mpu9250)
target_link_libraries( ahrsTest  imu )


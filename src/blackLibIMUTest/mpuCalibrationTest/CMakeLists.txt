add_executable(imuCalTest "main.cpp")

target_link_libraries( imuCalTest BlackLibWrapper mpu9250 )

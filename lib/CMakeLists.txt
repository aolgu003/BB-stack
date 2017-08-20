add_library( madgwick MadgwickAHRS/MadgwickAHRS.c )

add_subdirectory( HAL )
add_subdirectory(sensors)

target_include_directories( imu PUBLIC HAL/)
target_include_directories( imu PUBLIC HAL/BlackLibWrapper/ )
target_include_directories( imu PUBLIC MadgwickAHRS/ )

target_include_directories( mpu9250 PUBLIC HAL/ )
target_include_directories( mpu9250 PUBLIC HAL/BlackLibWrapper/ )

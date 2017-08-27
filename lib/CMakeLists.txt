add_library( madgwick MadgwickAHRS/MadgwickAHRS.c )

add_subdirectory( HAL )
add_subdirectory(sensors)

#target_include_directories( imu PUBLIC HAL/)
#target_include_directories( imu PUBLIC HAL/BlackLibWrapper/ )M
#target_include_directories( imu PUBLIC MadgwickAHRS/ )M

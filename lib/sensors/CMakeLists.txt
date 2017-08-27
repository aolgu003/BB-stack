add_library( imu imu/imu.h )
add_library( mpu9250 imu/mpu9250/mpu9250.cpp)

target_link_libraries( mpu9250 halFactory)

target_include_directories( mpu9250 PUBLIC . )
target_include_directories( mpu9250 PUBLIC imu/ )
target_include_directories( mpu9250 PUBLIC imu/mpu9250/ )
target_include_directories( mpu9250 PUBLIC ../HAL/ )
target_include_directories( mpu9250 PUBLIC ../HAL/BlackLibWrapper/ )

target_link_libraries( imu madgwick mpu9250)

target_include_directories( imu PUBLIC . )
target_include_directories( imu PUBLIC imu/ )
target_include_directories( imu PUBLIC imu/mpu9250/ )
target_include_directories( imu PUBLIC ../HAL/ )
target_include_directories( imu PUBLIC ../HAL/BlackLibWrapper/ )
target_include_directories( imu PUBLIC ../MadgwickAHRS/ )


  add_library( imu imu/imu.cpp )
  add_library( mpu9250 imu/mpu9250/mpu9250.cpp)
  target_link_libraries( imu madgwick )

  target_include_directories( imu PUBLIC . )
  target_include_directories( imu PUBLIC imu/ )
  target_include_directories( imu PUBLIC imu/mpu9250/ )

  target_link_libraries( mpu9250 imu )
  target_include_directories( mpu9250 PUBLIC . )
  target_include_directories( mpu9250 PUBLIC imu/ )
  target_include_directories( mpu9250 PUBLIC imu/mpu9250/ )

if ( BUILD-AHRS OR BUILD-AHRS-UT OR BUILD-UNIT-TESTS )
  INCLUDE_DIRECTORIES("${gmock_SOURCE_DIR}/include")

  add_library( madgwick MadgwickAHRS/MadgwickAHRS.c )
  add_library( imu sensors/imu/imu.cpp )
  add_library( mpu9250 sensors/imu/mpu9250/mpu9250.cpp)


  target_link_libraries( imu madgwick )
  target_include_directories( imu PUBLIC HAL/)
  target_include_directories( imu PUBLIC sensors/ )
  target_include_directories( imu PUBLIC sensors/imu/ )
  target_include_directories( imu PUBLIC sensors/imu/mpu9250/ )
  target_include_directories( imu PUBLIC MadgwickAHRS/ )

  target_link_libraries( mpu9250 imu )
  target_include_directories( mpu9250 PUBLIC HAL/)
  target_include_directories( mpu9250 PUBLIC sensors/ )
  target_include_directories( mpu9250 PUBLIC sensors/imu/ )
  target_include_directories( mpu9250 PUBLIC sensors/imu/mpu9250/ )


  #add_library(HAL HAL/)

endif()

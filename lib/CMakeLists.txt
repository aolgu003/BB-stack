if ( BUILD-AHRS OR BUILD-AHRS-UT OR BUILD-UNIT-TESTS )
  add_library( ahrs MadgwickAHRS/MadgwickAHRS.c)

  add_library(imu sensors/imu/imu.cpp)
  #add_library(mpu9250 sensors/imu/mpu9250/)

  target_link_libraries( imu ahrs )
  target_include_directories( imu PUBLIC sensors/ )
  target_include_directories( imu PUBLIC sensors/imu )
  target_include_directories( imu PUBLIC sensors/imu/mpu9250 )
  target_include_directories( imu PUBLIC MadgwickAHRS/ )
endif()

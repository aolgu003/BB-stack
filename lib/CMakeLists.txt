add_library( madgwick MadgwickAHRS/MadgwickAHRS.c )
if ( BUILD-AHRS OR BUILD-AHRS-UT OR BUILD-UNIT-TESTS )
  INCLUDE_DIRECTORIES("${gmock_SOURCE_DIR}/include")

  add_subdirectory(sensors)

  target_include_directories( imu PUBLIC HAL/)
  target_include_directories( imu PUBLIC MadgwickAHRS/ )

  target_include_directories( mpu9250 PUBLIC HAL/)
endif()

if ( BUILD-BLACKLIB-TESTS )

endif()

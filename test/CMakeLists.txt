add_subdirectory(sensor)

add_test(
  AllTests sensor
)

add_test(
  AHRS imuTest
)

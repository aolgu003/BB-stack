add_subdirectory(sensor)

add_test(
  NAME AllTests COMMAND imuTest
)

add_test(
  NAME AHRS COMMAND imuTest
)

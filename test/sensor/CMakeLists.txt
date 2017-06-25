add_executable( sensor sensor.cpp)
target_link_libraries(sensor ${GTEST_BOTH_LIBRARIES} gtest_main)

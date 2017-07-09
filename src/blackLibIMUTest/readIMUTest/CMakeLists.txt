MESSAGE ( "INCLUDE_DIRECTORIES:   " ${BlackLib_SOURCE_DIR} )

add_executable(readIMUTest main.cpp)
target_link_libraries(readIMUTest black )
target_include_directories(readIMUTest PUBLIC ${BlackLib_SOURCE_DIR}/v3_0 )

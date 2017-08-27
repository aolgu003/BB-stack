add_subdirectory(BlackLibWrapper)

include_directories(HAL)
add_library(halFactory halfactory.cpp)
target_link_libraries(halFactory BlackLibWrapper black)
target_include_directories(halFactory PUBLIC .)
target_include_directories(halFactory PUBLIC BlackLibWrapper/)

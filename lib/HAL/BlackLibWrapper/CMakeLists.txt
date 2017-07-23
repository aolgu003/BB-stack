add_library( BlackLibWrapper blacklibwrapper.cpp )
target_include_directories( BlackLibWrapper PUBLIC ../ )
target_include_directories(BlackLibWrapper PUBLIC black )
target_link_libraries( BlackLibWrapper black )

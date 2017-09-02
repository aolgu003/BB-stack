add_executable(hello-zmq-server "main.cpp")    
target_link_libraries(hello-zmq-server ${cppzmq_LIBRARY})

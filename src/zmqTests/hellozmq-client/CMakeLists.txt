add_executable(hello-zmq-client "main.cpp")
target_link_libraries(hello-zmq-client ${cppzmq_LIBRARY})

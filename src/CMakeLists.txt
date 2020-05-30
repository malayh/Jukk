cmake_minimum_required(VERSION 3.1...3.16)

project(Jukk VERSION 1.0 LANGUAGES CXX)



add_library(protocol protocol/protocol.cpp)
add_library(util util.cpp)
target_include_directories(protocol PUBLIC ../include)
target_include_directories(util PUBLIC ../include)


add_executable(client client/client.cpp)
add_executable(server server/server.cpp)
add_executable(tests test.cpp)


target_link_libraries(client 
                        PUBLIC protocol
                        PUBLIC util
)
target_link_libraries(tests 
                        PUBLIC protocol
                        PUBLIC util
)
target_link_libraries(server 
                        PRIVATE protocol
                        pthread
)

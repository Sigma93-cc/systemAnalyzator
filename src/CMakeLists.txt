cmake_minimum_required(VERSION 3.18)

project(systemAnalyzator VERSION 1.0.0.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
                      Qt5::Core
                      Qt5::Widgets
                      Boost::filesystem)        

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
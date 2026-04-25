cmake_minimum_required(VERSION 3.18)

project(systemAnalyzator VERSION 1.0.0.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME} main.cpp window.h window.cpp window.ui)

target_link_libraries(${PROJECT_NAME} PRIVATE 
                      Qt5::Core
                      Qt5::Widgets
                      Boost::filesystem)        

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

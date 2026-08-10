cmake_minimum_required(VERSION 3.18)

project(systemanalyzator VERSION 1.0.0.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(spdlog REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME} src/main.cpp src/window.h src/window.cpp src/window.ui)

target_link_libraries(${PROJECT_NAME} PRIVATE 
                      Qt5::Core
                      Qt5::Widgets
                      Boost::filesystem
                      spdlog::spdlog)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES data/systemanalyzer.service DESTINATION ${CMAKE_INSTALL_PREFIX})

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")
    set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/deb")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    set(CPACK_PACKAGE_DESCRIPTION "SigmaStudio system soft")
    set(CPACK_PACKAGE_CONTACT "aqwerty362@gmail.com")
    set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
    message(STATUS "DEB ARCH: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
    set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}_${PROJECT_VERSION})
    set(PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})   
endif()

include (CPack)



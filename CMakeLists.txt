cmake_minimum_required(VERSION 3.18)

execute_process(
    COMMAND git describe --match=v*.*.*.* --abbrev=0 --tags --dirty
    OUTPUT_VARIABLE GIT_TAG
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(GIT_TAG)
    string(REPLACE "v" "" VERSION_FROM_GIT "${GIT_TAG}")
endif()

if(VERSION_FROM_GIT AND VERSION_FROM_GIT MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$")
    set(PROJECT_VERSION ${VERSION_FROM_GIT})
else()
    set(PROJECT_VERSION "1.0.0.0")
endif()

message(STATUS "System analyzer version: ${PROJECT_VERSION}")
project(systemanalyzer VERSION ${PROJECT_VERSION} LANGUAGES CXX)

configure_file(src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(spdlog REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SYSTEMD_UNIT_DIR "${CMAKE_INSTALL_PREFIX}/lib/systemd/system"
    CACHE PATH "Directory for systemd service files")

set(DISABLE_FULLSCREEN 0 CACHE BOOL "Enabling/disabling fullscreen logo")

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/versionchecker.cpp
    src/versionchecker.h
    src/window.h
    src/window.cpp
    src/window.ui
    src/version.h.in)

target_link_libraries(${PROJECT_NAME} PRIVATE 
                      Qt5::Core
                      Qt5::Widgets
                      Boost::filesystem)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES data/systemanalyzer.service DESTINATION ${SYSTEMD_UNIT_DIR})

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")
    set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/deb")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    set(CPACK_PACKAGE_DESCRIPTION "SigmaStudio system soft")
    set(CPACK_PACKAGE_CONTACT "---@---")
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



cmake_minimum_required(VERSION 3.16)
project(MSGenerator)

set(MSGenerator_VERSION_MAJOR 0)
set(MSGenerator_VERSION_MINOR 1)

# Check platforms
if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif(CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()

# Set RPATH
if(WINDOWS)
else()
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    MESSAGE( STATUS "CMAKE_INSTALL_RPATH: " "${CMAKE_INSTALL_PREFIX}/lib" )
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/MSGeneratorConfig.h.in"
        "${PROJECT_BINARY_DIR}/MSGeneratorConfig.h"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lcurl")
add_executable(MSGenerator main.cpp SpringInitializr.cpp SpringInitializr.h GenerateFiles.cpp GenerateFiles.h)

# Set installation directory
if(WINDOWS)
    set(CMAKE_INSTALL_PREFIX "e:/${PROJECT_NAME}")
    install (DIRECTORY  "${PROJECT_SOURCE_DIR}/platforms/win/bin/" DESTINATION bin)
elseif(LINUX)
    install (DIRECTORY  "${PROJECT_SOURCE_DIR}/platforms/linux/" DESTINATION lib)
elseif(MACOS)
    install (DIRECTORY  "${PROJECT_SOURCE_DIR}/platforms/macos/" DESTINATION lib)
endif()

install (TARGETS MSGenerator DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/MSGeneratorConfig.h" DESTINATION include)
install (DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include)
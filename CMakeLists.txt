cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0 CACHE STRING "iOS Deployment Target")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10 CACHE STRING "macOS Deployment Target")
    endif()
endif()

project(Jordmynt VERSION 0.0)

set(PROJECT_ABBR "jm_")
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(INFO "-msvc${MSVC_TOOLSET_VERSION}$<$<CONFIG:DEBUG>:-d>-x64-${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")
else()
    set(INFO "-msvc${MSVC_TOOLSET_VERSION}$<$<CONFIG:DEBUG>:-d>-x86-${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")
endif()

Set(Boost_ROOT "D:/Boost/stage/lib/cmake/")
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

Set(OPENSSL_ROOT_DIR "D:/OpenSSL/")
find_package(OpenSSL)

Set(wxWidgets_ROOT_DIR "D:/wxWidgets/")
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
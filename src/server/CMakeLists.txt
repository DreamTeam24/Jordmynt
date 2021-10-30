add_library(
    Server
    STATIC
    server.h
    server.cpp)

set_target_properties(
    Server PROPERTIES
    OUTPUT_NAME "lib${PROJECT_ABBR}server${INFO}")

target_link_libraries(
    Server
    PUBLIC
    Crypto
    Logger)

target_include_directories(
    Server
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    PUBLIC
    ${Boost_INCLUDE_DIRS})
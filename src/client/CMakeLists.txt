add_library(
    Client
    STATIC
    client.h
    client.cpp )

set_target_properties(
    Client PROPERTIES
    OUTPUT_NAME "lib${PROJECT_ABBR}client${INFO}")

target_link_libraries(
    Client
    PUBLIC
    Logger)

target_include_directories(
    Client
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    PUBLIC
    ${Boost_INCLUDE_DIRS})             
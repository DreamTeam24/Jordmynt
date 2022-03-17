add_library(
    Network
    STATIC
    "client.h"
    "client.cpp"
    "error.h"
    "error.cpp"
    "record.h"
    "record.cpp")

set_target_properties(
    Network PROPERTIES
    OUTPUT_NAME "lib${PROJECT_ABBR}network${INFO}")

target_link_libraries(
    Network
    PUBLIC
    Logger
    Crypto)

target_include_directories(
    Network
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    PUBLIC
    ${Boost_INCLUDE_DIRS})             
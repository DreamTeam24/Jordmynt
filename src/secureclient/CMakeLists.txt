add_library(
    SecureClient
    STATIC
    secureclient.h
    secureclient.cpp)

set_target_properties(
    SecureClient PROPERTIES
    OUTPUT_NAME "lib${PROJECT_ABBR}secureclient${INFO}")

target_link_libraries(
    SecureClient
    PUBLIC
    Logger
    Client
    Crypto)

target_include_directories(
    SecureClient
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src")
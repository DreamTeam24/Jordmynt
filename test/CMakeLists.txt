add_executable(
    Test
    "main.cpp"
    "common/test_conversions.cpp"
    "crypto/test_cipher.cpp"
    "crypto/test_ephemeralkey.cpp"
    "crypto/test_hashfunction.cpp"
	"crypto/test_identitykey.cpp"
	"crypto/test_keyderivationfunction.cpp")

set_target_properties(
    Test PROPERTIES
    OUTPUT_NAME "${PROJECT_ABBR}test${INFO}")

target_link_directories(
    Test
    PRIVATE
    ${Boost_LIBRARY_DIRS})

target_link_libraries(
    Test
    PRIVATE
    Common
    Crypto
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

target_include_directories(
    Test
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    ${Boost_INCLUDE_DIRS})

add_custom_command(
    TARGET Test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/data/libcrypto-3-x64.dll"
        $<TARGET_FILE_DIR:Test>)

add_test(NAME MyTest COMMAND Test)
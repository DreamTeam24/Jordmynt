add_executable(
    Test
    main.cpp 
    crypto/test_conversions.cpp
    crypto/test_decrypter.cpp
    crypto/test_encrypter.cpp
    crypto/test_ephemeralkeypair.cpp
    crypto/test_functions.cpp
	crypto/test_identitykeypair.cpp 
	crypto/test_keyderivationfunction.cpp)

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
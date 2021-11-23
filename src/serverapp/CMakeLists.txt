add_executable(
	ServerApp
	main.cpp)

set_target_properties(
	ServerApp PROPERTIES
	OUTPUT_NAME "${PROJECT_ABBR}server${INFO}")

target_link_libraries(
	ServerApp
	PRIVATE
	Server)

target_include_directories(
	ServerApp
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
	${Boost_INCLUDE_DIRS})

add_custom_command(
    TARGET ServerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/data/libcrypto-3-x64.dll"
        $<TARGET_FILE_DIR:ServerApp>)
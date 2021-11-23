add_executable(
	ClientApp
	WIN32 MACOSX_BUNDLE
#	main.cpp)
	"clientapp.h"
	"clientapp.cpp"
#	id.h
	logindialog.h
	logindialog.cpp
	main.h
	main.cpp
	"gui.h"
	"gui.cpp"
	errorbox.h
	errorbox.cpp "secureclient.cpp")

set_target_properties(
	ClientApp PROPERTIES
	OUTPUT_NAME "${PROJECT_ABBR}client${INFO}")

target_link_libraries(
	ClientApp
	PRIVATE
	Client
    Logger
    ${wxWidgets_LIBRARIES})

target_include_directories(
	ClientApp
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
    ${Boost_INCLUDE_DIRS}
    ${wxWidgets_INCLUDE_DIRS})

add_custom_command(
    TARGET ClientApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/data/libcrypto-3-x64.dll"
        $<TARGET_FILE_DIR:ClientApp>)
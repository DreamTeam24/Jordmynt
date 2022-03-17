#add_executable(
#	Common
#	bytes.h
#	bytes.cpp
#	conversions.h
#	conversions.cpp
#	error.h
#	error.cpp
#	main.cpp)

add_library(
	Common
	STATIC
	"byte.h"
	"byte.cpp"
	"conversions.h"
	"conversions.cpp"
	"error.h"
	"error.cpp")

set_target_properties(
	Common PROPERTIES 
	OUTPUT_NAME "lib${PROJECT_ABBR}common${INFO}")

target_link_libraries(
	Common
	PUBLIC
	Logger)

target_include_directories(
	Common
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
    PUBLIC
    ${Boost_INCLUDE_DIRS})
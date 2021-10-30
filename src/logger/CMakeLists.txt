add_library(
	Logger
	STATIC 
	logger.h
	logger.cpp)

set_target_properties(
	Logger PROPERTIES
	OUTPUT_NAME "lib${PROJECT_ABBR}logger${INFO}")

target_link_directories(
	Logger 
	PUBLIC 
	${Boost_LIBRARY_DIRS})

target_link_libraries(
	Logger
	PUBLIC 
	${Boost_DATE_TIME})

target_include_directories(
	Logger
	PUBLIC 
	${Boost_INCLUDE_DIRS})
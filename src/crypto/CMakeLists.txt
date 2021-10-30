add_library(
	Crypto
	STATIC
	bytes.h
	cipher.h
	cipher.cpp
	conversions.h
	conversions.cpp
	functions.h
	functions.cpp
	keyderivationfunction.h
	keyderivationfunction.cpp
	keypair.h
	keypair.cpp
	secureclientapp.h
	secureclientapp.cpp)

set_target_properties(
	Crypto PROPERTIES 
	OUTPUT_NAME "lib${PROJECT_ABBR}crypto${INFO}")

target_link_directories(
	Crypto
	PUBLIC
	${OPENSSL_LIBRARY_DIR})

target_link_libraries(
	Crypto
	PUBLIC
	Logger
    ${OPENSSL_CRYPTO_LIBRARY})

target_include_directories(
	Crypto
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
    PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS})
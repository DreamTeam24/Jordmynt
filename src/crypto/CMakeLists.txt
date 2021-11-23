add_library(
	Crypto
	STATIC
	conversions.h
	conversions.cpp
	decrypter.h
	decrypter.cpp
	encrypter.h
	encrypter.cpp
	ephemeralkeypair.h
	ephemeralkeypair.cpp
	functions.h
	functions.cpp
	identitykeypair.h
	identitykeypair.cpp
	keyderivationfunction.h
	keyderivationfunction.cpp
	securer.h
	securer.cpp)

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
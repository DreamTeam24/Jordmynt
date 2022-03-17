add_library(
	Crypto
	STATIC
	"cipher.h"
	"cipher.cpp"
	"ephemeralkey.h"
	"ephemeralkey.cpp"
	"generaterandombytes.h"
	"generaterandombytes.cpp"
	"hashfunction.h"
	"hashfunction.cpp"
	"identitykey.h"
	"identitykey.cpp"
	"keyderivationfunction.h"
	"keyderivationfunction.cpp"
	"opensslerror.h"
	"opensslerror.cpp"
	"pointer.h"
	"pointer.cpp")
	#protocol.h
	#protocol.cpp 
	#"record.h"
	#"record.cpp")

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
    ${OPENSSL_INCLUDE_DIR})
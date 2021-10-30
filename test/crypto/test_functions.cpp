#include "crypto/functions.h"
#include "crypto/bytes.h"
#include "crypto/conversions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Crypto)

BOOST_AUTO_TEST_CASE(functions_hash)
{
    // Input
    std::string dataHex{ "" };
    jm::crypto::Bytes data{ jm::crypto::convertHexStringToBytes(dataHex) };

    // Compute output
    jm::crypto::Bytes hash{ jm::crypto::hash(data) };
    std::string hashHex{ jm::crypto::convertBytesToHexString(hash) };

    // Expected output
    std::string correctHashHex{ "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" };

    // Test
    BOOST_TEST(hashHex == correctHashHex);
}

BOOST_AUTO_TEST_SUITE_END()
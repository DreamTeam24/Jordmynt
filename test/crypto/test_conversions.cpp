#include "crypto/conversions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Crypto)

BOOST_AUTO_TEST_CASE(conversions_convertHexStringToBytes)
{
    // Input
    std::string hexString{ "00010203fdfeff" }; 

    // Compute output
    jm::crypto::Bytes bytes{ jm::crypto::convertHexStringToBytes(hexString) };

    // Expected output
    jm::crypto::Bytes correctBytes{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0xfe, 0xff };

    // Test
    BOOST_TEST(bytes == correctBytes);
}

BOOST_AUTO_TEST_CASE(conversions_convertBytesToHexString)
{
    // Input
    jm::crypto::Bytes bytes{ 0x00, 0x01, 0x02, 0x03, 0xfd, 0xfe, 0xff };

    // Compute output
    std::string hexString{ jm::crypto::convertBytesToHexString(bytes) };

    // Expected output
    std::string correctHexString{ "00010203fdfeff" };

    // Test
    BOOST_TEST(hexString == correctHexString);
}

BOOST_AUTO_TEST_CASE(conversions_convertBase64StringToBytes)
{
    //---------------------------------------------------//
    //   Based on https://en.wikipedia.org/wiki/Base64   //
    //---------------------------------------------------//

    // Input
    std::string base64String{ "TWE=" };

    // Compute output
    jm::crypto::Bytes bytes{ jm::crypto::convertBase64StringToBytes(base64String) };

    // Expected output
    jm::crypto::Bytes correctBytes{ 0x4d, 0x61 };

    // Test
    BOOST_TEST(bytes == correctBytes);
}

BOOST_AUTO_TEST_CASE(conversions_convertBytesToBase64String)
{
    //---------------------------------------------------//
    //   Based on https://en.wikipedia.org/wiki/Base64   //
    //---------------------------------------------------//

    // Input
    jm::crypto::Bytes bytes{ 0x4d, 0x61 };

    // Compute output
    std::string base64String{ jm::crypto::convertBytesToBase64String(bytes) };

    // Expected output
    std::string correctBase64String{ "TWE=" };

    // Test
    BOOST_TEST(base64String == correctBase64String);
}

BOOST_AUTO_TEST_CASE(conversions_convertTextStringToBytes)
{
    //---------------------------------------------------//
    //   Based on https://en.wikipedia.org/wiki/Base64   //
    //---------------------------------------------------//

    // Input
    std::string textString{ "Hello" };

    // Compute output
    jm::crypto::Bytes bytes{ jm::crypto::convertTextStringToBytes(textString) };

    // Expected output
    jm::crypto::Bytes correctBytes{ 0x48, 0x65, 0x6C, 0x6C, 0x6F };

    // Test
    BOOST_TEST(bytes == correctBytes);
}

BOOST_AUTO_TEST_CASE(conversions_convertBytesToTextString)
{
    //---------------------------------------------------//
    //   Based on https://en.wikipedia.org/wiki/Base64   //
    //---------------------------------------------------//

    // Input
    jm::crypto::Bytes bytes{ 0x48, 0x65, 0x6C, 0x6C, 0x6F };

    // Compute output
    std::string textString{ jm::crypto::convertBytesToTextString(bytes) };

    // Expected output
    std::string correctTextString{ "Hello" };

    // Test
    BOOST_TEST(textString == correctTextString);
}


BOOST_AUTO_TEST_SUITE_END()
#include "crypto/identitykeypair.h"
#include "crypto/conversions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Crypto)

BOOST_AUTO_TEST_CASE(IdentityKeyPair_keySize)
{
    // No input
    
    // Compute output
    jm::crypto::IdentityKeyPair identityKeyPair{};

    // Expected output
    jm::crypto::Bytes::size_type correctPublicIdentityKeySize{ 32 };

    // Test
    BOOST_TEST(identityKeyPair.getPublicIdentityKeySize() == correctPublicIdentityKeySize);
}

BOOST_AUTO_TEST_CASE(IdentityKeyPair_getPublicIdentityKey)
{
    //------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/rfc8032   //
    //------------------------------------------------------------//

    // Input
    std::string privateIdentityKey1Hex{ "9d61b19deffd5a60ba844af492ec2cc4"
                                        "4449c5697b326919703bac031cae7f60" }; 
    jm::crypto::Bytes privateIdentityKey1{ jm::crypto::convertHexStringToBytes(privateIdentityKey1Hex) };
    std::string privateIdentityKey2Hex{ "4ccd089b28ff96da9db6c346ec114e0f"
                                        "5b8a319f35aba624da8cf6ed4fb8a6fb" };
    jm::crypto::Bytes privateIdentityKey2{ jm::crypto::convertHexStringToBytes(privateIdentityKey2Hex) };
    
    // Compute output
    jm::crypto::IdentityKeyPair identityKeyPair1{ privateIdentityKey1 };
    jm::crypto::Bytes publicIdentityKey1{ identityKeyPair1.getPublicIdentityKey() };
    std::string publicIdentityKey1Hex{ jm::crypto::convertBytesToHexString(publicIdentityKey1) };
    jm::crypto::IdentityKeyPair identityKeyPair2{ privateIdentityKey2 };
    jm::crypto::Bytes publicIdentityKey2{ identityKeyPair2.getPublicIdentityKey() };
    std::string publicIdentityKey2Hex{ jm::crypto::convertBytesToHexString(publicIdentityKey2) };

    // Expected output
    std::string correctPublicIdentityKey1Hex{ "d75a980182b10ab7d54bfed3c964073a"
                                              "0ee172f3daa62325af021a68f707511a" };
    std::string correctPublicIdentityKey2Hex{ "3d4017c3e843895a92b70aa74d1b7ebc"
                                              "9c982ccf2ec4968cc0cd55f12af4660c" };

    // Test
    BOOST_TEST(publicIdentityKey1Hex == correctPublicIdentityKey1Hex);
    BOOST_TEST(publicIdentityKey2Hex == correctPublicIdentityKey2Hex);
}

BOOST_AUTO_TEST_CASE(IdentityKeyPair_sign)
{
    //------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/rfc8032   //
    //------------------------------------------------------------//

    // Input
    // Input
    std::string privateIdentityKey1Hex{ "9d61b19deffd5a60ba844af492ec2cc4"
                                        "4449c5697b326919703bac031cae7f60" };
    jm::crypto::Bytes privateIdentityKey1{ jm::crypto::convertHexStringToBytes(privateIdentityKey1Hex) };
    jm::crypto::Bytes message1{};
    std::string privateIdentityKey2Hex{ "4ccd089b28ff96da9db6c346ec114e0f"
                                        "5b8a319f35aba624da8cf6ed4fb8a6fb" };
    jm::crypto::Bytes privateIdentityKey2{ jm::crypto::convertHexStringToBytes(privateIdentityKey2Hex) };
    jm::crypto::Bytes message2{ 0x72 };
    
    // Compute output
    jm::crypto::IdentityKeyPair identityKeyPair1{ privateIdentityKey1 };
    jm::crypto::Bytes signature1{ identityKeyPair1.sign(message1) };
    std::string signature1Hex{ jm::crypto::convertBytesToHexString(signature1) };
    jm::crypto::IdentityKeyPair identityKeyPair2{ privateIdentityKey2 };
    jm::crypto::Bytes signature2{ identityKeyPair2.sign(message2) };
    std::string signature2Hex{ jm::crypto::convertBytesToHexString(signature2) };

    // Expected output
    std::string correctSignature1Hex{ "e5564300c360ac729086e2cc806e828a"
                                      "84877f1eb8e5d974d873e06522490155"
                                      "5fb8821590a33bacc61e39701cf9b46b"
                                      "d25bf5f0595bbe24655141438e7a100b" };
    std::string correctSignature2Hex{ "92a009a9f0d4cab8720e820b5f642540"
                                      "a2b27b5416503f8fb3762223ebdb69da"
                                      "085ac1e43e15996e458f3613d0f11d8c"
                                      "387b2eaeb4302aeeb00d291612bb0c00" };

    // Test
    BOOST_TEST(signature1Hex == correctSignature1Hex);
    BOOST_TEST(signature2Hex == correctSignature2Hex);
}

BOOST_AUTO_TEST_SUITE_END()
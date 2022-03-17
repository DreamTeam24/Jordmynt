#include "crypto/identitykey.h"
#include "common/byte.h"
#include "common/conversions.h"
#include <boost/test/unit_test.hpp>
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(Crypto)
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IdentityKey_getPublicKey)
{
    //------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/rfc8032   //
    //------------------------------------------------------------//

    jm::Bytes memory(64);
    jm::BytesSpan buffer{ memory };

    // Input
    std::string privateKeyHex{ "c5aa8df43f9f837bedb7442f31dcb7b1"
                               "66d38535076f094b85ce3a2e0b4458f7" }; 
    jm::Bytes privateKey{ jm::convertHexStringToBytes(privateKeyHex) };
    
    // Compute output
    jm::crypto::IdentityKey key{ privateKey };
    jm::BytesSpan publicKey{ key.getPublicKey(buffer) };
    std::string publicKeyHex{ jm::convertBytesToHexString(publicKey) };

    // Expected output
    std::string correctPublicKeyHex{ "fc51cd8e6218a1a38da47ed00230f058"
                                     "0816ed13ba3303ac5deb911548908025" };

    // Test
    BOOST_TEST(publicKeyHex == correctPublicKeyHex);
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IdentityKey_sign)
{
    //------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/rfc8032   //
    //------------------------------------------------------------//

    jm::Bytes memory(128);
    jm::BytesSpan buffer{ memory };
    
    // Input
    std::string privateKey1Hex{ "9d61b19deffd5a60ba844af492ec2cc4"
                                "4449c5697b326919703bac031cae7f60" };
    jm::Bytes privateKey1{ jm::convertHexStringToBytes(privateKey1Hex) };
    jm::Bytes message1{ };
    std::string privateKey2Hex{ "c5aa8df43f9f837bedb7442f31dcb7b1"
                                "66d38535076f094b85ce3a2e0b4458f7" };
    jm::Bytes privateKey2{ jm::convertHexStringToBytes(privateKey2Hex) };
    jm::Bytes message2{ 0xaf, 0x82 };
    
    // Compute output
    jm::crypto::IdentityKey key1{ privateKey1 };
    jm::BytesSpan signature1{ key1.sign(message1, buffer) };
    std::string signature1Hex{ jm::convertBytesToHexString(signature1) };
    jm::crypto::IdentityKey key2{ privateKey2 };
    jm::BytesSpan signature2{ key2.sign(message2, buffer) };
    std::string signature2Hex{ jm::convertBytesToHexString(signature2) };

    // Expected output
    std::string correctSignature1Hex{ "e5564300c360ac729086e2cc806e828a"
                                      "84877f1eb8e5d974d873e06522490155"
                                      "5fb8821590a33bacc61e39701cf9b46b"
                                      "d25bf5f0595bbe24655141438e7a100b" };
    std::string correctSignature2Hex{ "6291d657deec24024827e69c3abe01a3"
                                      "0ce548a284743a445e3680d7db5ac3ac"
                                      "18ff9b538d16f290ae67f760984dc659"
                                      "4a7c15e9716ed28dc027beceea1ec40a" };

    // Test
    BOOST_TEST(signature1Hex == correctSignature1Hex);
    BOOST_TEST(signature1.size() == key1.getSignatureSize());
    BOOST_TEST(signature2Hex == correctSignature2Hex);
    BOOST_TEST(signature2.size() == key2.getSignatureSize());
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(IdentityKey_verify)
{
    //------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/rfc8032   //
    //------------------------------------------------------------//

    // Input
    std::string peerPublicKeyHex{ "fc51cd8e6218a1a38da47ed00230f058"
                                  "0816ed13ba3303ac5deb911548908025" };
    jm::Bytes peerPublicKey{ jm::convertHexStringToBytes(peerPublicKeyHex) };
    jm::Bytes message{ 0xaf, 0x82 };
    std::string signatureHex{ "6291d657deec24024827e69c3abe01a3"
                              "0ce548a284743a445e3680d7db5ac3ac"
                              "18ff9b538d16f290ae67f760984dc659"
                              "4a7c15e9716ed28dc027beceea1ec40a" };
    jm::Bytes signature{ jm::convertHexStringToBytes(signatureHex) };

    // Compute output
    std::string privateKeyHex{ "00000000000000000000000000000000"
                               "00000000000000000000000000000000" };
    jm::Bytes privateKey{ jm::convertHexStringToBytes(privateKeyHex) };
    jm::crypto::IdentityKey key{ privateKey };
    bool check{ key.verify(message, signature, peerPublicKey) };

    // Test
    BOOST_TEST(check == true);
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
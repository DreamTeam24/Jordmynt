#include "crypto/ephemeralkeypair.h"
#include "crypto/conversions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Crypto)

BOOST_AUTO_TEST_CASE(EphemeralKeyPair_keySize)
{
    // No input
    
    // Compute output
    jm::crypto::EphemeralKeyPair ephemeralKeyPair{};

    // Expected output
    jm::crypto::Bytes::size_type correctPublicEphemeralKeySize{ 32 };

    // Test
    BOOST_TEST(ephemeralKeyPair.getPublicEphemeralKeySize() == correctPublicEphemeralKeySize);
}

BOOST_AUTO_TEST_CASE(EphemeralKeyPair_getPublicEphemeralKey)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    // Input
    std::string privateEphemeralKeyHex{ "202122232425262728292a2b2c2d2e2f"
                                        "303132333435363738393a3b3c3d3e3f" };
    jm::crypto::Bytes privateEphemeralKey{ jm::crypto::convertHexStringToBytes(privateEphemeralKeyHex) };
    
    // Compute output
    jm::crypto::EphemeralKeyPair ephemeralKeyPair{ privateEphemeralKey };
    jm::crypto::Bytes publicEphemeralKey{ ephemeralKeyPair.getPublicEphemeralKey() };
    std::string publicEphemeralKeyHex{ jm::crypto::convertBytesToHexString(publicEphemeralKey) };

    // Expected output
    std::string correctPublicEphemeralKeyHex{ "358072d6365880d1aeea329adf912138"
                                              "3851ed21a28e3b75e965d0d2cd166254" };

    // Test
    BOOST_TEST(publicEphemeralKeyHex == correctPublicEphemeralKeyHex);
}

BOOST_AUTO_TEST_CASE(EphemeralKeyPair_deriveSharedSecret)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    // Input
    std::string privateEphemeralKeyHex{ "202122232425262728292a2b2c2d2e2f"
                                        "303132333435363738393a3b3c3d3e3f" };
    jm::crypto::Bytes privateEphemeralKey{ jm::crypto::convertHexStringToBytes(privateEphemeralKeyHex) };
    std::string peerPublicEphemeralKeyHex{ "9fd7ad6dcff4298dd3f96d5b1b2af910"
                                           "a0535b1488d7f8fabb349a982880b615" };
    jm::crypto::Bytes peerPublicEphemeralKey{ jm::crypto::convertHexStringToBytes(peerPublicEphemeralKeyHex) };
    
    // Compute output
    jm::crypto::EphemeralKeyPair ephemeralKeyPair{ privateEphemeralKey };
    jm::crypto::Bytes sharedSecret{ ephemeralKeyPair.deriveSharedSecret(peerPublicEphemeralKey) };
    std::string sharedSecretHex{ jm::crypto::convertBytesToHexString(sharedSecret) };
    
    // Expected output
    std::string correctSharedSecretHex{ "df4a291baa1eb7cfa6934b29b474baad"
                                        "2697e29f1f920dcc77c8a0a088447624" };

    // Test
    BOOST_TEST(sharedSecretHex == correctSharedSecretHex);
}

BOOST_AUTO_TEST_SUITE_END()
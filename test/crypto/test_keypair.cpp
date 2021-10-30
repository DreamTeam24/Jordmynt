#include "crypto/keypair.h"
#include "crypto/bytes.h"
#include "crypto/conversions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Crypto)

BOOST_AUTO_TEST_CASE(KeyPair_keySize)
{
    // No input
    
    // Compute output
    jm::crypto::KeyPair keyPair{ };

    // Expected output
    jm::crypto::Bytes::size_type correctPublicKeySize{ 32 };

    // Test
    BOOST_TEST(keyPair.getPublicKeySize() == correctPublicKeySize);
}

BOOST_AUTO_TEST_CASE(KeyPair_getPublicKey)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    // Input
    std::string privateKeyHex{ "202122232425262728292a2b2c2d2e2f"
                               "303132333435363738393a3b3c3d3e3f" };
    jm::crypto::Bytes privateKey{ jm::crypto::convertHexStringToBytes(privateKeyHex) };
    
    // Compute output
    jm::crypto::KeyPair keyPair{ privateKey };
    jm::crypto::Bytes publicKey{ keyPair.getPublicKey() };
    std::string publicKeyHex{ jm::crypto::convertBytesToHexString(publicKey) };

    // Expected output
    std::string correctPublicKeyHex{ "358072d6365880d1aeea329adf912138"
                                     "3851ed21a28e3b75e965d0d2cd166254" };

    // Test
    BOOST_TEST(publicKeyHex == correctPublicKeyHex);
}

BOOST_AUTO_TEST_CASE(KeyPair_deriveSharedSecret)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    // Input
    std::string privateKeyHex{ "202122232425262728292a2b2c2d2e2f"
                               "303132333435363738393a3b3c3d3e3f" };
    jm::crypto::Bytes privateKey{ jm::crypto::convertHexStringToBytes(privateKeyHex) };
    std::string peerPublicKeyHex{ "9fd7ad6dcff4298dd3f96d5b1b2af910"
                                  "a0535b1488d7f8fabb349a982880b615" };
    jm::crypto::Bytes peerPublicKey{ jm::crypto::convertHexStringToBytes(peerPublicKeyHex) };
    
    // Compute output
    jm::crypto::KeyPair keyPair{ privateKey };
    jm::crypto::Bytes sharedSecret{ keyPair.deriveSharedSecret(peerPublicKey) };
    std::string sharedSecretHex{ jm::crypto::convertBytesToHexString(sharedSecret) };
    
    // Expected output
    std::string correctSharedSecretHex{ "df4a291baa1eb7cfa6934b29b474baad"
                                        "2697e29f1f920dcc77c8a0a088447624" };

    // Test
    BOOST_TEST(sharedSecretHex == correctSharedSecretHex);
}

BOOST_AUTO_TEST_SUITE_END()
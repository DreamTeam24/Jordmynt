#include "crypto/ephemeralkey.h"
#include "common/byte.h"
#include "common/conversions.h"
#include <boost/test/unit_test.hpp>
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(Crypto)
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EphemeralKey_getPublicKey)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    jm::Bytes memory(32);
    jm::BytesSpan buffer{ memory };

    // Input
    std::string privateKeyHex{ "202122232425262728292a2b2c2d2e2f"
                               "303132333435363738393a3b3c3d3e3f" };
    jm::Bytes privateKey{ jm::convertHexStringToBytes(privateKeyHex) };

    // Compute output
    jm::crypto::EphemeralKey key{ privateKey };
    jm::BytesSpan publicKey{ key.getPublicKey(buffer) };
    std::string publicKeyHex{ jm::convertBytesToHexString(publicKey) };

    // Expected output
    std::string correctPublicKeyHex{ "358072d6365880d1aeea329adf912138"
                                     "3851ed21a28e3b75e965d0d2cd166254" };

    // Test
    BOOST_TEST(publicKeyHex == correctPublicKeyHex);
    BOOST_TEST(publicKey.size() == key.getPublicKeySize());
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EphemeralKey_deriveSharedSecret)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    jm::Bytes memory(32);
    jm::BytesSpan buffer{ memory };
    
    // Input
    std::string privateKeyHex{ "202122232425262728292a2b2c2d2e2f"
                               "303132333435363738393a3b3c3d3e3f" };
    jm::Bytes privateKey{ jm::convertHexStringToBytes(privateKeyHex) };
    std::string peerPublicKeyHex{ "9fd7ad6dcff4298dd3f96d5b1b2af910"
                                  "a0535b1488d7f8fabb349a982880b615" };
    jm::Bytes peerPublicKey{ jm::convertHexStringToBytes(peerPublicKeyHex) };
    
    // Compute output
    jm::crypto::EphemeralKey key{ privateKey };
    jm::BytesSpan sharedSecret{ key.deriveSharedSecret(peerPublicKey, buffer) };
    std::string sharedSecretHex{ jm::convertBytesToHexString(sharedSecret) };
    
    // Expected output
    std::string correctSharedSecretHex{ "df4a291baa1eb7cfa6934b29b474baad"
                                        "2697e29f1f920dcc77c8a0a088447624" };

    // Test
    BOOST_TEST(sharedSecretHex == correctSharedSecretHex);
    BOOST_TEST(sharedSecret.size() == key.getSharedSecretSize());
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
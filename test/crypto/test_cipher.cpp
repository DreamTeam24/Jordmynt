#include "crypto/cipher.h"
#include "common/byte.h"
#include "common/conversions.h"
#include <boost/test/unit_test.hpp>
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(Crypto)
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Cipher_encryptAndAuthenticate)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    jm::Bytes memory(402);
    jm::BytesSpan buffer{ memory };

    // Input
    std::string keyHex{ "0b6d22c8ff68097ea871c672073773bf" };
    jm::Bytes key{ jm::convertHexStringToBytes(keyHex) };
    std::string initializationVectorHex{ "1b13dd9f8d8f17091d34b349" };
    jm::Bytes initializationVector{ jm::convertHexStringToBytes(initializationVectorHex) };
    std::string plaintext1Hex{ "040000b20002a30004030201010000a0"
                               "01060911161921262931363941464951"
                               "03060913161923262933363943464953"
                               "f70029ecf2c4a441fc30172e9f7ca8af"
                               "7570f01fc798f7cf5a5a6b5bfef1e73a"
                               "e8f76cd2a8a6925b968ddedbd3206acb"
                               "6906f491852ee65e0c59f29e9b799124"
                               "7e4a323dbe4b8070afd01de2ca053509"
                               "09050fbbc4aed7c4edd7ae35c8736378"
                               "64c97a1fed7a9a4744fd50f7b7e064a9"
                               "02c15c23183fc4cf7202592de1aa6172"
                               "00045a5a0000040000b20002a3000403"
                               "0201010100a001060911161921262931"
                               "36394146495103060913161923262933"
                               "363943464953f70029ecf2c4a441fc30"
                               "172e9f7ca8af4f69197b804884c2df76"
                               "0cf4be7b8b6dfb7173e99052ef4b5018"
                               "2fc07443ed10a9f5070567053a2ae8f2"
                               "18179c11f1f13ec9d1857f8e01b499ff"
                               "2482c62af74e1c86a9fccad984c9abec"
                               "40de8003a8164ffca68f925f25f3be18"
                               "4166172bfbef664b0a5d6f94ccedc7c2"
                               "2f6429a3185f00045a5a000016" };
    jm::Bytes plaintext1{ jm::convertHexStringToBytes(plaintext1Hex) };
    std::string associatedData1Hex{ "170303017d" };
    jm::Bytes associatedData1{ jm::convertHexStringToBytes(associatedData1Hex) };
    std::string plaintext2Hex{ "706f6e6717" };
    jm::Bytes plaintext2{ jm::convertHexStringToBytes(plaintext2Hex) };
    std::string associatedData2Hex{ "1703030015" };
    jm::Bytes associatedData2{ jm::convertHexStringToBytes(associatedData2Hex) };

    // Compute output
    jm::crypto::Cipher cipher{ key, initializationVector };
    jm::BytesSpan authenticatedCiphertext1{ cipher.encryptAndAuthenticate(plaintext1, associatedData1, buffer) };
    std::string authenticatedCiphertext1Hex{ jm::convertBytesToHexString(authenticatedCiphertext1) };
    jm::BytesSpan authenticatedCiphertext2{ cipher.encryptAndAuthenticate(plaintext2, associatedData2, buffer) };
    std::string authenticatedCiphertext2Hex{ jm::convertBytesToHexString(authenticatedCiphertext2) };

    // Expected output
    std::string correctAuthenticatedCiphertext1Hex{ "c4d4b71b6f4c2f30130274e7b46e4089"
                                                    "68de0798f3607c4360bd46de37a7db43"
                                                    "46a135b6e5db37c42fc8e09dda3dd1f1"
                                                    "a7df4db2c410afdda8dd817b8d895182"
                                                    "48d2d95e5e19864cc55e7febe70f15da"
                                                    "f4a64d0b30750879ccb2c5d288be3574"
                                                    "5e5dc601d3e574f217f1b2a6381ad7b6"
                                                    "e1b6c718b265c6f282a192accb225b33"
                                                    "a7733d726e92ea2b4f8b0020e7c74b73"
                                                    "96307f6f5d5a2c1c6169c0f088b6bc02"
                                                    "56fad5bb27ee82f789b165de265b4d4d"
                                                    "b84086ca62652b1b0c22b4e58f98bbb1"
                                                    "ec0d7c91bd200ca08bf2f79f86f4e560"
                                                    "558e5714c525238392e2232b2ce22417"
                                                    "fc0d4d42935f549b7b27a86df03e5304"
                                                    "4a64dd74bcedc6e42912e8ea1bc93520"
                                                    "d4d46d6e1bf1f4393594184836441a3a"
                                                    "f97c4d0484b5c7198f681955eaae5519"
                                                    "26ff4ec839fdd2b84810eeab83e075c9"
                                                    "496b454315feda35b01e46eef41ef866"
                                                    "4946223f1bffb3b9ad58b57fd8ce3f7e"
                                                    "e11679222ed36d9e070318965f821f43"
                                                    "357aae5b95449a005f88e0a2dabfe0c1"
                                                    "f688627643a049c3492eaf6f0f" };
    std::string correctAuthenticatedCiphertext2Hex{ "370e5f168afa7fb16b663ecdfca3dbb8"
                                                    "1931a90ca7" };

    // Test
    BOOST_TEST(authenticatedCiphertext1Hex == correctAuthenticatedCiphertext1Hex);
    BOOST_TEST(authenticatedCiphertext2Hex == correctAuthenticatedCiphertext2Hex);
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(cipher_decrypt)
{
    //-----------------------------------------//
    //   Based on https://tls13.ulfheim.net/   //
    //-----------------------------------------//

    jm::Bytes memory(370);
    jm::BytesSpan buffer{ memory };

    // Input
    std::string keyHex{ "0b6d22c8ff68097ea871c672073773bf" };
    jm::Bytes key{ jm::convertHexStringToBytes(keyHex) };
    std::string initializationVectorHex{ "1b13dd9f8d8f17091d34b349" };
    jm::Bytes initializationVector{ jm::convertHexStringToBytes(initializationVectorHex) };
    std::string authenticatedCiphertext1Hex{ "c4d4b71b6f4c2f30130274e7b46e4089"
                                             "68de0798f3607c4360bd46de37a7db43"
                                             "46a135b6e5db37c42fc8e09dda3dd1f1"
                                             "a7df4db2c410afdda8dd817b8d895182"
                                             "48d2d95e5e19864cc55e7febe70f15da"
                                             "f4a64d0b30750879ccb2c5d288be3574"
                                             "5e5dc601d3e574f217f1b2a6381ad7b6"
                                             "e1b6c718b265c6f282a192accb225b33"
                                             "a7733d726e92ea2b4f8b0020e7c74b73"
                                             "96307f6f5d5a2c1c6169c0f088b6bc02"
                                             "56fad5bb27ee82f789b165de265b4d4d"
                                             "b84086ca62652b1b0c22b4e58f98bbb1"
                                             "ec0d7c91bd200ca08bf2f79f86f4e560"
                                             "558e5714c525238392e2232b2ce22417"
                                             "fc0d4d42935f549b7b27a86df03e5304"
                                             "4a64dd74bcedc6e42912e8ea1bc93520"
                                             "d4d46d6e1bf1f4393594184836441a3a"
                                             "f97c4d0484b5c7198f681955eaae5519"
                                             "26ff4ec839fdd2b84810eeab83e075c9"
                                             "496b454315feda35b01e46eef41ef866"
                                             "4946223f1bffb3b9ad58b57fd8ce3f7e"
                                             "e11679222ed36d9e070318965f821f43"
                                             "357aae5b95449a005f88e0a2dabfe0c1"
                                             "f688627643a049c3492eaf6f0f" };
    jm::Bytes authenticatedCiphertext1{ jm::convertHexStringToBytes(authenticatedCiphertext1Hex) };
    std::string associatedData1Hex{ "170303017d" };
    jm::Bytes associatedData1{ jm::convertHexStringToBytes(associatedData1Hex) };
    std::string authenticatedCiphertext2Hex{ "370e5f168afa7fb16b663ecdfca3dbb8"
                                             "1931a90ca7" };
    jm::Bytes authenticatedCiphertext2{ jm::convertHexStringToBytes(authenticatedCiphertext2Hex) };
    std::string associatedData2Hex{ "1703030015" };
    jm::Bytes associatedData2{ jm::convertHexStringToBytes(associatedData2Hex) };

    // Compute output
    jm::crypto::Cipher cipher{ key, initializationVector };
    jm::BytesSpan plaintext1{ cipher.decrypt(authenticatedCiphertext1, associatedData1, buffer) };
    std::string plaintext1Hex{ jm::convertBytesToHexString(plaintext1) };
    jm::BytesSpan plaintext2{ cipher.decrypt(authenticatedCiphertext2, associatedData2, buffer) };
    std::string plaintext2Hex{ jm::convertBytesToHexString(plaintext2) };

    // Expected output
    std::string correctPlaintext1Hex{ "040000b20002a30004030201010000a0"
                                      "01060911161921262931363941464951"
                                      "03060913161923262933363943464953"
                                      "f70029ecf2c4a441fc30172e9f7ca8af"
                                      "7570f01fc798f7cf5a5a6b5bfef1e73a"
                                      "e8f76cd2a8a6925b968ddedbd3206acb"
                                      "6906f491852ee65e0c59f29e9b799124"
                                      "7e4a323dbe4b8070afd01de2ca053509"
                                      "09050fbbc4aed7c4edd7ae35c8736378"
                                      "64c97a1fed7a9a4744fd50f7b7e064a9"
                                      "02c15c23183fc4cf7202592de1aa6172"
                                      "00045a5a0000040000b20002a3000403"
                                      "0201010100a001060911161921262931"
                                      "36394146495103060913161923262933"
                                      "363943464953f70029ecf2c4a441fc30"
                                      "172e9f7ca8af4f69197b804884c2df76"
                                      "0cf4be7b8b6dfb7173e99052ef4b5018"
                                      "2fc07443ed10a9f5070567053a2ae8f2"
                                      "18179c11f1f13ec9d1857f8e01b499ff"
                                      "2482c62af74e1c86a9fccad984c9abec"
                                      "40de8003a8164ffca68f925f25f3be18"
                                      "4166172bfbef664b0a5d6f94ccedc7c2"
                                      "2f6429a3185f00045a5a000016" };
    std::string correctPlaintext2Hex{ "706f6e6717" };

    // Test
    BOOST_TEST(plaintext1Hex == correctPlaintext1Hex);
    BOOST_TEST(plaintext2Hex == correctPlaintext2Hex);
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
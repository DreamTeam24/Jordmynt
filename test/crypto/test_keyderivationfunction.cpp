#include "crypto/keyderivationfunction.h"
#include "common/byte.h"
#include "common/conversions.h"
#include <boost/test/unit_test.hpp>
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(Crypto)
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(KeyDerivationFunction_extractPseudoRandomKey)
{
    //------------------------------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-vectors-02   //
    //------------------------------------------------------------------------------------//
    
    jm::Bytes memory(32);
    jm::BytesSpan buffer{ memory };

    // Input
    std::string keyHex{ "f677c3cdac26a755455b130efa9b1a3f"
                        "3cafb153544ca46addf670df199d996e" };
    jm::Bytes key{ jm::convertHexStringToBytes(keyHex) };
    std::string saltHex{ "6f2615a108c702c5678f54fc9dbab697"
                         "16c076189c48250cebeac3576c3611ba" }; 
    jm::Bytes salt{ jm::convertHexStringToBytes(saltHex) };
    
    // Compute output
    jm::crypto::KeyDerivationFunction function{};
    jm::BytesSpan extractedKey{ function.extractKey(key, salt, buffer) };
    std::string extractedKeyHex{ jm::convertBytesToHexString(extractedKey) };
    
    // Expected output
    std::string correctExtractedKeyHex{ "0cefce00d5d29fd09f5de36c86fc8e72"
                                        "99b4ad11ba4211c67063c2cc539fc4f9" };
    
    // Test
    BOOST_TEST(extractedKeyHex == correctExtractedKeyHex);
    BOOST_TEST(extractedKey.size() == function.getExtractedKeySize());
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(KeyDerivationFunction_expandPseudoRandomKey)
{
    //------------------------------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-vectors-02   //
    //------------------------------------------------------------------------------------//

    jm::Bytes memory(32);
    jm::BytesSpan buffer{ memory };
    
    // Input
    std::string keyHex{ "0cefce00d5d29fd09f5de36c86fc8e72"
                        "99b4ad11ba4211c67063c2cc539fc4f9" };
    jm::Bytes key{ jm::convertHexStringToBytes(keyHex) };
    std::string infoHex{ "002012746c7331332063206873207472"
                         "6166666963208ac51822361c59632de3" 
                         "c6b259e5808ce52b8278a6493de2a976"
                         "f441abbadc8c"};
    jm::Bytes info{ jm::convertHexStringToBytes(infoHex) };
    jm::BytesSize expandedKeySize{ 32 };
    
    // Compute output
    jm::crypto::KeyDerivationFunction function{}; 
    jm::BytesSpan expandedKey{ function.expandKey(key, info, expandedKeySize, buffer) };
    std::string expandedKeyHex{ jm::convertBytesToHexString(expandedKey) };
    
    // Expected output
    std::string correctExpandedKeyHex{ "5a63db760b817b1bda96e72832333aec"
                                       "6a177deeadb5b407501ac10c17dac0a4" };
    
    // test
    BOOST_TEST(expandedKeyHex == correctExpandedKeyHex);
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
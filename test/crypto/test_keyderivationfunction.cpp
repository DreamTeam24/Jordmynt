#include "crypto/keyderivationfunction.h"
#include "crypto/conversions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Crypto)

BOOST_AUTO_TEST_CASE(KeyDerivationFunction_extractPseudoRandomKey)
{
    //------------------------------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-vectors-02   //
    //------------------------------------------------------------------------------------//
    
    // Input
    std::string inputKeyingMaterialHex{ "f677c3cdac26a755455b130efa9b1a3f"
                                        "3cafb153544ca46addf670df199d996e" };
    jm::crypto::Bytes inputKeyingMaterial{ jm::crypto::convertHexStringToBytes(inputKeyingMaterialHex) };
    std::string saltHex{ "6f2615a108c702c5678f54fc9dbab697"
                         "16c076189c48250cebeac3576c3611ba" }; 
    jm::crypto::Bytes salt{ jm::crypto::convertHexStringToBytes(saltHex) };
    
    // Compute output
    jm::crypto::KeyDerivationFunction keyDerivationFunction{};
    jm::crypto::Bytes extractedKey{ keyDerivationFunction.extractPseudoRandomKey(inputKeyingMaterial, salt) };
    std::string extractedKeyHex{ jm::crypto::convertBytesToHexString(extractedKey) };
    
    // Expected output
    std::string correctExtractedKeyHex{ "0cefce00d5d29fd09f5de36c86fc8e72"
                                        "99b4ad11ba4211c67063c2cc539fc4f9" };
    
    // Test
    BOOST_TEST(extractedKeyHex == correctExtractedKeyHex);
}

BOOST_AUTO_TEST_CASE(KeyDerivationFunction_expandPseudoRandomKey)
{
    //------------------------------------------------------------------------------------//
    //   Based on https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-vectors-02   //
    //------------------------------------------------------------------------------------//

    // Input
    std::string pseudoRandomKeyHex{ "0cefce00d5d29fd09f5de36c86fc8e72"
                                    "99b4ad11ba4211c67063c2cc539fc4f9" };
    jm::crypto::Bytes pseudoRandomKey{ jm::crypto::convertHexStringToBytes(pseudoRandomKeyHex) };
    std::string labelHex{ "002012746c7331332063206873207472"
                          "616666696320" };
    jm::crypto::Bytes label{ jm::crypto::convertHexStringToBytes(labelHex) };
    std::string infoHex{ "8ac51822361c59632de3c6b259e5808c"
                         "e52b8278a6493de2a976f441abbadc8c" };
    jm::crypto::Bytes info{ jm::crypto::convertHexStringToBytes(infoHex) };
    jm::crypto::Bytes::size_type expandedKeySize{ 32 };
    
    // Compute output
    jm::crypto::KeyDerivationFunction keyDerivationFunction{}; 
    jm::crypto::Bytes expandedKey{ keyDerivationFunction.expandPseudoRandomKey(pseudoRandomKey, label, info, expandedKeySize) };
    std::string expandedKeyHex{ jm::crypto::convertBytesToHexString(expandedKey) };
    
    // Expected output
    std::string correctExpandedKeyHex{ "5a63db760b817b1bda96e72832333aec"
                                       "6a177deeadb5b407501ac10c17dac0a4" };
    
    // test
    BOOST_TEST(expandedKeyHex == correctExpandedKeyHex);
}

BOOST_AUTO_TEST_SUITE_END()
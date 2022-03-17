#include "crypto/hashfunction.h"
#include "common/byte.h"
#include "common/conversions.h"
#include <boost/test/unit_test.hpp>
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(Crypto)
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(HashFunction_getDigest)
{
    //------------------------------------------------------//
    //   Based on https://en.wikipedia.org/wiki/SHA-2 and   //
    //   https://tls13.ulfheim.net/                         //
    //------------------------------------------------------//
    
    jm::Bytes memory(64);
    jm::BytesSpan buffer{ memory };

    // Input
    jm::Bytes emptyMessage{};
    std::string clientHelloHex{"010000c60303000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff0006130113021303010000770000001800160000136578616d706c652e756c666865696d2e6e6574000a00080006001d00170018000d00140012040308040401050308050501080606010201003300260024001d0020358072d6365880d1aeea329adf9121383851ed21a28e3b75e965d0d2cd166254002d00020101002b0003020304" };
    jm::Bytes clientHello{ jm::convertHexStringToBytes(clientHelloHex) };
    std::string serverHelloHex{"020000760303707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f20e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff130100002e00330024001d00209fd7ad6dcff4298dd3f96d5b1b2af910a0535b1488d7f8fabb349a982880b615002b00020304" };
    jm::Bytes serverHello{ jm::convertHexStringToBytes(serverHelloHex) };
    std::string serverHandshakeHex{ "0800000200000b00032e0000032a0003253082032130820209a0030201020208155a92adc2048f90300d06092a864886f70d01010b05003022310b300906035504061302555331133011060355040a130a4578616d706c65204341301e170d3138313030353031333831375a170d3139313030353031333831375a302b310b3009060355040613025553311c301a060355040313136578616d706c652e756c666865696d2e6e657430820122300d06092a864886f70d01010105000382010f003082010a0282010100c4803606bae7476b089404eca7b691043ff792bc19eefb7d74d7a80d001e7b4b3a4ae60fe8c071fc73e7024c0dbcf4bdd11d396bba70464a13e94af83df3e10959547bc955fb412da3765211e1f3dc776caa53376eca3aecbec3aab73b31d56cb6529c8098bcc9e02818e20bf7f8a03afd1704509ece79bd9f39f1ea69ec47972e830fb5ca95de95a1e60422d5eebe527954a1e7bf8a86f6466d0d9f16951a4cf7a04692595c1352f2549e5afb4ebfd77a37950144e4c026874c653e407d7d23074401f484ffd08f7a1fa05210d1f4f0d5ce79702932e2cabe701fdfad6b4bb71101f44bad666a11130fe2ee829e4d029dc91cdd6716dbb9061886edc1ba94210203010001a3523050300e0603551d0f0101ff0404030205a0301d0603551d250416301406082b0601050507030206082b06010505070301301f0603551d23041830168014894fde5bcc69e252cf3ea300dfb197b81de1c146300d06092a864886f70d01010b05000382010100591645a69a2e3779e4f6dd271aba1c0bfd6cd75599b5e7c36e533eff3659084324c9e7a504079d39e0d42987ffe3ebdd09c1cf1d914455870b571dd19bdf1d24f8bb9a11fe80fd592ba0398cde11e2651e618ce598fa96e5372eef3d248afde17463ebbfabb8e4d1ab502a54ec0064e92f7819660d3f27cf209e667fce5ae2e4ac99c7c93818f8b2510722dfed97f32e3e9349d4c66c9ea6396d744462a06b42c6d5ba688eac3a017bddfc8e2cfcad27cb69d3ccdca280414465d3ae348ce0f34ab2fb9c618371312b191041641c237f11a5d65c844f0404849938712b959ed685bc5c5dd645ed19909473402926dcb40e3469a15941e8e2cca84bb6084636a000000f0001040804010017feb533ca6d007d0058257968424bbc3aa6909e9d49557576a520e04a5ef05f0e86d24ff43f8eb861eef595228d7032aa360f714e667413926ef4f8b5803b69e35519e3b23f4373dfac6787066dcb4756b54560e0886e9b962c4ad28dab26bad1abc25916b09af286537f684f808aefee73046cb7df0a84fbb5967aca131f4b1cf389799403a30c02d29cbdadb72512db9cec2e5e1d00e50cafcf6f21091ebc4f253c5eab01a679baeabeedb9c9618f66006b8244d6622aaa56887ccfc66a0f3851dfa13a78cff7991e03cb2c3a0ed87d7367362eb7805b00b2524ff298a4da487cacdeaf8a2336c5631b3efa935bb411e753ca13b015fec7e4a730f1369f9e14000020ea6ee176dccc4af1859e9e4e93f797eac9a78ce439301e35275ad43f3cddbde3" };
    jm::Bytes serverHandshake{ jm::convertHexStringToBytes(serverHandshakeHex) };

    // Compute output
    jm::crypto::HashFunction hashFunction{};
    hashFunction.digest(emptyMessage);
    jm::BytesSpan digest1{ hashFunction.getDigest(buffer) };
    std::string digest1Hex{ jm::convertBytesToHexString(digest1) };
    hashFunction.digest(clientHello);
    hashFunction.digest(serverHello);
    hashFunction.digest(serverHandshake);
    jm::BytesSpan digest2{ hashFunction.getDigest(buffer) };
    std::string digest2Hex{ jm::convertBytesToHexString(digest2) };

    // Expected output
    std::string correctDigest1Hex{ "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" };
    std::string correctDigest2Hex{ "22844b930e5e0a59a09d5ac35fc032fc91163b193874a265236e568077378d8b" };

    // Test
    BOOST_TEST(digest1Hex == correctDigest1Hex);
    BOOST_TEST(digest2Hex == correctDigest2Hex);
}
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
#include "securer.h"
/*
#include "conversions.h"
#include "functions.h"
#include "keyderivationfunction.h"
#include "keypair.h"
#include "logger/logger.h"
#include <algorithm>

namespace jm {
namespace crypto {

constexpr Bytes::size_type g_randomBytesSize{ 32 };

Securer buildClientSecurer(std::string const& clientPrivateKeyFilename,
                           Bytes const&       serverPublicKey,
                           SendAndReceive     getResponseFromServer)
{
    // Client request 1 = Client random bytes + Client ephemeral public key
    Bytes clientRandomBytes{ generateRandomBytes(g_randomBytesSize) };
    KeyPair clientEphemeralKeyPair{};
    Bytes clientEphemeralPublicKey{ clientEphemeralKeyPair.getPublicKey() };
    Bytes clientRequest1(clientRandomBytes.size() + clientEphemeralPublicKey.size());
    std::copy(clientEphemeralPublicKey.begin(), clientEphemeralPublicKey.end(),
        std::copy(clientRandomBytes.begin(), clientRandomBytes.end(), clientRequest1.begin()));

    // Server response 1 = Server random bytes + Server ephemeral public key + Server public key + Server handshake signature
    Bytes serverResponse1{ getResponseFromServer(clientRequest1) };
    if (serverResponse1.size() != g_randomBytesSize + 2*KeyPair::getPublicKeySize())
        logger::THROW("Invalid server response.");
    
    Bytes serverEphemeralPublicKey{ serverResponse1.begin() + g_randomBytesSize, 
                                    serverResponse1.begin() + g_randomBytesSize + KeyPair::getPublicKeySize() };

    // Shared secret from ephemeral keys
    Bytes sharedSecret{ clientEphemeralKeyPair.deriveSharedSecret(serverEphemeralPublicKey) };

    // Hash of Client random bytes + Client ephemeral public key + Server random bytes + Server ephemeral public key
    Bytes hello{ clientRequest1 };
    hello.insert(hello.end(), serverHello.begin(), serverHello.end());
    Bytes helloHash{ hash(hello) };

    // Handshake secret
    KeyDerivationFunction keyDerivationFunction{};
    Bytes inputKeyingMaterial(KeyDerivationFunction::getPseudoRandomKeySize(), 0x00);
    Bytes salt{};
    Bytes earlySecret{ 
        keyDerivationFunction.extractPseudoRandomKey(inputKeyingMaterial, 
                                                     salt) };
    Bytes emptyHash{ hash(Bytes{}) };
    Bytes derivedSecret{
        keyDerivationFunction.expandPseudoRandomKey(earlySecret,
                                                    convertTextStringToBytes("derived"),
                                                    emptyHash,
                                                    KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes handshakeSecret{ 
        keyDerivationFunction.extractPseudoRandomKey(sharedSecret, 
                                                     derivedSecret) };

    // Handshake encrypter
    Bytes clientHandshakeTrafficSecret{
        keyDerivationFunction.expandPseudoRandomKey(handshakeSecret,
                                                    convertTextStringToBytes("c hs traffic"),
                                                    helloHash,
                                                    KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes clientHandshakeKey{
        keyDerivationFunction.expandPseudoRandomKey(clientHandshakeTrafficSecret,
                                                    convertTextStringToBytes("key"),
                                                    Bytes{},
                                                    Encrypter::getKeySize()) };
    Bytes clientHandshakeInitializationVector{
        keyDerivationFunction.expandPseudoRandomKey(clientHandshakeTrafficSecret,
                                                    convertTextStringToBytes("iv"),
                                                    Bytes{},
                                                    Encrypter::getKeySize()) };
    Encrypter handshakeEncrypter{ clientHandshakeKey, clientHandshakeInitializationVector };

    // Handshake decrypter
    Bytes serverHandshakeTrafficSecret{
        keyDerivationFunction.expandPseudoRandomKey(handshakeSecret,
                                                    convertTextStringToBytes("s hs traffic"),
                                                    helloHash,
                                                    KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes serverHandshakeKey{
        keyDerivationFunction.expandPseudoRandomKey(serverHandshakeTrafficSecret,
                                                    convertTextStringToBytes("key"),
                                                    Bytes{},
                                                    Encrypter::getKeySize()) };
    Bytes serverHandshakeInitializationVector{
        keyDerivationFunction.expandPseudoRandomKey(serverHandshakeTrafficSecret,
                                                    convertTextStringToBytes("iv"),
                                                    Bytes{},
                                                    Encrypter::getKeySize()) };
    Decrypter handshakeDecrypter{ serverHandshakeKey, serverHandshakeInitializationVector };



    // Client request 2 = Client public key + Client handshake signature + Client handshake finished
    

    // Server response 2 = Server handshake finished
}


Securer buildServerSecurer(std::string const& serverPrivateKeyFilename,
                           Bytes const& clientPublicKey,
                           std::function<Bytes(Bytes const&)> getResponseFromServer)
{

}

} // namespace crypto
} // namespace jm*/
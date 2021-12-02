#include "securer.h"

#include "conversions.h"
#include "ephemeralkeypair.h"
#include "generaterandombytes.h"
#include "hash.h"
#include "identitykeypair.h"
#include "keyderivationfunction.h"
#include "logger/logger.h"
//#include <algorithm>

namespace jm {
namespace crypto {

constexpr Bytes::size_type g_randomBytesSize{ 32 };

/*Securer buildClientSecurer(std::string const& clientPrivateIdentityKeyFilename,
                           Bytes const&       serverPublicIdentityKey,
                           SendAndReceive     getResponseFromServer)
{
    // Generate client ephemeral key pair
    EphemeralKeyPair clientEphemeralKeyPair{};
    Bytes clientPublicEphemeralKey{ clientEphemeralKeyPair.getPublicEphemeralKey() };
    
    // Client request 1: Client random bytes + Client public ephemeral key
    Bytes clientRequest1{ generateRandomBytes(g_randomBytesSize) };
    clientRequest1.insert(clientRequest1.end(), 
                          clientPublicEphemeralKey.begin(), 
                          clientPublicEphemeralKey.end());

    // Server response 1: Server random bytes + Server public ephemeral key 
    //                  + Server public identity key (encrypted) + Server handshake signature (encrypted)
    //                  + authentication tag
    Bytes serverResponse1{ getResponseFromServer(clientRequest1) };
    if (serverResponse1.size() != g_randomBytesSize 
                                + EphemeralKeyPair::getPublicEphemeralKeySize()
                                + IdentityKeyPair::getPublicIdentityKeySize()
                                + IdentityKeyPair::getSignatureSize())
        logger::THROW("Invalid server response.");

    // Shared secret: derive from ephemeral keys
    Bytes sharedSecret{
        clientEphemeralKeyPair.deriveSharedSecret(
            Bytes{serverResponse1.begin() + g_randomBytesSize,
                  serverResponse1.begin() + g_randomBytesSize + EphemeralKeyPair::getPublicEphemeralKeySize() }) };

    // Hello: Client random bytes + Client public ephemeral key + Server random bytes + Server public ephemeral key
    Bytes hello{ std::move(clientRequest1) };   // clientRequest1 is no longer needed, so it can be moved
    hello.insert(hello.end(), 
                 serverResponse1.begin(), 
                 serverResponse1.begin() + g_randomBytesSize + EphemeralKeyPair::getPublicEphemeralKeySize());
    
    // Hash the hello messages
    Bytes helloHash{ hash(hello) };

    // Handshake secret
    KeyDerivationFunction keyDerivationFunction{};
    Bytes inputKeyingMaterial(KeyDerivationFunction::getPseudoRandomKeySize(), 0x00);
    Bytes salt{};
    Bytes earlySecret{ 
        keyDerivationFunction.extractPseudoRandomKey(
            inputKeyingMaterial, 
            salt) };
    Bytes emptyHash{ hash(Bytes{}) };
    Bytes derivedSecret{
        keyDerivationFunction.expandPseudoRandomKey(
            earlySecret,
            convertTextStringToBytes("derived"),
            emptyHash,
            KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes handshakeSecret{ 
        keyDerivationFunction.extractPseudoRandomKey(
            sharedSecret, 
            derivedSecret) };

    // Handshake encrypter
    Bytes clientHandshakeTrafficSecret{
        keyDerivationFunction.expandPseudoRandomKey(
            handshakeSecret,
            convertTextStringToBytes("c hs traffic"),
            helloHash,
            KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes clientHandshakeKey{
        keyDerivationFunction.expandPseudoRandomKey(
            clientHandshakeTrafficSecret,
            convertTextStringToBytes("key"),
            Bytes{},
            Encrypter::getKeySize()) };
    Bytes clientHandshakeInitializationVector{
        keyDerivationFunction.expandPseudoRandomKey(
            clientHandshakeTrafficSecret,
            convertTextStringToBytes("iv"),
            Bytes{},
            Encrypter::getKeySize()) };
    Encrypter handshakeEncrypter{ clientHandshakeKey, clientHandshakeInitializationVector };

    // Handshake decrypter
    Bytes serverHandshakeTrafficSecret{
        keyDerivationFunction.expandPseudoRandomKey(
            handshakeSecret,
            convertTextStringToBytes("s hs traffic"),
            helloHash,
            KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes serverHandshakeKey{
        keyDerivationFunction.expandPseudoRandomKey(
            serverHandshakeTrafficSecret,
            convertTextStringToBytes("key"),
            Bytes{},
            Encrypter::getKeySize()) };
    Bytes serverHandshakeInitializationVector{
        keyDerivationFunction.expandPseudoRandomKey(
            serverHandshakeTrafficSecret,
            convertTextStringToBytes("iv"),
            Bytes{},
            Encrypter::getKeySize()) };
    Decrypter handshakeDecrypter{ serverHandshakeKey, serverHandshakeInitializationVector };

    
    
    // Verify server identity


    // Client request 2: Client public identity key + Client handshake signature + Client handshake finished
    //IdentityKeyPair clientIdentityKeyPair{ clientPrivateIdentityKeyFilename };
    //Bytes clientPublicIdentityKey{ clientIdentityKeyPair.getPublicIdentityKey() };
    //Bytes clientRequest2{  };
    //clientRequest2.insert(clientRequest1.end(),
    //    ,
    //    ;

    //additional authenticated data!

    // Server response 2: Server handshake finished

    return Securer{};
}*/


//Securer buildServerSecurer(std::string const& serverPrivateIdentityKeyFilename,
//                           Bytes const& clientPublicIdentityKey,
//                           std::function<Bytes(Bytes const&)> getResponseFromServer)
//{
//
//}

} // namespace crypto
} // namespace jm
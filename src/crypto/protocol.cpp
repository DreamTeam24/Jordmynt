#include "protocol.h"
#include "logger/logger.h"
#include "common/conversions.h"
//#include "ephemeralkeypair.h"
//#include "generaterandombytes.h"
//#include "hash.h"
//#include "identitykeypair.h"
//#include "keyderivationfunction.h"
//
//#include <type_traits>
//#include <algorithm>
#include <cassert>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
enum Protocol::ContentType : Byte
{
    alert = 21,
    handshake = 22,
    application = 23
};
// ----------------------------------------------------------------------------
enum Protocol::AlertType : Byte
{
    disconnectNotification = 0
};
// ----------------------------------------------------------------------------
Bytes::size_type const Protocol::s_contentTypeSize{ 1 };
// ----------------------------------------------------------------------------
Bytes::size_type const Protocol::s_bodySizeSize{ 2 };
// ----------------------------------------------------------------------------
Bytes::size_type Protocol::getHeaderSize()
{
    return s_contentTypeSize + s_bodySizeSize;
}
// ----------------------------------------------------------------------------
Bytes::size_type Protocol::readBodySize(Bytes const& header)
{
    assert(header.size() == s_contentTypeSize + s_bodySizeSize);
    
    return convert2BytesToSize(Bytes(header.begin() + s_contentTypeSize, header.end()));
}
// ----------------------------------------------------------------------------
Protocol::Protocol()
{

}
// ----------------------------------------------------------------------------
Bytes Protocol::makeDisconnectNotification()
{
    return Bytes{ ContentType::alert,
                  0x00, 0x01,
                  AlertType::disconnectNotification };
}
// ----------------------------------------------------------------------------
Bytes Protocol::encrypt(Bytes const& plaintext)
{
    Bytes header{ convertSizeTo2Bytes(plaintext.size() + Encrypter::getAuthenticationTagSize()) };
    header.insert(header.begin(), ContentType::application);
    Bytes record{ m_encrypter->encrypt(plaintext, header) };
    record.insert(record.begin(), header.begin(), header.end());
    return record;
}
// ----------------------------------------------------------------------------
Bytes Protocol::decrypt(Bytes const& header,
                        Bytes const& body)
{
    try
    {
        Bytes plaintext{ m_decrypter->decrypt(body, header) };
    }
    catch (OpenSSLError const& error)
    {

    }
    return m_decrypter->decrypt(body, header);
}



/*
    Bytes::size_type constexpr g_recordHeaderSize{ 2 };
    // ----------------------------------------------------------------------------
    enum class ContentType : Byte
    {
        alert = 21,
        handshake = 22,
        application = 23
    };
    // ----------------------------------------------------------------------------
    void SecureClient::send(Bytes const& request)
    {
        logger::DEBUG("Encrypting the request ...");

        Bytes recordHeader{};

        Bytes ciphertextAndAuthenticationTag{ m_encrypter->encrypt(request, recordHeader) };

        Bytes record(recordHeader.size() + ciphertextAndAuthenticationTag.size());
        std::copy(ciphertextAndAuthenticationTag.begin(), ciphertextAndAuthenticationTag.end(),
            std::copy(recordHeader.begin(), recordHeader.end(), record.begin()));

        m_client.send(std::move(record));
    }
    // ----------------------------------------------------------------------------
    Bytes SecureClient::receive()
    {
        Bytes recordHeader{ m_client.receive(g_recordHeaderSize) };

        // decode header

        Bytes ciphertextAndAuthenticationTag{ m_client.receive(g_recordHeaderSize) };

        logger::DEBUG("Decrypting the response ...");

        Bytes response{ m_decrypter->decrypt(ciphertextAndAuthenticationTag, recordHeader) };

        return response;
    }
    // ----------------------------------------------------------------------------
constexpr Bytes::size_type g_randomBytesSize{ 32 };
constexpr Bytes::size_type g_headerSize{ 3 };
constexpr Bytes::size_type g_byteMax{  };

constexpr Bytes::size_type g_additionalAuthenticatedDataSize{ 3 };
// ----------------------------------------------------------------------------
Securer buildHandshakeSecurer(Bytes              sharedSecret,
                              Bytes              hello,
                              std::string const& selfLabel,
                              std::string const& peerLabel)
{
    // Hash of the hello conversation
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
    Bytes selfHandshakeTrafficSecret{
        keyDerivationFunction.expandPseudoRandomKey(
            handshakeSecret,
            convertTextStringToBytes(selfLabel),
            helloHash,
            KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes selfHandshakeKey{
        keyDerivationFunction.expandPseudoRandomKey(
            selfHandshakeTrafficSecret,
            convertTextStringToBytes("key"),
            Bytes{},
            Encrypter::getKeySize()) };
    Bytes selfHandshakeInitializationVector{
        keyDerivationFunction.expandPseudoRandomKey(
            selfHandshakeTrafficSecret,
            convertTextStringToBytes("iv"),
            Bytes{},
            Encrypter::getKeySize()) };
    Encrypter handshakeEncrypter{ selfHandshakeKey, selfHandshakeInitializationVector };

    // Handshake decrypter
    Bytes peerHandshakeTrafficSecret{
        keyDerivationFunction.expandPseudoRandomKey(
            handshakeSecret,
            convertTextStringToBytes(peerLabel),
            helloHash,
            KeyDerivationFunction::getPseudoRandomKeySize()) };
    Bytes peerHandshakeKey{
        keyDerivationFunction.expandPseudoRandomKey(
            peerHandshakeTrafficSecret,
            convertTextStringToBytes("key"),
            Bytes{},
            Encrypter::getKeySize()) };
    Bytes peerHandshakeInitializationVector{
        keyDerivationFunction.expandPseudoRandomKey(
            peerHandshakeTrafficSecret,
            convertTextStringToBytes("iv"),
            Bytes{},
            Encrypter::getKeySize()) };
    Decrypter handshakeDecrypter{ peerHandshakeKey, peerHandshakeInitializationVector };

    return Securer{ std::move(handshakeEncrypter), std::move(handshakeDecrypter) };
}
// ----------------------------------------------------------------------------
Securer buildClientHandshakeSecurer(Bytes sharedSecret,
                                    Bytes hello)
{
    return buildHandshakeSecurer(std::move(sharedSecret), std::move(hello), "c hs traffic", "s hs traffic");
}
// ----------------------------------------------------------------------------
Securer buildServerHandshakeSecurer(Bytes sharedSecret,
                                    Bytes hello)
{
    return buildHandshakeSecurer(std::move(sharedSecret), std::move(hello), "s hs traffic", "c hs traffic");
}
// ----------------------------------------------------------------------------
Securer buildClientSecurer(std::string const& clientPrivateIdentityKeyFilename,
                           Bytes const&       serverPublicIdentityKey,
                           Send               sendRequestToServer,
                           Receive            receiveResponseFromServer)
{
    // Generate client random bytes
    Bytes clientRandomBytes{ generateRandomBytes(g_randomBytesSize) };
    
    // Generate client ephemeral key pair
    EphemeralKeyPair clientEphemeralKeyPair{};
    Bytes clientPublicEphemeralKey{ clientEphemeralKeyPair.getPublicEphemeralKey() };
    
    // Client hello = Client random bytes
    //              + Client public ephemeral key
    Bytes::size_type clientHelloSize{ clientRandomBytes.size() + clientPublicEphemeralKey.size() };
    Bytes clientRequest1{};
    clientRequest1.reserve(g_headerSize + clientHelloSize);




    clientRequest1.push_back(static_cast<std::underlying_type<MessageType>::type>(MessageType::ClientHello));
    clientRequest1.push_back(static_cast<Byte>(clientHelloSize/));
    clientRequest1.push_back(static_cast<Byte>(clientHelloSize / ));
    clientRequest1.insert(clientRequest1.end(),
                          clientRandomBytes.begin(),
                          clientRandomBytes.end());
    clientRequest1.insert(clientRequest1.end(),
                          clientPublicEphemeralKey.begin(),
                          clientPublicEphemeralKey.end());
    
    Bytes clientRequest1{ generateRandomBytes(g_randomBytesSize) };
    clientRequest1.insert(clientRequest1.end(), 
                          clientPublicEphemeralKey.begin(), 
                          clientPublicEphemeralKey.end());
    sendRequestToServer(clientRequest1);


    Bytes serverHeader1{ receiveResponseFromServer(g_headerSize) };



    // Server response 1 = Server random bytes
    //                   + Server public ephemeral key
    //                   + type of encrypted message
    //                   + size of encrypted message
    //                   + Server public identity key (encrypted)
    //                   + Server signature (encrypted)
    //                   + authentication tag
    Bytes serverResponse1{ receiveResponseFromServer(clientRequest1) };
    
    
    Bytes::const_iterator serverRandomBytesBegin{ serverResponse1.begin() };
    Bytes::const_iterator serverRandomBytesEnd{ serverRandomBytesBegin + g_randomBytesSize };
    Bytes::const_iterator serverPublicEphemeralKeyBegin{ serverRandomBytesEnd };
    Bytes::const_iterator serverPublicEphemeralKeyEnd{ serverPublicEphemeralKeyBegin + EphemeralKeyPair::getPublicEphemeralKeySize() };
    Bytes::const_iterator additionalAuthenticatedDataBegin{ serverPublicEphemeralKeyEnd };
    Bytes::const_iterator additionalAuthenticatedDataEnd{ additionalAuthenticatedDataBegin + g_additionalAuthenticatedDataSize };


    if (serverResponse1.size() < g_randomBytesSize 
                               + EphemeralKeyPair::getPublicEphemeralKeySize()
                               + g_headerSize)
        logger::THROW("Invalid server response.");
    Bytes::size_type encryptedPartSize{ static_cast<Bytes::size_type>(serverResponse1[g_randomBytesSize + EphemeralKeyPair::getPublicEphemeralKeySize()]) };
    if (serverResponse1.size() < g_randomBytesSize
                               + EphemeralKeyPair::getPublicEphemeralKeySize()
                               + g_headerSize
                               + encryptedPartSize
                               + Encrypter::getAuthenticationTagSize())
        logger::THROW("Invalid server response.");

    // Derive shared secret from ephemeral keys
    Bytes sharedSecret{
        clientEphemeralKeyPair.deriveSharedSecret(
            Bytes{ serverPublicEphemeralKeyBegin, serverPublicEphemeralKeyEnd}) };

    // Hello = Client random bytes
    //       + Client public ephemeral key
    //       + Server random bytes
    //       + Server public ephemeral key
    Bytes hello{ std::move(clientRequest1) };   // clientRequest1 is no longer needed, so it can be moved
    hello.insert(hello.end(), 
                 serverRandomBytesBegin,
                 serverPublicEphemeralKeyEnd);
    
    // Build handshake securer
    Securer handshakeSecurer{ buildHandshakeSecurer(sharedSecret, hello) };

    // Decrypt server public identity key and signature
    handshakeSecurer.decrypter.decrypt(
        Bytes{}
    )

    // Create client identity key pair
    IdentityKeyPair clientIdentityKeyPair{ clientPrivateIdentityKeyFilename };
    Bytes clientPublicIdentityKey{ clientIdentityKeyPair.getPublicIdentityKey() };
    
    // Verify server identity
    clientIdentityKeyPair.isSignatureValid()

    // Client request 2: Client public identity key + Client handshake signature + Client handshake finished
    
    Bytes clientRequest2{  };
    clientRequest2.insert(clientRequest1.end(),
        ,
        ;

    additional authenticated data!

    // Server response 2: Server handshake finished

    return Securer{};
}
// ----------------------------------------------------------------------------
//Securer buildServerSecurer(std::string const& serverPrivateIdentityKeyFilename,
//                           Bytes const& clientPublicIdentityKey,
//                           std::function<Bytes(Bytes const&)> getResponseFromServer)
//{
//
//}*/
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
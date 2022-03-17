#ifndef JM_CRYPTO_PROTOCOL_H
#define JM_CRYPTO_PROTOCOL_H
// ----------------------------------------------------------------------------
#include "cipher.h"
#include "common/byte.h"
#include <optional>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
class Protocol
{
public:
    class Alert
    {};

public:
    static Bytes::size_type getHeaderSize();
    static Bytes::size_type readBodySize(Bytes const& header);
    
    Protocol();

    // Handshake
    Bytes makeClientHello();
    Bytes makeServerHelloAndAuthentication(Bytes const& clientHelloHeader,
                                           Bytes const& clientHelloBody);
    Bytes makeClientAuthentication(Bytes const& serverHelloAndAuthenticationHeader,
                                   Bytes const& serverHelloAndAuthenticationBody);
    
    // Application
    Bytes encrypt(Bytes const& plaintext);
    Bytes decrypt(Bytes const& header,
                  Bytes const& body);

    // Disconnect
    Bytes makeDisconnectNotification();

private:    
    enum ContentType : Byte;
    enum AlertType : Byte;

    static Bytes::size_type const s_contentTypeSize;
    static Bytes::size_type const s_bodySizeSize;

    std::optional<Cipher> m_encrypter;
};

// ----------------------------------------------------------------------------
// https://tls13.ulfheim.net/, https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-vectors-02
//
//Securer buildClientSecurer(std::string const& clientPrivateIdentityKeyFilename,
//                           Bytes const&       serverPublicIdentityKey,
//                           Send               sendRequestToServer,
//                           Receive            receiveResponseFromServer);
//Securer buildServerSecurer(std::string const& serverPrivateIdentityKeyFilename,
//                           Bytes const&       clientPublicIdentityKey,
//                           SendAndReceive     getResponseFromServer);
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_PROTOCOL_H
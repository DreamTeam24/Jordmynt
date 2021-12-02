#ifndef JM_CRYPTO_SECURER_H
#define JM_CRYPTO_SECURER_H

#include "decrypter.h"
#include "encrypter.h"
#include <functional>
#include <string>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;
using SendAndReceive = std::function<Bytes(Bytes const&)>;

struct Securer
{
    Encrypter encrypter;
    Decrypter decrypter;
};

// https://tls13.ulfheim.net/, https://datatracker.ietf.org/doc/html/draft-ietf-tls-tls13-vectors-02

//Securer buildClientSecurer(std::string const& clientPrivateIdentityKeyFilename,
//                           Bytes const&       serverPublicIdentityKey,
//                           SendAndReceive     getResponseFromServer);
//Securer buildServerSecurer(std::string const& serverPrivateIdentityKeyFilename,
//                           Bytes const&       clientPublicIdentityKey,
//                           SendAndReceive     getResponseFromServer);

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_SECURECLIENT_H
#ifndef JM_CRYPTO_KEYPAIR_H
#define JM_CRYPTO_KEYPAIR_H

#include "bytes.h"
#include <openssl/evp.h>
#include <string>

namespace jm {
namespace crypto {
                         
// C++ wrapper class around OpenSSL functions involving EVP_PKEY
// to derive a shared secret from one's own private key and a peer's public key.
class KeyPair
{
public:
    KeyPair();
    KeyPair(Bytes const& privateKey);
    KeyPair(std::string const& privateKeyFilename);
    ~KeyPair();

    KeyPair(KeyPair const& copyFromMe) = delete;              // non-copyable
    KeyPair& operator=(KeyPair const& assignFromMe) = delete; // non-copyable

    static Bytes::size_type getPublicKeySize();

    Bytes getPublicKey() const;
    Bytes deriveSharedSecret(Bytes const& peerPublicKey) const;

private:
    static constexpr int              s_id{ EVP_PKEY_X25519 };
    static constexpr Bytes::size_type s_keySize{ 32 };  // both private and public key are 32 bytes (256 bits) long

    EVP_PKEY* m_keyPair;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_KEYPAIR_H
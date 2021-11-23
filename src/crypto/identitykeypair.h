#ifndef JM_CRYPTO_IDENTITYKEYPAIR_H
#define JM_CRYPTO_IDENTITYKEYPAIR_H

#include <openssl/evp.h>
#include <string>
#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

class IdentityKeyPair
{
public:
    static Bytes::size_type getPublicIdentityKeySize();
    
    IdentityKeyPair();
    IdentityKeyPair(Bytes const& privateIdentityKey);
    IdentityKeyPair(std::string const& privateIdentityKeyFilename);
    ~IdentityKeyPair();

    IdentityKeyPair(IdentityKeyPair const& identityKeyPair) = delete;               // not copyable
    IdentityKeyPair& operator=(IdentityKeyPair const& identityKeyPair) = delete;    // not copy-asignable

    IdentityKeyPair(IdentityKeyPair&& identityKeyPair) noexcept;                    // movable
    IdentityKeyPair& operator=(IdentityKeyPair&& identityKeyPair) noexcept;         // move-assignable

    Bytes getPublicIdentityKey() const;
    Bytes sign(Bytes const& message) const;

private:
    static constexpr int              s_id{ EVP_PKEY_ED25519 };
    static constexpr Bytes::size_type s_keySize{ 32 };  // both private and public key are 32 bytes (256 bits) long
    static constexpr Bytes::size_type s_signatureSize{ 64 };

    void initialize();
    void free();

    EVP_PKEY*     m_keyPair;    // m_keyContext depends on m_keyPair
    EVP_PKEY_CTX* m_keyContext;
    EVP_MD_CTX*   m_hashContext;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_IDENTITYKEYPAIR_H
#ifndef JM_CRYPTO_EPHEMERALKEYPAIR_H
#define JM_CRYPTO_EPHEMERALKEYPAIR_H

#include <openssl/evp.h>
#include <string>
#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

class EphemeralKeyPair
{
public:
    static Bytes::size_type getPublicEphemeralKeySize();
    
    EphemeralKeyPair();
    EphemeralKeyPair(Bytes const& privateEphemeralKey);
    EphemeralKeyPair(std::string const& privateEphemeralKeyFilename);
    ~EphemeralKeyPair();

    EphemeralKeyPair(EphemeralKeyPair const& ephemeralKeyPair) = delete;             // not copyable
    EphemeralKeyPair& operator=(EphemeralKeyPair const& ephemeralKeyPair) = delete;  // not copy-asignable

    EphemeralKeyPair(EphemeralKeyPair&& ephemeralKeyPair) noexcept;                  // movable
    EphemeralKeyPair& operator=(EphemeralKeyPair&& ephemeralKeyPair) noexcept;       // move-assignable

    Bytes getPublicEphemeralKey() const;
    Bytes deriveSharedSecret(Bytes const& peerPublicEphemeralKey) const;

private:
    static inline EVP_MD const* const s_hashFunction{ EVP_sha256() }; 
    static constexpr int              s_id{ EVP_PKEY_X25519 };
    static constexpr Bytes::size_type s_keySize{ 32 };      // both private and public key are 32 bytes (256 bits) long

    void initialize();
    void free();

    EVP_PKEY*     m_keyPair;    // m_keyContext depends on m_keyPair
    EVP_PKEY_CTX* m_keyContext;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_EPHEMERALKEYPAIR_H
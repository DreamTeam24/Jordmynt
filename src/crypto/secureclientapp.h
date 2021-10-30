#ifndef JM_CRYPTO_SECURECLIENTAPP_H
#define JM_CRYPTO_SECURECLIENTAPP_H

#include <jm/crypto/bytes.h>
#include <memory>
#include <string>

namespace jm {
namespace crypto {

class SecureClientApp
{
public:
    SecureClientApp(std::string const& privateKeyClientFilename,
                    std::string const& publicKeysServersFilename);
    ~SecureClientApp();

    SecureClientApp(SecureClientApp const& copyFromMe) = delete;              // non-copyable
    SecureClientApp& operator=(SecureClientApp const& assignFromMe) = delete; // non-copyable

    void connect(Endpoints          server,
                 std::string const& filenamePublicKeyServer,
                 ConnectHandler     handler) const;
    void send(Bytes const& message) const;
    Bytes receive() const;

private:
    class Implementation;

    std::unique_ptr<Implementation> m_implementation;
};

}
}

#endif // JM_CRYPTO_SECURECLIENTAPP_H
/*#include <jm/crypto/securesocket.h>
#include "crypto/cipher.h"
#include "crypto/functions.h"
#include "crypto/keyderivationfunction.h"
#include "crypto/keypair.h"
#include "logger/logger.h"

namespace jm {
namespace crypto {

SecureSocket::SecureSocket(boost::asio::ip::tcp::socket socket,
                           SocketType                   socketType,
                           std::string const&           filenamePrivateKey) :
    m_implementation{ std::make_unique<Implementation>(socket, socketType, filenamePrivateKey) }
{
}

SecureSocket::~SecureSocket()
{
}

void SecureSocket::connect(Endpoints          server,
                           std::string const& filenamePublicKeyServer,
                           ConnectHandler     handler) const
{
    m_implementation->connect(server,
                              filenamePublicKeyServer,
                              handler);
}

void SecureSocket::send(Bytes const& message) const
{
    m_implementation->send();
}

Bytes SecureSocket::receive() const
{
    return m_implementation->receive();
}
    
class SecureSocket::Implementation
{
public:
    using Socket = boost::asio::ip::tcp::socket;
    
    Implementation(Socket&&           socket,
                   SocketType         socketType,
                   std::string const& filenamePrivateKey) :
        m_socket{ std::move(socket) },
        m_socketType{ socketType },
        m_buffer(s_bufferSize), // {}-initialization does not set the size
        m_randomBytes{},
        m_keyPair{ std::make_unique<KeyPair>(filenamePrivateKey, KeyPair::KeyType::PRIVATE) },
        m_keyPairPeer{ nullptr },
        m_keyPairEphemeral{ nullptr },
        m_keyDerivationFunction{ std::make_unique<KeyDerivationFunction>() }
    {
    }

    void connect(Endpoints          server,
                 std::string const& filenamePublicKeyServer,
                 ConnectHandler     handler)
    {
        logger::INFO("Connecting to the server ...");
        
        // Read server public key from file
        m_keyPairPeer = std::make_unique<KeyPair>(filenamePublicKeyServer, KeyPair::KeyType::PUBLIC);
        
        boost::asio::async_connect(
            m_socket,
            server,
            [this](boost::system::error_code const&      error,
                   boost::asio::ip::tcp::endpoint const& endpoint)
            {
                if (error)
                    logger::THROW("Failed to connect to the server: " + error.message());
                else
                    this->sendSessionEncryptionData();
            }
        );
    }

    void sendSessionEncryptionData()
    {
        logger::INFO("Sending session encryption data ...");

        // Put random bytes and ephemeral public key in buffer (in that order)
        m_randomBytes = crypto::generateRandomBytes(s_randomBytesSize);
        m_keyPairEphemeral = std::make_unique<KeyPair>(crypto::KeyPair{});
        crypto::Bytes ephemeralPublicKey{ m_keyPairEphemeral->getPublicKey() };
        m_buffer.resize(m_randomBytes.size() + ephemeralPublicKey.size());
        std::copy(ephemeralPublicKey.begin(), ephemeralPublicKey.end(),
            std::copy(m_randomBytes.begin(), m_randomBytes.end(), m_buffer.begin()));

        boost::asio::async_write(
            m_socket,
            boost::asio::buffer(m_buffer, m_buffer.size()),
            [this](boost::system::error_code const& error,
                   std::size_t                      )
            {
                if (error)
                    logger::THROW("Failed to send session encryption data to the server: " + error.message());
                else
                {
                    switch (m_socketType)
                    {
                        case SocketType::CLIENT:
                        {
                            this->receiveSessionEncryptionData();
                        }
                        case SocketType::SERVER:
                        {
                            this->receiveSessionEncryptionData();
                        }
                    }
                }
            }
        );
    }

    void receiveSessionEncryptionData()
    {
        logger::INFO("Receiving session encryption data ...");

        m_buffer.resize(s_randomBytesSize + KeyPair::getPublicKeySize());
        boost::asio::async_read(
            m_socket,
            boost::asio::buffer(m_buffer, m_buffer.size()),
            [this](boost::system::error_code error,
                   std::size_t               )
            {
                if (error)
                    logger::THROW("Failed to receive session encryption data from the server: " + error.message());
                else
                {
                    switch (m_socketType)
                    {
                    case SocketType::CLIENT:
                    {
                        this->receiveSessionEncryptionData();
                    }
                    case SocketType::SERVER:
                    {
                        this->receiveSessionEncryptionData();
                    }
                    }
                    
                    
                    // Buffer contains random bytes and ephemeral public key of server (in that order)
                    crypto::Bytes::iterator serverRandomBytesBegin{ this->m_buffer.begin() };
                    crypto::Bytes::iterator serverRandomBytesEnd{ std::next(serverRandomBytesBegin, s_randomBytesSize) };
                    crypto::Bytes::iterator serverEphemeralPublicKeyBegin{ serverRandomBytesEnd };
                    crypto::Bytes::iterator serverEphemeralPublicKeyEnd{ std::next(serverEphemeralPublicKeyBegin, KeyPair::getPublicKeySize()) };

                    // Join random bytes of client and server (in that order)
                    crypto::Bytes randomBytes(2 * s_randomBytesSize);        // {}-initialization does not set the size
                    std::copy(serverRandomBytesBegin, serverRandomBytesEnd,
                        std::copy(this->m_randomBytes.begin(), this->m_randomBytes.end(), randomBytes.begin()));

                    // Hash joined random bytes (with SHA256)
                    crypto::Bytes randomBytesHash{ crypto::hash(randomBytes) };

                    // Derive shared secret from ephemeral keys
                    crypto::Bytes sharedSecret{ this2->m_keyPairEphemeral->deriveSharedSecret(crypto::Bytes{serverEphemeralPublicKeyBegin, serverEphemeralPublicKeyEnd}) };

                    //this2->m_keyDerivationFunction->extractPseudoRandomKey(crypto::Bytes{})

                    // Send message
                    this2->sendIdentityProof();
                    
                }
            }
        );
    }

    void setPublicKeyPeer(std::string const& filenamePublicKeyPeer)
    {}
    void generateSessionEncryptionData(Bytes::iterator sessionEncryptionData)
    {}
    void configureEncryption(Bytes::const_iterator sessionEncryptionDataPeer)
    {}
    void generateIdentityProof(Bytes::iterator identityProof)
    {}
    bool isPeerIdentityProven(Bytes::const_iterator identityProofPeer)
    {}

    
    
    
    
    
    
    
    
    Bytes encrypt(Bytes const& plaintext,
        Bytes const& additionalAuthenticatedData,
        Bytes& authenticationTag = Bytes{});      // extra return value
    Bytes decrypt(Bytes const& ciphertext,
        Bytes const& additionalAuthenticatedData,
        Bytes& authenticationTag);          // parameter, non-const because of OpenSSL

private:
    static constexpr crypto::Bytes::size_type s_bufferSize{ 1024 };
    static constexpr crypto::Bytes::size_type s_randomBytesSize{ 32 };

    void sendSessionEncryptionData();
    void receiveSessionEncryptionData();
    void sendIdentityProof();
    void receiveIdentityProof();

    boost::asio::ip::tcp::socket           m_socket;
    SocketType                             m_socketType;
    Bytes                                  m_buffer;
    Bytes                                  m_randomBytes;
    std::unique_ptr<KeyPair>               m_keyPair;
    std::unique_ptr<KeyPair>               m_keyPairPeer;
    std::unique_ptr<KeyPair>               m_keyPairEphemeral;
    std::unique_ptr<KeyDerivationFunction> m_keyDerivationFunction;
    std::unique_ptr<Cipher>                m_cipher;
};



void SecureSocket::secure(boost::asio::ip::tcp::socket& socket, )
{

}


void SecureSocket::sendSessionEncryptionData()
{
    logger::INFO("Sending session encryption data ...");

    // Generate random bytes
    m_randomBytes = crypto::generateRandomBytes(s_randomBytesSize);

    // Generate ephemeral public key
    m_keyPairEphemeral = crypto::KeyPair::create();
    crypto::Bytes ephemeralPublicKey{ m_keyPairEphemeral->getPublicKey() };

    // Put random bytes and ephemeral public key in buffer (in that order)
    std::copy(ephemeralPublicKey.begin(), ephemeralPublicKey.end(),
        std::copy(m_randomBytes.begin(), m_randomBytes.end(), m_buffer.begin()));

    // Send random bytes and ephemeral public key to server
    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2{ shared_from_this() }](boost::system::error_code const& error,
            std::size_t )
    {
        if (error)
            logger::THROW("Failed to send session encryption data to the server: " + error.message());
        else
            this2->receiveSessionEncryptionData();
    }
    );
}

void Client::receiveSessionEncryptionData()
{
    logger::INFO("Receiving session encryption data ...");

    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2{ shared_from_this() }](boost::system::error_code error,
            std::size_t )
    {
        if (error)
            logger::THROW("Failed to receive session encryption data from the server: " + error.message());
        else
        {
            // Buffer contains random bytes and ephemeral public key of server
            crypto::Bytes::iterator serverRandomBytesBegin{ this2->m_buffer.begin() };
            crypto::Bytes::iterator serverRandomBytesEnd{ std::next(serverRandomBytesBegin, s_randomBytesSize) };
            crypto::Bytes::iterator serverEphemeralPublicKeyBegin{ serverRandomBytesEnd };
            crypto::Bytes::iterator serverEphemeralPublicKeyEnd{ std::next(serverEphemeralPublicKeyBegin, jm::crypto::KeyPair::getPublicKeySize()) };

            // Join random bytes of client and server
            // (! this2->m_randomBytes is copied first !)
            crypto::Bytes randomBytes(2 * s_randomBytesSize);        // {}-initialization does not set the size
            std::copy(serverRandomBytesBegin, serverRandomBytesEnd,
                std::copy(this2->m_randomBytes.begin(), this2->m_randomBytes.end(), randomBytes.begin()));

            // Hash joined random bytes (with SHA256)
            crypto::Bytes randomBytesHash{ crypto::hash(randomBytes) };

            // Derive shared secret from ephemeral keys
            crypto::Bytes sharedSecret{ this2->m_keyPairEphemeral->deriveSharedSecret(crypto::Bytes{serverEphemeralPublicKeyBegin, serverEphemeralPublicKeyEnd}) };

            //this2->m_keyDerivationFunction->extractPseudoRandomKey(crypto::Bytes{})

            // Send message
            this2->sendIdentityProof();
        }
    }
    );
}

void Client::sendIdentityProof()
{
    logger::INFO("Sending identity proof ...");

    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2{ shared_from_this() }](boost::system::error_code const& error,
            std::size_t )
    {
        if (error)
            logger::THROW("Failed to send identity proof to the server: " + error.message());
        else
            this2->receiveIdentityProof();
    }
    );
}

void Client::receiveIdentityProof()
{
    logger::INFO("Receiving encryption data ...");

    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2{ shared_from_this() }](boost::system::error_code error,
            std::size_t )
    {
        if (error)
            logger::THROW("Failed to receive identity proof from the server: " + error.message());
        else
            this2->send();
    }
    );
}




void SecureSocket::setPublicKeyPeer(std::string const& filenamePublicKeyPeer)
{
    m_keyPairPeer = crypto::KeyPair::createFromPublicKeyFile(filenamePublicKeyPeer);
}

void SecureSocket::generateSessionEncryptionData(Bytes::iterator sessionEncryptionData)
{
    m_randomBytes = crypto::generateRandomBytes(s_randomBytesSize);
    m_keyPairEphemeral = crypto::KeyPair::create();
    crypto::Bytes ephemeralPublicKey{ m_keyPairEphemeral->getPublicKey() };
    std::copy(ephemeralPublicKey.begin(), ephemeralPublicKey.end(),
        std::copy(m_randomBytes.begin(), m_randomBytes.end(), sessionEncryptionData));
}

void SecureSocket::configureEncryption(Bytes::const_iterator sessionEncryptionDataPeer)
{
    // Buffer contains random bytes and ephemeral public key of server
    crypto::Bytes::iterator serverRandomBytesBegin{ this2->m_buffer.begin() };
    crypto::Bytes::iterator serverRandomBytesEnd{ std::next(serverRandomBytesBegin, s_randomBytesSize) };
    crypto::Bytes::iterator serverEphemeralPublicKeyBegin{ serverRandomBytesEnd };
    crypto::Bytes::iterator serverEphemeralPublicKeyEnd{ std::next(serverEphemeralPublicKeyBegin, jm::crypto::KeyPair::getPublicKeySize()) };

    // Join random bytes of client and server
    // (! this2->m_randomBytes is copied first !)
    crypto::Bytes randomBytes(2 * s_randomBytesSize);        // {}-initialization does not set the size
    std::copy(serverRandomBytesBegin, serverRandomBytesEnd,
        std::copy(this2->m_randomBytes.begin(), this2->m_randomBytes.end(), randomBytes.begin()));

    // Hash joined random bytes (with SHA256)
    crypto::Bytes randomBytesHash{ crypto::hash(randomBytes) };

    // Derive shared secret from ephemeral keys
    crypto::Bytes sharedSecret{ this2->m_keyPairEphemeral->deriveSharedSecret(crypto::Bytes{serverEphemeralPublicKeyBegin, serverEphemeralPublicKeyEnd}) };

    //this2->m_keyDerivationFunction->extractPseudoRandomKey(crypto::Bytes{})
}

void SecureSocket::generateIdentityProof(Bytes::iterator identityProof)
{

}

bool SecureSocket::isPeerIdentityProven(Bytes::const_iterator identityProofPeer)
{
    return true;
}

}
}
*/
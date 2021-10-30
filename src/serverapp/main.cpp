#include "server/server.h"
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        // Create server
        boost::asio::io_context ioContext{};
        jm::server::Server server{ ioContext, 7 };
        server.accept(); //, "server_private.pem")
            

        // Run asynchronous calls
        ioContext.run();
    }
    catch (std::exception exception)
    {
        std::cerr << exception.what() << std::endl;
    }

    system("pause");

    return 0;
}

//#include <openssl/evp.h>
//#include <openssl/pem.h>
//
//int main(int argc, char** argv)
//{
//    // Generate private and public key
//    EVP_PKEY* pkey = NULL;
//    EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(NID_KeyPair, NULL);
//    EVP_PKEY_keygen_init(pctx);
//    EVP_PKEY_keygen(pctx, &pkey);
//
//    // Write private and public key to file
//    BIO* out;
//    out = BIO_new_file("server_private.pem", "w+");
//    PEM_write_bio_PrivateKey(out, pkey, nullptr, nullptr, 0, nullptr, nullptr);
//    BIO_flush(out);
//    out = BIO_new_file("server_public.pem", "w+");
//    PEM_write_bio_PUBKEY(out, pkey);
//    BIO_flush(out);
//
//    // Free memory
//    EVP_PKEY_CTX_free(pctx);
//    EVP_PKEY_free(pkey);
//    BIO_free(out);
//
//    return 0;
//}
#ifndef JM_CRYPTO_RECORD_H
#define JM_CRYPTO_RECORD_H
// ----------------------------------------------------------------------------
#include "common/byte.h"
// ----------------------------------------------------------------------------
namespace jm {
namespace network {
// ----------------------------------------------------------------------------
class Record
{
public:
    enum class ContentType : Byte;
    enum class AlertType : Byte;
    
    static Size getHeaderSize();
    static Size readBodySize(Bytes const& header);
    
    Record(ContentType contentType,
           BytesView  plaintext,
           BytesSpan&  buffer);
    
    
    
    //Record(Bytes header,
    //       Size  macSize);
    //Record(AlertType alertType,
    //       Size      macSize);

    //BytesView getPlaintext() const;
    //BytesHandle setPlaintext(BytesHandle plaintext);

    //BytesView getCiphertext() const;
    //BytesHandle setCiphertext(BytesHandle ciphertext);

    BytesView getHeader() const;


    BytesSpan getBody() const;

    Span getMessageAuthenticationCode() const;

    Span getCiphertext() const;

private:
    Bytes m_record;
    Bytes m_plaintext;
};
// ----------------------------------------------------------------------------
} // namespace network
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_RECORD_H
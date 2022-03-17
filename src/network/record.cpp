#include "record.h"
#include <utility>
// ----------------------------------------------------------------------------
namespace jm {
namespace network {
// ----------------------------------------------------------------------------
enum Cipher::Record::ContentType : Byte
{
    alert = 21,
    handshake = 22,
    application = 23
};
// ----------------------------------------------------------------------------
enum Cipher::Record::AlertType : Byte
{
    disconnectNotification = 0
};
// ----------------------------------------------------------------------------
Cipher::Record::Record(ContentType      contentType,
    Bytes            plaintext) :
    m_plaintext{ std::move(plaintext) },
    m_record{ static_cast<Byte>(contentType), 0x00, 0x00 }
{
    m_record.reserve();
}
// ----------------------------------------------------------------------------
Record::Record(Bytes            header,
    Bytes::size_type macSize)
{

}
// ----------------------------------------------------------------------------
Record::Record(AlertType        alertType,
    Bytes::size_type macSize)
{

}
// ----------------------------------------------------------------------------


Record::Record(Bytes            plaintext,
               ContentType      contentType,
               Bytes::size_type macSize) :
    m_plaintext{ std::move(plaintext) },
    m_record{ static_cast<Byte>(contentType), 0x00, 0x00 }
{
    m_record.reserve();
}
// ----------------------------------------------------------------------------
Record::Record(Bytes            header,
               Bytes::size_type macSize)
{

}
// ----------------------------------------------------------------------------
Record::Record(AlertType        alertType,
               Bytes::size_type macSize)
{

}
// ----------------------------------------------------------------------------
} // namespace network
} // namespace jm
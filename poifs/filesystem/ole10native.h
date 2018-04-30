#ifndef OLE10NATIVE_H
#define OLE10NATIVE_H

#include <string>

#include "../common/bytestream.h"
#include "../poifs_global.h"

namespace poifs {
namespace filesystem {

struct Ole10Native_exception : public std::exception { };

/**
 * Represents an Ole10Native record which is wrapped around certain binary
 * files being embedded in OLE2 documents.
 */
class Ole10Native
{
public:
    static const std::u16string OLE10_NATIVE;
    static constexpr unsigned MAX_RECORD_LENGTH = 100000000;
private:
    enum class EncodingMode {
            /**
             * the data is stored in parsed format - including label, command, etc.
             */
            parsed,
            /**
             * the data is stored raw after the length field
             */
            unparsed,
            /**
             * the data is stored raw after the length field and the flags1 field
             */
            compact
    };

    uint32_t m_totalSize;
    uint16_t m_flags1;
    std::string m_label;
    std::string m_fileName;
    uint16_t m_flags2;
    uint16_t m_unknown1;
    std::string m_command;
    std::vector<unsigned char> m_dataBuffer;
    uint16_t m_flags3;
    EncodingMode m_mode;
public:
    Ole10Native(bytestream::bytestream &stream);
    Ole10Native(std::string label, std::string filename, std::string command, std::vector<unsigned char> data) :
        m_label(std::move(label)), m_fileName(std::move(filename)), m_command(std::move(command)),
        m_dataBuffer(std::move(data)), m_mode(EncodingMode::parsed)
    { }

    POIFS_GETTER_SETTER(totalSize, uint32_t)
    POIFS_GETTER_SETTER(flags1, uint16_t)
    POIFS_GETTER_SETTER(label, const std::string &)
    POIFS_GETTER_SETTER(fileName, const std::string &)
    POIFS_GETTER_SETTER(flags2, uint16_t)
    POIFS_GETTER_SETTER(unknown1, uint16_t)
    POIFS_GETTER_SETTER(command, const std::string &)
    size_t dataSize() const {
        return m_dataBuffer.size();
    }
    POIFS_GETTER_SETTER(dataBuffer, const std::vector<unsigned char> &)
    POIFS_GETTER_SETTER(flags3, uint16_t)
    POIFS_GETTER_SETTER(mode, EncodingMode)

};

}
}

#endif // OLE10NATIVE_H

#ifndef OLE10NATIVE_H
#define OLE10NATIVE_H

#include <string>

#include "../common/bytestream.h"

namespace poifs {
namespace filesystem {

class Ole10Native
{
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


};

}
}

#endif // OLE10NATIVE_H

#include "ole10native.h"
#include "stringutils.h"

using namespace poifs::filesystem;

/*
 * Helper - determine length of zero terminated string (ASCIIZ).
 */
static size_t getStringLength(bytestream::bytestream &stream) {
    return static_cast<size_t>(stream.find('\0') - stream.pos()) + 1;
}

Ole10Native::Ole10Native(bytestream::bytestream &stream) {

    stream >> m_totalSize;

    uint16_t tmp;
    stream >> tmp;
    m_mode = EncodingMode::unparsed;
    if (tmp == 2) {
        auto c = stream.peek();
        static_assert (std::is_unsigned<decltype (c)>::value, "it should be unsigned, else uncomment next line");
        if ((/*0x00 <= c &&*/ c <= 0x1F) || (0x7F <= c && c <= 0x9F))
            m_mode = EncodingMode::compact;
        else
            m_mode = EncodingMode::parsed;
    }
    stream.unread();

    uint32_t dataSize = 0, len32 = 0;
    size_t len = 0;
    switch (m_mode) {
        case EncodingMode::parsed:
            stream >> m_flags1;

            len = getStringLength(stream);
            m_label = string_utils::getFromCompressedUnicode(stream, len);
            m_label.pop_back();

            len = getStringLength(stream);
            m_fileName = string_utils::getFromCompressedUnicode(stream, len);
            m_label.pop_back();

            stream >> m_flags2;
            stream >> m_unknown1;

            stream >> len32;
            m_command = string_utils::getFromCompressedUnicode(stream, len32);

            break;
        case EncodingMode::compact:
            stream >> m_flags1;
            dataSize = m_totalSize - sizeof(m_flags1);
            break;
        case EncodingMode::unparsed:
            dataSize = m_totalSize;
            break;
    }
}

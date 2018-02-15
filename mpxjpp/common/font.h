#ifndef FONT_H
#define FONT_H

#include <string>

#include "../mpxjpp-gens.h"

namespace mpxjpp {
namespace common {

enum class BackgroundPattern {
    TRANSPARENT,
    SOLID,
    LIGHTDOTTED,
    DOTTED,
    HEAVYDOTTED,
    BACKSLASH,
    FORWARDSLASH,
    INVERSEBACKSLASH,
    INVERSEFORWARDSLASH,
    LIGHTVERTICALSTRIPE,
    HEAVYVERTICALSTRIPE,
    CHECKERED,
    DENSEFORWARDSLASH,
    INVERSECHECKERED
};

struct Color final {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a = 0xFF;
};

class FontBase final {
private:
    int m_index;
    std::string m_name;
    int m_size;
public:
    FontBase(int index, const std::string &name, int size) :
        m_index(index), m_name(name), m_size(size)
    {}
    FontBase(int index, std::string &&name, int size) :
        m_index(index), m_name(std::move(name)), m_size(size)
    {}

    MPXJPP_GETTER(name, const std::string &)
    MPXJPP_GETTER(index, int)
    MPXJPP_GETTER(size, int)
};

class FontStyle final {
private:
    FontBase m_fontBase;
    Color m_color;
    Color m_backgroundColor;
    BackgroundPattern m_backgroundPattern;
    unsigned char m_italic:1;
    unsigned char m_bold:1;
    unsigned char m_underline:1;
    unsigned char m_strikethrough:1;
public:
    FontStyle(FontBase &&fontBase, bool italic, bool bold, bool underline,
              bool strikethrough, Color color, Color backgroundColor, BackgroundPattern backgroundPattern) :
        m_fontBase(std::move(fontBase)), m_color(color), m_backgroundColor(backgroundColor),
        m_backgroundPattern(backgroundPattern), m_italic(italic), m_bold(bold),
        m_underline(underline), m_strikethrough(strikethrough)
    {}

    MPXJPP_GETTER(fontBase, const FontBase &)
    MPXJPP_GETTER(color, Color)
    MPXJPP_GETTER(backgroundColor, Color)
    MPXJPP_GETTER(backgroundPattern, BackgroundPattern)
    MPXJPP_GETTER(italic, bool)
    MPXJPP_GETTER(bold, bool)
    MPXJPP_GETTER(underline, bool)
    MPXJPP_GETTER(strikethrough, bool)
};

}
}

#endif // FONT_H

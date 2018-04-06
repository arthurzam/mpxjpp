#ifndef PRIORITY_H
#define PRIORITY_H

namespace mpxjpp {

struct Priority final {
public:
    static constexpr int
        LOWEST = 100,
        VERY_LOW = 200,
        LOWER = 300,
        LOW = 400,
        MEDIUM = 500,
        HIGH = 600,
        HIGHER = 700,
        VERY_HIGH = 800,
        HIGHEST = 900,
        DO_NOT_LEVEL = 1000;
    using ANY_TYPE_CAST = int;
private:
    int m_value;
public:
    constexpr Priority(int p) :
        m_value((p < 0 || p > DO_NOT_LEVEL) ? MEDIUM : p)
        {}

    operator int() const {
        return m_value;
    }
};

}

#endif // PRIORITY_H

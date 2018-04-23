#ifndef BYTESTREAM_H
#define BYTESTREAM_H

#include <algorithm>
#include <array>
#include <cstring>
#include <ios>
#include <stdexcept>
#include <vector>

namespace bytestream {

class bytestream final {
public:
    using _vector = std::vector<unsigned char>;

    using value_type = _vector::value_type;
    using pointer = _vector::pointer;
    using const_pointer = _vector::const_pointer;
    using reference = _vector::reference;
    using const_reference = _vector::const_reference;
    using iterator = _vector::iterator;
    using const_iterator = _vector::const_iterator;
    using const_reverse_iterator = _vector::const_reverse_iterator;
    using reverse_iterator = _vector::reverse_iterator;
    using size_type = _vector::size_type;
    using difference_type = _vector::difference_type;
private:
    const_iterator m_start;
    const_iterator m_curr;
    const_iterator m_end;
    std::size_t m_gcount = 0;
public:
    bytestream() noexcept = delete;
    bytestream(const _vector &buffer) noexcept :
        m_start(buffer.cbegin()), m_curr(buffer.cbegin()), m_end(buffer.cend())
    { }


    bool eof() const noexcept {
        return m_curr >= m_end;
    }
    std::size_t gcount() const noexcept {
        return m_gcount;
    }
    difference_type tellg() const noexcept {
        return m_curr - m_start;
    }
    std::size_t available() const noexcept {
        return static_cast<std::size_t>(m_end - m_curr);
    }
    bytestream &seekg(std::streamoff pos) {
        if (pos > m_end - m_start || pos < 0)
            throw std::invalid_argument("seek out of range");
        m_curr = m_start + pos;
        m_gcount = 0;
        return *this;
    }
    bytestream &seekg(std::streamoff off, std::ios::seekdir dir) {
        auto pos = (dir == std::ios::beg ? m_start :
                    dir == std::ios::cur ? m_curr  :
                                           m_end);
        if (pos + off >= m_end || pos < m_start)
            throw std::invalid_argument("seek out of range");
        m_curr = pos + off;
        m_gcount = 0;
        return *this;
    }
    void unread() noexcept {
        m_curr -= static_cast<ssize_t>(m_gcount);
    }

    value_type peek() const {
        return *m_curr;
    }
    const_iterator pos() const noexcept {
        return m_curr;
    }
    const_iterator find(value_type x) const {
        return std::find(m_curr, m_end, x);
    }

    bytestream &read(void *buf, std::size_t size) {
        m_gcount = std::min(size, static_cast<std::size_t>(m_end - m_curr));
        std::memcpy(buf, &*m_curr, m_gcount);
        m_curr += static_cast<ssize_t>(m_gcount);
        return *this;
    }

    template<std::size_t N>
    bytestream &operator>> (std::array<value_type, N> &buf) {
        return read(buf.data(), N);
    }
    template<typename T>
    auto operator>> (T &val) -> std::enable_if_t<std::is_arithmetic<T>::value, bytestream &> {
        return read(&val, sizeof(T));
    }
};

}

#endif // BYTESTREAM_H

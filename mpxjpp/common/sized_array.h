#ifndef SIZED_ARRAY_H
#define SIZED_ARRAY_H

#include <stdexcept>
#include <type_traits>

namespace mpxjpp {
namespace common {

namespace details {

template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

}

template <typename T>
struct sized_array {
private:
    unsigned m_size;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_objs[1];

public:
    explicit sized_array(unsigned size) : m_size(size) {
        for (decltype(m_size) pos = 0; pos < m_size; ++pos)
            new (m_objs + pos) T();
    }

    template <typename U, class = details::void_t<decltype(std::declval<U>().size()),
                                                  decltype(std::declval<U>().begin()),
                                                  decltype(std::declval<U>().end())>>
    explicit sized_array(U &&container) {
        // There is good posibillity to create here a runtime error if not enough space was
        // allocated (for example with wrong call for operator new). If this struct fail, check
        // firstly the allocation.
        // A solution for this problem would be a type for m_size that isn't default initialized.
        m_size = container.size();

        unsigned pos = 0;
        for (auto iter = container.begin(), end = container.end(); iter != end; ++iter, ++pos)
            new (m_objs + pos) T(*iter);
    }

    sized_array &operator =(const sized_array &) = delete;
    sized_array &operator =(sized_array &&) = delete;

    ~sized_array() {
        for(decltype(m_size) pos = 0; pos < m_size; ++pos)
            reinterpret_cast<T &>(m_objs[pos]).~T();
    }

    size_t size() const { return m_size; }
    T *begin() { return reinterpret_cast<T *>(m_objs); }
    T *end() { return reinterpret_cast<T *>(m_objs + m_size); }
    const T *begin() const { return reinterpret_cast<T *>(m_objs); }
    const T *end() const { return reinterpret_cast<T *>(m_objs + m_size); }
    const T *cbegin() const { return reinterpret_cast<T *>(m_objs); }
    const T *cend() const { return reinterpret_cast<T *>(m_objs + m_size); }

    template<typename ...Args>
    void emplace(unsigned pos, Args&&... args) {
        reinterpret_cast<T &>(m_objs[pos]).~T();
        new (m_objs + pos) T(std::forward<Args>(args)...);
    }

    T &operator [](unsigned pos) {
        if (pos >= m_size)
            throw std::out_of_range("pos >= vector size");
        return reinterpret_cast<T &>(m_objs[pos]);
    }

    const T &operator [](unsigned pos) const {
        if (pos >= m_size)
            throw std::out_of_range("pos >= vector size");
        return reinterpret_cast<const T &>(m_objs[pos]);
    }

    static void *operator new(size_t, unsigned amount) {
        enum { base_size = std::max(alignof(decltype(m_objs[0])), alignof(decltype(m_size))) };
        char *memmory = new char[base_size + sizeof(m_objs[0]) * amount];
        sized_array *res = reinterpret_cast<sized_array *>(memmory);
        res->m_size = amount;
        return res;
    }

    template <typename U, class = details::void_t<decltype(std::declval<U>().size())>>
    static void *operator new(size_t s, const U &container) {
        return operator new(s, container.size());
    }

    static void operator delete(void *ptr) {
        delete[] static_cast<char *>(ptr);
    }
};

}
}
#endif // SIZED_ARRAY_H

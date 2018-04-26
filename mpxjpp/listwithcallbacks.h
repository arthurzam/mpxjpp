#ifndef LISTWITHCALLBACKS_H
#define LISTWITHCALLBACKS_H

#include <vector>

namespace mpxjpp {

template <typename T>
class ListWithCallbacks : private std::vector<T> {
protected:
    virtual void added(unsigned) { }
    virtual void removed(const T &) { }
    virtual void replaced(const T &, const T &) { }
public:
    using std::vector<T>::vector;
    using std::vector<T>::reserve;
    using std::vector<T>::resize;
    using std::vector<T>::clear;
    using std::vector<T>::shrink_to_fit;

    using std::vector<T>::size;
    using std::vector<T>::empty;
    using std::vector<T>::begin;
    using std::vector<T>::end;
    using std::vector<T>::cbegin;
    using std::vector<T>::cend;

    using std::vector<T>::at;
    using std::vector<T>::front;
    using std::vector<T>::back;
    using std::vector<T>::operator [];

    template <typename U>
    void add(U &&val) {
        this->push_back(std::forward<U>(val));
        added(size() - 1);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        this->emplace_back(std::forward<Args>(args)...);
        added(size() - 1);
    }

    void add(const T &val, size_t index) {
        this->insert(begin() + index, val);
        added(index);
    }

    void add(T &&val, size_t index) {
        this->insert(begin() + index, std::move(val));
        added(index);
    }

    T set(size_t index, const T &element) {
        T &ref(this->at(index));
        T removed(ref);
        ref = element;
        replaced(removed, element);
        return std::move(removed);
    }

    T remove(size_t index) {
        T obj(std::move(this->at(index)));
        removed(obj);
        this->erase(begin() + index);
        return std::move(obj);
    }
    auto remove(typename std::vector<T>::const_iterator iter) {
        removed(*iter);
        return this->erase(iter);
    }
};

}

#endif // LISTWITHCALLBACKS_H

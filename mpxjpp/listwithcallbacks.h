#ifndef LISTWITHCALLBACKS_H
#define LISTWITHCALLBACKS_H

#include <vector>

namespace mpxjpp {

template <typename T>
class ListWithCallbacks : private std::vector<T> {
protected:
	virtual void added(int) { }
	virtual void removed(const T &) { }
	virtual void replaced(const T &, const T &) { }
public:
	using ListWithCallbacks::vector::vector;
	using ListWithCallbacks::vector::reserve;
	using ListWithCallbacks::vector::resize;
	using ListWithCallbacks::vector::clear;

	using ListWithCallbacks::vector::size;
	using ListWithCallbacks::vector::empty;
	using ListWithCallbacks::vector::begin;
	using ListWithCallbacks::vector::end;
	using ListWithCallbacks::vector::cbegin;
	using ListWithCallbacks::vector::cend;

	using ListWithCallbacks::vector::at;
	using ListWithCallbacks::vector::front;
	using ListWithCallbacks::vector::back;
	using ListWithCallbacks::vector::operator [];

	void add(const T &val) {
		this->push_back(val);
		added(size() - 1);
	}

	void add(T &&val) {
		this->push_back(std::move(val));
		added(size() - 1);
	}

	void add(const T &val, int index) {
		this->insert(begin() + index, val);
		added(index);
	}

	void add(T &&val, int index) {
		this->insert(begin() + index, std::move(val));
		added(index);
	}

	T&& set(int index, const T &element) {
		T &ref(this->at(index));
		T removed(ref);
		ref = element;
		replaced(removed, element);
		return std::move(removed);
	}

	T&& remove(int index) {
		T obj(this->at(index));
		this->erase(begin() + index);
		removed(obj);
		return std::move(obj);
	}
};

}

#endif // LISTWITHCALLBACKS_H

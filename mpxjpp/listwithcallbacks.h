#ifndef LISTWITHCALLBACKS_H
#define LISTWITHCALLBACKS_H

#include <vector>

namespace mpxjpp {

template <typename T>
class ListWithCallbacks {
	protected:
		std::vector<T> m_list;
		virtual void added(int) { }
		virtual void removed(const T &) { }
		virtual void replaced(const T &, const T &) { }
	public:
		ListWithCallbacks()
			: m_list()
		{}

		void reserve(size_t n) {
			m_list.reserve(n);
		}

		void resize(size_t n) {
			m_list.resize(n);
		}

		void add(const T &val) {
			m_list.push_back(val);
			added(m_list.size() - 1);
		}

		void add(T &&val) {
			m_list.push_back(std::move(val));
			added(m_list.back());
		}

		void add(const T &val, int index) {
			m_list.insert(m_list.begin() + index, val);
			added(val);
		}

		void add(T &&val, int index) {
			m_list.insert(m_list.begin() + index, std::move(val));
			added(*(m_list.begin() + index));
		}

		const T &operator [](int index) const {
			return m_list[index];
		}

		void clear() {
			m_list.clear();
		}

		T&& set(int index, const T &element) {
			T &ref = m_list[index];
			T removed(ref);
			ref = element;
			replaced(removed, element);
			return std::move(removed);
		}

		T&& remove(int index) {
			T removed = m_list[index];
			m_list.erase(m_list.begin() + index);
			return std::move(removed);
		}

		bool empty() const {
			return m_list.empty();
		}

		auto begin() {
			return m_list.begin();
		}

		auto end() {
			return m_list.end();
		}

		auto cbegin() const {
			return m_list.cbegin();
		}

		auto cend() const {
			return m_list.cend();
		}

		auto begin() const {
			return m_list.cbegin();
		}

		auto end() const {
			return m_list.cend();
		}

		// TODO: work on it
};

}

#endif // LISTWITHCALLBACKS_H

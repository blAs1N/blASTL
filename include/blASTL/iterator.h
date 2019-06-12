#pragma once

#include <memory>
#include <iterator>

// This template specialization is built in std namespace for compatibility with real standards
namespace std {

	template <class T>
	struct iterator_traits<const T*>
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
	};

}

namespace blASTL {

	template <class Category, class T, class Distance = std::ptrdiff_t,
		class Pointer = T*, class Reference = T&>
	struct iterator {
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};

	template <class Iter>
	class reverse_iterator final {
	public:
		//Member Types
		using value_type = typename std::iterator_traits<Iter>::value_type;
		using difference_type = typename std::iterator_traits<Iter>::difference_type;
		using pointer = typename std::iterator_traits<Iter>::pointer;
		using reference = typename std::iterator_traits<Iter>::reference;
		using iterator_category = typename std::iterator_traits<Iter>::iterator_category;

		// Constructor and Destructor
		constexpr reverse_iterator()
			: current() {}

		constexpr explicit reverse_iterator(Iter x)
			: current(x) {}

		template <class U>
		constexpr reverse_iterator(const reverse_iterator<U>& other)
			: current(other.base()) {}

		// Member Operator
		template <class U>
		constexpr reverse_iterator& operator=(const reverse_iterator<U>& other) {
			current = other.base();
			return *this;
		}

		constexpr reference operator*() const {
			auto tmp = current;
			return *(--tmp);
		}

		constexpr pointer operator->() const {
			return std::addressof(operator*());
		}

		constexpr reference operator[](difference_type n) const {
			current[-n - 1];
		}

		constexpr reverse_iterator& operator++() {
			--current;
			return *this;
		}

		constexpr reverse_iterator& operator--() {
			++current;
			return *this;
		}

		constexpr reverse_iterator operator++(int) {
			auto it = *this;
			current--;
			return it;
		}

		constexpr reverse_iterator operator--(int) {
			auto it = *this;
			current++;
			return it;
		}

		constexpr reverse_iterator operator+(difference_type n) const {
			auto it = *this;
			it.current - n;
			return it;
		}

		constexpr reverse_iterator operator-(difference_type n) const {
			auto it = *this;
			it.current + n;
			return it;
		}

		constexpr reverse_iterator& operator+=(difference_type n) {
			current -= n;
			return *this;
		}

		constexpr reverse_iterator& operator-=(difference_type n) {
			current += n;
			return *this;
		}

		// Member Function
		constexpr Iter base() const {
			return current;
		}

	private:
		// Member Object
		Iter current;
	};

	// Non-Member Operator
	template <class Iterator1, class Iterator2>
	constexpr bool operator==(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() == rhs.base();
	}

	template <class Iterator1, class Iterator2>
	constexpr bool operator!=(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() != rhs.base();
	}

	template <class Iterator1, class Iterator2>
	constexpr bool operator<(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() > rhs.base();
	}

	template <class Iterator1, class Iterator2>
	constexpr bool operator<=(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() >= rhs.base();
	}

	template <class Iterator1, class Iterator2>
	constexpr bool operator>(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() < rhs.base();
	}

	template <class Iterator1, class Iterator2>
	constexpr bool operator>=(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() <= rhs.base();
	}

	template <class Iter>
	constexpr reverse_iterator<Iter>
		operator+(typename reverse_iterator<Iter>::difference_type n,
			const reverse_iterator<Iter>& it) {

		return reverse_iterator<Iter>(it.base() - n);
	}

	template <class Iter>
	constexpr decltype(auto) operator+(const reverse_iterator<Iter>& it,
			typename reverse_iterator<Iter>::difference_type n) {

		return reverse_iterator<Iter>(it.base() - n);
	}

	template <class Iterator1, class Iterator2>
	constexpr decltype(auto) operator-(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return (rhs.base() - lhs.base());
	}

	// Non-Member Function
	template <class Iter>
	constexpr decltype(auto) make_reverse_iterator(Iter i) {
		return std::reverse_iterator<Iter>(i);
	}

}
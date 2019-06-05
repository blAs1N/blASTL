#pragma once

#include <iterator>

namespace blASTL {

	template <class Iter>
	class reverse_iterator {
	public:
		//Member Types
		using value_type = typename std::iterator_traits<Iter>::value_type;
		using difference_type = typename std::iterator_traits<Iter>::difference_type;
		using pointer = typename std::iterator_traits<Iter>::pointer;
		using reference = typename std::iterator_traits<Iter>::reference;
		using iterator_category = typename std::iterator_traits<Iter>::iterator_category;
		using iterator_type = Iter;

		// Constructor and Destructor
		constexpr reverse_iterator()
			: current() {}

		constexpr explicit reverse_iterator(iterator_type x)
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
			iterator_type tmp = current;
			return *(--tmp);
		}

		constexpr pointer operator->() const {
			return std::addressof(operator*());
		}

		constexpr operator[](difference_type n) const {
			base()[-n - 1];
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
		constexpr iterator_type base() const {
			return current;
		}

	protected:
		// Member Object
		iterator_type current;
	};

	// Non-Member Operator
	template< class Iterator1, class Iterator2 >
	constexpr bool operator==(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() == rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	constexpr bool operator!=(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() != rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	constexpr bool operator<(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() > rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	constexpr bool operator<=(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() >= rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	constexpr bool operator>(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() < rhs.base();
	}

	template< class Iterator1, class Iterator2 >
	constexpr bool operator>=(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs) {

		return lhs.base() <= rhs.base();
	}

	template< class Iter >
	constexpr reverse_iterator<Iter>
		operator+(typename reverse_iterator<Iter>::difference_type n,
			const reverse_iterator<Iter>& it) {

		return reverse_iterator<Iter>(it.base() - n);
	}

	template< class Iter >
	constexpr reverse_iterator<Iter>
		operator+(const reverse_iterator<Iter>& it,
			typename reverse_iterator<Iter>::difference_type n) {

		return reverse_iterator<Iter>(it.base() - n);
	}

	template< class Iterator1, class Iterator2 >
	constexpr auto operator-(const reverse_iterator<Iterator1>& lhs,
		const reverse_iterator<Iterator2>& rhs
		) -> decltype(rhs.base() - lhs.base()) {

		return (rhs.base() - lhs.base());
	}

	// Non-Member Function
	template <class Iter>
	constexpr std::reverse_iterator<Iter> make_reverse_iterator(Iter i) {
		return std::reverse_iterator<Iter>(i);
	}

}
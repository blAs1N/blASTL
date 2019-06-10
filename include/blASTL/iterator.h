#pragma once

#include <memory>
#include <iterator>

namespace blASTL {

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	class bidirectional_iterator {
	public:
		//Member Operator
		constexpr reference operator*() {
			return *ptr;
		}

		auto operator++() -> decltype(*this)& {
			ptr++;
			return *this;
		}

		auto operator--() -> decltype(*this)& {
			ptr--;
			return *this;
		}
		
		auto operator++(int) -> decltype(*this)& {
			auto& tmp = *this;
			ptr++;
			return tmp;
		}

		auto operator--(int) -> decltype(*this)& {
			auto& tmp = *this;
			ptr--;
			return tmp;
		}

	protected:
		// Member Object
		pointer ptr;
	};

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	class random_access_iterator : public bidirectional_iterator<iterator_category, value_type, difference_type, pointer, reference> {
	public:
		//Member Operator
		auto operator+=(difference_type n) -> decltype(*this)& {
			if (n >= 0) while (n--) ++ptr;
			else while (n++) --ptr;
			return *this;
		}

		auto operator-=(difference_type n) -> decltype(*this)& {
			return (*this -= n);
		}
	
		constexpr reference operator[](size_t n) {
			return *(ptr + n)
		}

	};

	// Non-Member Operator
	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr auto operator+(random_access_iterator<iterator_category, value_type, difference_type, pointer, reference> a, difference_type n) -> decltype(a) {
		auto tmp = a;
		tmp += n;
		return tmp;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr auto operator+(difference_type n, random_access_iterator<iterator_category, value_type, difference_type, pointer, reference> a) -> decltype(a) {
		return a + n;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr auto operator-(random_access_iterator<iterator_category, value_type, difference_type, pointer, reference> a, difference_type n) -> decltype(a) {
		auto tmp = a;
		tmp -= n;
		return tmp;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr auto operator-(difference_type n, random_access_iterator<iterator_category, value_type, difference_type, pointer, reference> a) -> decltype(a) {
		return a - n;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr difference_type operator-(random_access_iterator<iterator_category, value_type, difference_type, pointer, reference> lhs,
		random_access_iterator<iterator_category, value_type, difference_type, pointer, reference> rhs) {

		return std::distance(lhs, rhs);
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr bool operator<(const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& lhs,
		const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& rhs) {
		return lhs.ptr < rhs.ptr;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr bool operator<=(const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& lhs,
		const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& rhs) {
		return lhs.ptr <= rhs.ptr;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr bool operator>(const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& lhs,
		const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& rhs) {
		return lhs.ptr > rhs.ptr;
	}

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	constexpr bool operator>=(const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& lhs,
		const random_access_iterator<iterator_category, value_type, difference_type, pointer, reference>& rhs) {
		return lhs.ptr >= rhs.ptr;
	}

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
	constexpr reverse_iterator<Iter>
		operator+(const reverse_iterator<Iter>& it,
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
	constexpr std::reverse_iterator<Iter> make_reverse_iterator(Iter i) {
		return std::reverse_iterator<Iter>(i);
	}

}
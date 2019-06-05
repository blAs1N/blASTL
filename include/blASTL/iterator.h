#pragma once

#include <memory>

namespace blASTL {

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	class forward_iterator {
	public:
		// Member Operator
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

	protected:
		// Member Object
		pointer ptr;
	};

	template <class iterator_category, class value_type, class difference_type, class pointer, class reference>
	class bidirectional_iterator : public forward_iterator<iterator_category, value_type, difference_type, pointer, reference> {

	public:
		//Member Operator
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

		auto operator-=(difference_type n) -> decltype(*this) & {
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

		return static_cast<difference_type>((lhs.ptr - rhs.ptr) / sizeof(*lhs.ptr));
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

}
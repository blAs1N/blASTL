#pragma once

#include <initializer_list>
#include "iterator.h"

namespace blASTL {

	template <class T, class Allocator = std::allocator<T>>
	class vector {
	public:
		// Member types
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = std::allocator_traits<Allocator>::pointer;
		using const_pointer = std::allocator_traits<Allocator>::const_pointer;
		using iterator = random_access_iterator<std::random_access_iterator_tag, value_type, difference_type, pointer, reference>;
		using const_iterator = random_access_iterator<std::random_access_iterator_tag, value_type, difference_type, const_pointer, const_reference>;
		using reverse_iterator = reverse_iterator<iterator>;
		using const_reverse_iterator = reverse_iterator<const_iterator>;
	
	public:
		// Constructor and Destructor
		vector() noexcept(noexcept(Allocator())) = default;

		explicit vector(const Allocator& alloc) noexcept
			: begin(), end(), last, allocator(alloc) {}

		vector(size_type count, const T& value, const Allocator& alloc = Allocator())
			: allocator(alloc), begin(allocator.allocate(count)), end(begin + count), last(begin + count) {

			std::fill(begin, last, value);
		}

		explicit vector(size_type count, const Allocator& alloc = Allocator())
			: vector(count, T(), alloc) {}

		template <class InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			: allocator(alloc) {
			
			auto size = last - first;

			begin = allocator.allocate(size);
			end = this->last = begin + size;

			memmove_s(std::addressof(*begin), size, std::addressof(*first), size);
		}

		vector(const vector& other)
			: vector(other.begin, other.end, other.allocator) {}

		vector(const vector& other, const Allocator& alloc)
			: vector(other.begin, other.end, alloc) {}

		vector(vector&& other) noexcept
			: allocator(std::move(other.allocator)), begin(std::move(other.begin)),
			end(std::move(other.end)), last(end) {}

		vector(vector&& other, const Allocator& alloc)
			: allocator(alloc), begin(std::move(other.begin)),
			end(std::move(other.end)), last(std::move(other.last)) {}

		vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
			: allocator(alloc), begin(allocator.allocate(init.size())), end(begin + init.size()), last(begin + init.size()) {

			memmove_s(std::addressof(*begin), init.size(), std::addressof(*init.begin()), init.size);
		}
	
		~vector() {
			clear();
		}

		// Member Function
		vector& operator=(const vector& other) {
			clear();
			
			allocator = other.allocator;
			begin = allocator.allocate(other.size());
			end = last = begin + other.size();
			memmove_s(std::addressof(*begin), other.size(), std::addressof(*other.begin), other.size());
		}

		vector& operator=(vector&& other) noexcept {
			clear();

			allocator = std::move(other.allocator);
			begin = std::move(other.begin);
			end = last = std::move(other.end);
		}

		vector& operator=(std::initializer_list<T> ilist) {
			clear();
			
			begin = allocator.allocate(ilist.size());
			end = last = begin + ilist.size();
			memmove_s(std::addressof(*begin), ilist.size(), std::addressof(*ilist.begin()), ilist.size());
		}
		
		void assign(size_type count, const T& value) {
			clear();

			begin = allocator.allocate(count);
			end = this->last = begin + count;

			std::fill(begin, last, value);
		}

		template <class InputIt>
		void assign(InputIt first, InputIt last) {
			clear();

			auto size = last - first;
			
			begin = allocator.allocate(size);
			end = this->last = begin + size;

			memmove_s(std::addressof(*begin), size, std::addressof(*first), size);
		}
		
		void assign(std::initializer_list<T> ilist) {
			assign(ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const {
			return allocator;
		}

		// Element Access
		reference at(size_type pos);
		const_reference at(size_type pos) const;

		reference operator[](size_type pos);
		const_reference operator[](size_type pos) const;

		reference front();
		const_reference front() const;

		reference back();
		const_reference back() const;

		T* data() noexcept;
		const T* data() const noexcept;

		//Iterators
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		iterator end() noexcept;
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;

		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator crbegin() const noexcept;

		iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		const_reverse_iterator crend() const noexcept;

		// Capacity
		bool empty() const noexcept;
		size_type size() const noexcept;
		size_type max_size() const noexcept;
		void reserve(size_type new_cap);
		size_type capacity() const noexcept;
		void shrink_to_fit();
		
		// Modifiers
		void clear() noexcept;
		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);

		template <class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last);

		iterator insert(const_iterator pos, std::initializer_list<T> ilist);
	
		template <class... Args>
		iterator emplace(const_iterator pos, Args&& args);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
	
		void push_back(const T& value);
		void push_back(T&& value);

		template <class... Args>
		reference emplace_back(Args&&... args);

		void pop_back();
		
		void resize(size_type count);
		void resize(size_type count, const value_type& value);
		
		void swap(vector& other) noexcept;

	private:
		void clear() {
			for (size_type i = 0; i < size_; ++i)
				std::allocator_traits<Allocator>::destroy(allocator, std::addressof(*(begin + i)));
		}

	private:
		iterator begin;
		iterator end;
		iterator last;
		Allocator allocator
	};

	// Non-member functions
	template <class T, class Alloc>
	bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

	template <class T, class Alloc>
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

	template <class T, class Alloc>
	bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

	template <class T, class Alloc>
	bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

	template <class T, class Alloc>
	bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

	template <class T, class Alloc>
	bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

	template <class T, class Alloc>
	void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs);

	template <class T, class Alloc, class U>
	void erase(vector<T, Alloc>& c, const U& value);

	template <class T, class Alloc, class Pred>
	void erase_if(vector<T, Alloc>& c, Pred pred);

}
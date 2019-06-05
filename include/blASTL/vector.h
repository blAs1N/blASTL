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
		vector() noexcept(noexcept(Allocator()));
		explicit vector(const Allocator& alloc) noexcept;
		vector(size_type count, const T& value, const Allocator& alloc = Allocator());
		explicit vector(size_type count, const Allocator& alloc = Allocator());

		template <class InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());

		vector(const vector& other);
		vector(const vector& other, const Allocator& alloc);

		vector(vector&& other) noexcept;
		vector(vector&& other, const Allocator& alloc);

		vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());
	
		~vector();

		// Member Function
		vector& operator=(const vector& other);
		vector& operator=(vector&& other) noexcept;
		vector& operator=(std::initializer_list<T> ilist);
		
		void assign(size_type count, const T& value);

		template <class InputIt>
		void assign(InputIt first, InputIt last);
		
		void assign(std::initializer_list<T> ilist);

		allocator_type get_allocator() const;

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
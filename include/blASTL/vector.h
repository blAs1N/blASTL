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
		using pointer = std::allocator_traits<allocator_type>::pointer;
		using const_pointer = std::allocator_traits<allocator_type>::const_pointer;
		using iterator = random_access_iterator<std::random_access_iterator_tag, value_type, difference_type, pointer, reference>;
		using const_iterator = random_access_iterator<std::random_access_iterator_tag, value_type, difference_type, const_pointer, const_reference>;
		using reverse_iterator = reverse_iterator<iterator>;
		using const_reverse_iterator = reverse_iterator<const_iterator>;
	
	public:
		// Constructor and Destructor
		vector() noexcept(noexcept(Allocator())) = default;

		explicit vector(const Allocator& alloc) noexcept
			: mBegin(), mEnd(), mLast, allocator(alloc) {}

		vector(size_type count, const T& value, const Allocator& alloc = Allocator())
			: allocator(alloc), mBegin(allocator.allocate(count)), mEnd(mBegin + count), mLast(mBegin + count) {

			std::fill(mBegin, mLast, value);
		}

		explicit vector(size_type count, const Allocator& alloc = Allocator())
			: vector(count, T(), alloc) {}

		template <class InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			: allocator(alloc) {
			
			auto size = last - first;

			mBegin = allocator.allocate(size);
			mEnd = this->mLast = mBegin + size;

			memmove_s(std::addressof(*mBegin), size * sizeof(value_type), std::addressof(*first), size * sizeof(value_type));
		}

		vector(const vector& other)
			: vector(other.mBegin, other.mEnd, other.allocator) {}

		vector(const vector& other, const Allocator& alloc)
			: vector(other.mBegin, other.mEnd, alloc) {}

		vector(vector&& other) noexcept
			: allocator(std::move(other.allocator)), mBegin(std::move(other.mBegin)),
			mEnd(std::move(other.mEnd)), mLast(mEnd) {}

		vector(vector&& other, const Allocator& alloc)
			: allocator(alloc), mBegin(std::move(other.mBegin)),
			mEnd(std::move(other.mEnd)), mLast(std::move(other.mLast)) {}

		vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
			: allocator(alloc), mBegin(allocator.allocate(init.size())), mEnd(mBegin + init.size()), mLast(mBegin + init.size()) {

			memmove_s(std::addressof(*mBegin),
				init.size() * sizeof(value_type),
				std::addressof(*init.begin()),
				init.size * sizeof(value_type));
		}
	
		~vector() {
			clear();
		}

		// Member Function
		vector& operator=(const vector& other) {
			clear();
			
			allocator = other.allocator;
			mBegin = allocator.allocate(other.size());
			mEnd = mLast = mBegin + other.size();

			memmove_s(std::addressof(*mBegin),
				other.size() * sizeof(value_type),
				std::addressof(*other.mBegin),
				other.size() * sizeof(value_type));
		}

		vector& operator=(vector&& other) noexcept {
			clear();

			allocator = std::move(other.allocator);
			mBegin = std::move(other.mBegin);
			mEnd = mLast = std::move(other.mEnd);
		}

		vector& operator=(std::initializer_list<T> ilist) {
			clear();
			
			mBegin = allocator.allocate(ilist.size());
			mEnd = mLast = mBegin + ilist.size();

			memmove_s(std::addressof(*mBegin),
				ilist.size(),
				std::addressof(*ilist.begin()),
				ilist.size());
		}
		
		void assign(size_type count, const T& value) {
			clear();

			mBegin = allocator.allocate(count);
			mEnd = this->mLast = mBegin + count;

			std::fill(mBegin, mLast, value);
		}

		template <class InputIt>
		void assign(InputIt first, InputIt last) {
			clear();

			auto size = last - first;
			
			mBegin = allocator.allocate(size);
			mEnd = this->mLast = mBegin + size;

			memmove_s(std::addressof(*mBegin),
				size * sizeof(value_type),
				std::addressof(*first),
				size * sizeof(value_type));
		}
		
		void assign(std::initializer_list<T> ilist) {
			assign(ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const {
			return allocator;
		}

		// Element Access
		reference at(size_type pos) {
			if (pos >= size())
				throw std::out_of_range("vector::at - out of range");

			return mBegin[pos];
		}

		const_reference at(size_type pos) const {
			if (pos >= size())
				throw std::out_of_range("vector::at - out of range");

			return mBegin[pos];
		}

		reference operator[](size_type pos) {
			return mBegin[pos];
		}

		const_reference operator[](size_type pos) const {
			return mBegin[pos];
		}

		reference front() {
			return *mBegin;
		}

		const_reference front() const {
			return *mBegin;
		}

		reference back() {
			return *mEnd;
		}

		const_reference back() const {
			return *mEnd;
		}

		T* data() noexcept {
			return std::addressof(*mBegin);
		}

		const T* data() const noexcept {
			return std::addressof(*mBegin);
		}

		//Iterators
		iterator begin() noexcept {
			return mBegin;
		}

		const_iterator begin() const noexcept {
			return mBegin;
		}

		const_iterator cbegin() const noexcept {
			return mBegin;
		}

		iterator end() noexcept {
			return mEnd;
		}

		const_iterator end() const noexcept {
			return mEnd;
		}

		const_iterator cend() const noexcept {
			return mEnd;
		}

		reverse_iterator rbegin() noexcept {
			return reverse_iterator(mEnd);
		}

		const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(mEnd);
		}

		const_reverse_iterator crbegin() const noexcept {
			return const_reverse_iterator(mEnd);
		}

		iterator rend() noexcept {
			return reverse_iterator(mEnd);
		}

		const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(mEnd);
		}

		const_reverse_iterator crend() const noexcept {
			return const_reverse_iterator(mEnd);
		}

		// Capacity
		bool empty() const noexcept {
			return size() <= 0;
		}

		size_type size() const noexcept {
			return mEnd - mBegin;
		}

		size_type max_size() const noexcept {
			return __min(std::allocator_traits<allocator_type>::max_size(allocator),
				std::numeric_limits<difference_type>::max());
		}

		void reserve(size_type new_cap) {
			auto size = size();

			if (size + new_cap >= max_size())
				throw std::length_error("vector::reserve - new_cap is greater than max_size");

			if (new_cap <= capacity()) return;

			auto newBegin = allocator.allocate(new_cap);

			memmove_s(std::addressof(*newBegin),
				new_cap * sizeof(value_type),
				std::addressof(*mBegin),
				size * sizeof(value_type));

			allocator.deallocate(mBegin, capacity());

			mBegin = newBegin;
			mEnd = mBegin + size;
			mLast = mBegin + new_cap;
		}

		size_type capacity() const noexcept {
			return mLast - mBegin;
		}

		void shrink_to_fit();
		
		// Modifiers
		void clear() noexcept {
			for (size_type i = 0; i < size_; ++i)
				std::allocator_traits<allocator_type>::destroy(allocator, std::addressof(*(mBegin + i)));
		}

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
		iterator mBegin;
		iterator mEnd;
		iterator mLast;
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
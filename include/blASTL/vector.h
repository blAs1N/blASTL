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
			: mBegin(), mEnd(), mLast, mAllocator(alloc) {}

		vector(size_type count, const T& value, const Allocator& alloc = Allocator())
			: mAllocator(alloc), mBegin(mAllocator.allocate(count)), mEnd(mBegin + count), mLast(mBegin + count) {

			std::fill(mBegin, mLast, value);
		}

		explicit vector(size_type count, const Allocator& alloc = Allocator())
			: vector(count, T(), alloc) {}

		template <class InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			: mAllocator(alloc) {
			
			mBegin = mAllocator.allocate(last - first);
			mEnd = mLast = std::copy(first, last, mBegin);
		}

		vector(const vector& other)
			: vector(other.mBegin, other.mEnd, other.mAllocator) {}

		vector(const vector& other, const Allocator& alloc)
			: vector(other.mBegin, other.mEnd, alloc) {}

		vector(vector&& other) noexcept
			: mAllocator(std::move(other.mAllocator)),
			mBegin(std::move(other.mBegin)),
			mEnd(std::move(other.mEnd)),
			mLast(mEnd) {}

		vector(vector&& other, const Allocator& alloc)
			: mAllocator(alloc),
			mBegin(std::move(other.mBegin)),
			mEnd(std::move(other.mEnd)),
			mLast(mEnd) {}

		vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
			: mAllocator(alloc),
			mBegin(mAllocator.allocate(init.size())),
			mEnd(std::copy(init.begin(), init.end(), mBegin)),
			mLast(mEnd) {}
	
		~vector() {
			clear();
		}

		// Member Function
		vector& operator=(const vector& other) {
			clear();
			
			mAllocator = other.mAllocator;
			mBegin = mAllocator.allocate(other.size());

			mEnd = mLast = std::copy(
				other.mBegin,
				other.mEnd,
				mBegin
			);
		}

		vector& operator=(vector&& other) noexcept {
			clear();

			mAllocator = std::move(other.mAllocator);
			mBegin = std::move(other.mBegin);
			mEnd = mLast = std::move(other.mEnd);
		}

		vector& operator=(std::initializer_list<T> ilist) {
			clear();
			
			mBegin = mAllocator.allocate(ilist.size());
			mEnd = mLast = std::copy(
				ilist.begin(),
				ilist.end(),
				mBegin
			);
		}
		
		void assign(size_type count, const T& value) {
			clear();

			mBegin = mAllocator.allocate(count);
			mEnd = mLast = mBegin + count;

			std::fill(mBegin, mLast, value);
		}

		template <class InputIt>
		void assign(InputIt first, InputIt last) {
			clear();

			mBegin = mAllocator.allocate(last - first);
			mEnd = mLast = std::copy(first, last, mBegin);
		}
		
		void assign(std::initializer_list<T> ilist) {
			assign(ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const {
			return mAllocator;
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
			return __min(std::numeric_limits<difference_type>::max(),
				std::allocator_traits<allocator_type>::max_size(mAllocator));
		}

		void reserve(size_type new_cap) {
			if (new_cap >= max_size())
				throw std::length_error("vector::reserve - new_cap is greater than max_size");

			if (new_cap <= capacity()) return;

			auto newBegin = mAllocator.allocate(new_cap);
			auto newEnd = std::copy(mBegin, mEnd, newBegin);

			mAllocator.deallocate(mBegin, capacity());

			mBegin = newBegin;
			mEnd = newEnd;
			mLast = mBegin + new_cap;
		}

		size_type capacity() const noexcept {
			return mLast - mBegin;
		}

		void shrink_to_fit() {
			if (size() == capacity()) return;

			if (empty()) {
				mAllocator.deallocate(mBegin, capacity());
				mEnd = mLast = mBegin;
			}

			else {
				auto tmp = vector<T, Allocator>(mBegin, mEnd, mAllocator);
				swap(tmp);

				mAllocator.deallocate(tmp.mBegin, capacity());
			}
		}
		
		// Modifiers
		void clear() noexcept {
			for (size_type i = 0; i < size_; ++i)
				std::allocator_traits<allocator_type>::destroy(mAllocator, std::addressof(*(mBegin + i)));
		}

		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);

		template <class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last);

		iterator insert(const_iterator pos, std::initializer_list<T> ilist);
	
		template <class... Args>
		iterator emplace(const_iterator pos, Args&& args) {
			auto new_elem = rightShift(pos - mBegin, 1);

			std::allocator_traits<Allocator>::construct(
				allocator,
				std::addressof(*new_elem),
				std::forward<Args>(args)...);

			return new_elem;
		}

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
	
		void push_back(const T& value) {
			emplace_back(value);
		}

		void push_back(T&& value) {
			emplace_back(std::move(value));
		}

		template <class... Args>
		reference emplace_back(Args&& ... args) {
			emplace(end(), std::forward<Args>(args)...);
		}

		void pop_back() {
			erase(mEnd - 1);
		}
		
		void resize(size_type count) {
			resize(count, value_type());
		}

		void resize(size_type count, const value_type& value) {
			if (size() < count)
				insert(mEnd, count - size(), value);

			else if (size() > count)
				erase(mBegin + count, mEnd);
		}
		
		void swap(vector& other) noexcept {
			std::swap(mBegin, other.mBegin);
			std::swap(mEnd, other.mEnd);
			std::swap(mLast, other.mLast);
			std::swap(mAllocator, other.mAllocator);
		}

	private:
		iterator rightShift(size_type pos, size_type count) {
			if (count == 0 || empty()) return mBegin;

			if (capacity() == 0) {
				mBegin = allocator.allocate(count);
				mEnd = mLast = mBegin + count;
				return mBegin;
			}

			if (size() + count > capacity())
				reserve(capacity() * 2);
			
			return shift(pos, count);
		}

		iterator leftShift(size_type pos, size_type count) {
			if (count == 0 || empty()) return mBegin;

			return shift(pos, -count);
		}

		iterator shift(size_type pos, size_type count) {
			auto tmpBegin = vector<T, Allocator>(mEnd - (mBegin + pos));
			auto tmpEnd = std::copy(mBegin + pos, mEnd, tmp);

			mEnd = std::copy(tmpBegin, tmpEnd, mBegin + pos + count);
			return mBegin + pos;
		}

	private:
		iterator mBegin;
		iterator mEnd;
		iterator mLast;
		Allocator mAllocator;
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
	void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
		std::swap(lhs.mBegin, rhs.mBegin);
		std::swap(lhs.mEnd, rhs.mEnd);
		std::swap(lhs.mLast, rhs.mLast);
		std::swap(lhs.mAllocator, rhs.mAllocator);
	}

	template <class T, class Alloc, class U>
	void erase(vector<T, Alloc>& c, const U& value);

	template <class T, class Alloc, class Pred>
	void erase_if(vector<T, Alloc>& c, Pred pred);

}
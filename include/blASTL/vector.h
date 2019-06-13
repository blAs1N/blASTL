#pragma once

#include <initializer_list>
#include <algorithm>
#include "iterator.h"

namespace blASTL {

	template <class T, class Allocator = std::allocator<T>>
	class vector final {
	public:
		// Member types
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = value_type const&;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using iterator = T*;
		using const_iterator = T const*;
		using reverse_iterator = blASTL::reverse_iterator<iterator>;
		using const_reverse_iterator = blASTL::reverse_iterator<const_iterator>;
	
	public:
		// Constructor and Destructor
		vector() noexcept(noexcept(Allocator()))
			: mBegin(nullptr), mEnd(nullptr), mLast(nullptr), mAllocator(Allocator()) {}

		explicit vector(Allocator const& alloc) noexcept
			: mBegin(nullptr), mEnd(nullptr), mLast(nullptr), mAllocator(alloc) {}

		vector(size_type count, T const& value, Allocator const& alloc = Allocator())
			: mAllocator(alloc), mBegin(mAllocator.allocate(count)), mEnd(mBegin), mLast(mBegin + count) {

			fill(mBegin, mLast, value);
			mEnd = mLast;
		}

		explicit vector(size_type count, Allocator const& alloc = Allocator())
			: vector(count, T(), alloc) {}

		template <class InputIt, typename = std::enable_if_t<
			std::is_same_v<std::iterator_traits<InputIt>::iterator_category, std::random_access_iterator_tag>>>
		vector(InputIt first, InputIt last, Allocator const& alloc = Allocator())
			: mAllocator(alloc) {
			
			mBegin = mEnd = mAllocator.allocate(last - first);
			mLast = mBegin + (last - first);

			for (; first != last; first++)
				std::allocator_traits<Allocator>::construct(mAllocator, mEnd++, *first);
		}

		vector(vector const& other)
			: vector(other.mBegin, other.mEnd, other.mAllocator) {}

		vector(vector const& other, Allocator const& alloc)
			: vector(other.mBegin, other.mEnd, alloc) {}

		vector(vector&& other) noexcept
			: mAllocator(std::move(other.mAllocator)),
			mBegin(std::move(other.mBegin)),
			mEnd(std::move(other.mEnd)),
			mLast(mEnd) {

			other.mBegin = other.mEnd = other.mLast = nullptr;
		}

		vector(vector&& other, Allocator const& alloc)
			: mAllocator(alloc),
			mBegin(std::move(other.mBegin)),
			mEnd(std::move(other.mEnd)),
			mLast(mEnd) {
		
			other.mBegin = other.mEnd = other.mLast = nullptr;
		}

		vector(std::initializer_list<T> init, Allocator const& alloc = Allocator())
			: vector(init.begin(), init.end(), alloc) {}
	
		~vector() {
			reset();
		}

		// Member Function
		vector& operator=(vector const& other) {
			assign(other.mBegin, other.mEnd);
			return *this;
		}

		vector& operator=(vector&& other) noexcept {
			reset();

			mAllocator = std::move(other.mAllocator);
			mBegin = std::move(other.mBegin);
			mEnd = mLast = std::move(other.mEnd);

			other.mBegin = other.mEnd = other.mLast = nullptr;
			return *this;
		}

		vector& operator=(std::initializer_list<T> ilist) {
			assign(ilist.begin(), ilist.end());
			return *this;
		}
		
		void assign(size_type count, T const& value) {
			reset();

			mBegin = mAllocator.allocate(count);
			mLast = mBegin + count;

			fill(mBegin, mLast, value);
			mEnd = mLast;
		}

		template <class InputIt, typename = std::enable_if_t<
			std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
		void assign(InputIt first, InputIt last) {
			reset();

			mBegin = mEnd = mAllocator.allocate(last - first);
			mLast = mBegin + (last - first);

			for (; first != last; first++)
				std::allocator_traits<Allocator>::construct(mAllocator, mEnd++, *first);
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
			return *(mEnd - 1);
		}

		const_reference back() const {
			return *(mEnd - 1);
		}

		T* data() noexcept {
			return mBegin;
		}

		T const* data() const noexcept {
			return mBegin;
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
			return reverse_iterator(mBegin);
		}

		const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(mBegin);
		}

		const_reverse_iterator crend() const noexcept {
			return const_reverse_iterator(mBegin);
		}

		// Capacity
		bool empty() const noexcept {
			return size() <= 0;
		}

		size_type size() const noexcept {
			return std::distance(mBegin, mEnd);
		}

		size_type max_size() const noexcept {
			return std::min<size_type>(std::numeric_limits<difference_type>::max(),
				std::allocator_traits<allocator_type>::max_size(mAllocator));
		}

		void reserve(size_type new_cap) {
			if (new_cap >= max_size())
				throw std::length_error("vector::reserve - new_cap is greater than max_size");

			if (new_cap <= capacity()) return;

			auto tmpVec = vector<T, Allocator>(new_cap);
			int i = 0;

			for (auto it = mBegin; it != mEnd; it++)
				std::allocator_traits<allocator_type>::construct(mAllocator, tmpVec.mBegin + i++, *it);

			tmpVec.mEnd = tmpVec.mBegin + (mEnd - mBegin);
			swap(tmpVec);
		}

		size_type capacity() const noexcept {
			return std::distance(mBegin, mLast);
		}

		void shrink_to_fit() {
			if (size() == capacity()) return;

			if (empty()) reset();

			else {
				auto tmp = vector<T, Allocator>(mBegin, mEnd, mAllocator);
				swap(tmp);
			}
		}
		
		// Modifiers
		void clear() noexcept {
			if (size() == 0) return;

			while (mEnd > mBegin)
				std::allocator_traits<allocator_type>::destroy(mAllocator, --mEnd);
	
			std::allocator_traits<allocator_type>::destroy(mAllocator, mEnd);
		}

		iterator insert(const_iterator pos, T const& value) {
			return emplace(pos, value);
		}

		iterator insert(const_iterator pos, T&& value) {
			return emplace(pos, std::move(value));
		}

		iterator insert(const_iterator pos, size_type count, T const& value) {
			auto new_elem = shift(pos - mBegin, count);
			fill(new_elem, new_elem + count, value);
			return new_elem;
		}

		template <class InputIt, typename = std::enable_if_t<
			std::is_same_v<std::iterator_traits<InputIt>::iterator_category, std::random_access_iterator_tag>>>
		iterator insert(const_iterator pos, InputIt first, InputIt last) {
			iterator it = shift(pos - mBegin, last - first);
			iterator ret = it;

			for (; first != last; first++)
				std::allocator_traits<Allocator>::construct(mAllocator, it++, *first);

			return ret;
		}

		iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
			return insert(pos, ilist.begin(), ilist.end());
		}
	
		template <class... Args>
		iterator emplace(const_iterator pos, Args&&... args) {
			auto new_elem = shift(pos - mBegin, 1);

			std::allocator_traits<Allocator>::construct(
				mAllocator,
				new_elem,
				std::forward<Args>(args)...);

			return new_elem;
		}

		iterator erase(const_iterator pos) {
			std::allocator_traits<Allocator>::destroy(mAllocator, pos);
			return shift(pos - mBegin + 1, -1);
		}

		iterator erase(const_iterator first, const_iterator last) {
			for (auto it = first; it != last; it++)
				std::allocator_traits<Allocator>::destroy(mAllocator, it);
			
			return shift(last - mBegin, first - last);
		}
	
		void push_back(T const& value) {
			emplace_back(value);
		}

		void push_back(T&& value) {
			emplace_back(std::move(value));
		}

		template <class... Args>
		reference emplace_back(Args&& ... args) {
			return *emplace(mEnd, std::forward<Args>(args)...);
		}

		void pop_back() {
			erase(mEnd - 1);
		}
		
		void resize(size_type count) {
			resize(count, value_type());
		}

		void resize(size_type count, value_type const& value) {
			if (size() < count)
				insert(mEnd, count - size(), value);

			else if (size() > count)
				erase(mBegin + count, mEnd);
		}
		
		void swap(vector& other) noexcept {
			std::swap(mAllocator, other.mAllocator);
			std::swap(mBegin, other.mBegin);
			std::swap(mEnd, other.mEnd);
			std::swap(mLast, other.mLast);
		}

	private:
		iterator shift(size_type pos, difference_type count) {
			if (capacity() == 0) {
				assign(count, T());
				return mBegin;
			}

			if (count == 0) return mBegin + pos;

			if (size() + count > capacity()) {
				size_type new_cap;
				for (new_cap = capacity(); new_cap < size() + count; new_cap *= 2);
				reserve(new_cap);
			}

			if (pos < size()) {
				vector<T, Allocator> tmpVec(mBegin + pos, mEnd);
				move(tmpVec.mBegin, tmpVec.mEnd, mBegin + pos + count, mEnd + count);
			}

			mEnd += count;
			return mBegin + pos;
		}

		void move(iterator destFirst, iterator destLast, iterator srcFirst, iterator srcLast) {
			auto srcSize = srcLast - srcFirst;
			auto destSize = destLast - destFirst;

			if (srcFirst == destFirst || srcSize == 0 || destSize == 0)
				return;

			constexpr static auto inc = [](iterator& it) { ++it; };
			constexpr static auto dec = [](iterator& it) { --it; };

			bool isInc = srcFirst > destFirst;

			void(*op)(iterator&);

			if (isInc) op = inc;
			else op = inc;

			auto src = isInc ? srcFirst : srcLast - 1;
			auto dest = isInc ? destFirst : destLast - 1;
			auto count = std::min(srcSize, destSize);

			while (count--) {
				std::allocator_traits<Allocator>::construct(mAllocator, src, std::move(*dest));
				std::allocator_traits<Allocator>::destroy(mAllocator,dest);
				op(src); op(dest);
			}
		}

		void reset() {
			for (auto it = mBegin; it != mLast; it++)
				std::allocator_traits<allocator_type>::destroy(mAllocator, it);

			if (capacity() != 0)
				mAllocator.deallocate(mBegin, capacity());

			mEnd = mLast = mBegin;
		}

		void fill(iterator first, iterator last, T const& value) {
			for (; first != last; first++) {
				if (first < mEnd)
					std::allocator_traits<Allocator>::destroy(mAllocator, first);

				std::allocator_traits<Allocator>::construct(mAllocator, first, value);
			}
		}

	private:
		Allocator mAllocator;
		iterator mBegin;
		iterator mEnd;
		iterator mLast;
	};

	// Non-member functions
	template <class T, class Allocator>
	bool operator==(vector<T, Allocator> const& lhs, vector<T, Allocator> const& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		for (decltype(lhs.size()) i = 0; i < lhs.size(); i++)
			if (lhs[i] != rhs[i]) return false;

		return true;
	}

	template <class T, class Allocator>
	bool operator!=(vector<T, Allocator> const& lhs, vector<T, Allocator> const& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class Allocator>
	bool operator<(vector<T, Allocator> const& lhs, vector<T, Allocator> const& rhs) {
		return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	}

	template <class T, class Allocator>
	bool operator<=(vector<T, Allocator> const& lhs, vector<T, Allocator> const& rhs) {
		return lhs < rhs || lhs == rhs;
	}

	template <class T, class Allocator>
	bool operator>(vector<T, Allocator> const& lhs, vector<T, Allocator> const& rhs) {
		return !(lhs <= rhs);
	}

	template <class T, class Allocator>
	bool operator>=(vector<T, Allocator> const& lhs, vector<T, Allocator> const& rhs) {
		return !(lhs < rhs);
	}

	template <class T, class Allocator>
	void swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs) {
		lhs.swap(rhs);
	}

}
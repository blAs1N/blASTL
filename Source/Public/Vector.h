#pragma once

#include <initializer_list>

namespace blASTL {

	template <class T, class Allocator>
	class Vector {
	public:
		// Type Defenition
		using ThisType = Vector<T, Allocator>;
		using ValueType = T;
		using Pointer = T*;
		using ConstPointer = const T*;
		using Iterator = T*;
		using ConstIterator = T*;
		using Reference = T&;
		using ConstReference = const T&;
		using SizeType = size_t;

	public:
		// Construtor and Destructor
		Vector();
		Vector(const ThisType& other);
		Vector(ThisType&& other);
		explicit Vector(SizeType size);
		explicit Vector(SizeType size, const ValueType& value);
		explicit Vector(std::initializer_list<ValueType> list);

		template <class InputIterator>
		explicit Vector(InputIterator first, InputIterator last);

		~Vector();

		// Operator
		ThisType& operator=(const ThisType& other);
		ThisType& operator=(ThisType&& other);
		ThisType& operator=(std::initializer_list<ValueType> list);

		Reference operator[](SizeType n);
		ConstReference operator[](SizeType n) const;

		// Get Iterator
		Iterator Begin() noexcept;
		ConstIterator Begin() const noexcept;
		ConstIterator CBegin() const noexcept;

		Iterator End() noexcept;
		ConstIterator End() const noexcept;
		ConstIterator CEnd() const noexcept;

		// Size And Capacity
		bool IsEmpty() const noexcept;
		SizeType Size() const noexcept;
		SizeType Capacity() const noexcept;

		// Get Raw Pointer
		Pointer Get() noexcept;
		ConstPointer Get() const noexcept;

		// Get Element
		Reference At(SizeType n);
		ConstReference At(SizeType n) const;

		Reference Front();
		ConstReference Front() const;

		Reference Back();
		ConstReference Back() const;

		// Push and Pop
		void PushBack(const ValueType& value);
		void PushBack(ValueType&& value);
		void PopBack();

		// Emplace
		template<class... Args>
		Iterator Emplace(ConstIterator pos, Args&&... args);

		template<class... Args>
		Reference EmplaceBack(Args&&... args);

		// Insert and Erase
		Iterator Insert(ConstIterator pos, const ValueType& value);
		Iterator Insert(ConstIterator pos, ValueType&& value);
		Iterator Insert(ConstIterator pos, std::initializer_list<ValueType> list);

		template <class InputIterator>
		Iterator Insert(ConstIterator pos, InputIterator first, InputIterator last);

		Iterator EraseFirst(const ValueType& value);
		Iterator EraseLast(const ValueType& value);

		Iterator Erase(ConstIterator pos);
		Iterator Erase(ConstIterator first, ConstIterator end);

		// Clear and Reset
		void Clear() noexcept;
		void Reset() noexcept;

	private:
		Iterator mBegin;
		Iterator mEnd;

		Allocator mAllocator;
	};

}
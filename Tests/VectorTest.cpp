#include <gtest/gtest.h>
#include "blASTL/Vector.h"

using namespace blASTL;

template <class T>
void ExpectAllEqual(vector<T> const& v, std::initializer_list<T> i) {
	EXPECT_EQ(v.size(), i.size());

	auto lhs = v.begin();
	auto rhs = i.begin();

	while (lhs != v.end() || rhs != i.end())
		EXPECT_EQ(*lhs++, *rhs++);
}

class TestClass {
public:
	TestClass(int n = 0, void* ptr = nullptr) : i(n), p(ptr) {}
	virtual ~TestClass() = default;
	virtual void f() {};

	operator int() const {
		return i;
	}

private:
	int i;
	void* p;
};


TEST(Vector, Constructor) {
	std::initializer_list<TestClass> ilist = { 2, 2, 2, 2, 2 };

	vector<TestClass> v1(5, 2);
	vector<TestClass> v2(ilist);

	EXPECT_EQ(v1, v2) << "iterator and count constructor error";

	vector<TestClass> moveV2(std::move(v2));
	EXPECT_EQ(v1, moveV2) << "move constructor error";
}

TEST(Vector, Assign) {
	std::initializer_list<TestClass> ilist = { 2, 2, 2, 2, 2 };

	vector<TestClass> v1;
	v1.assign(5, 2);

	vector<TestClass> v2;
	v2.assign(ilist);

	EXPECT_EQ(v1, v2);
}

TEST(Vector, ElementAccess) {
	vector<TestClass> v{ 1, 2, 3, 4, 5 };
	EXPECT_THROW(v.at(5), std::out_of_range);
	EXPECT_EQ(v[2], 3);
	
	v[0] = 6;
	ExpectAllEqual<TestClass>(v, { 6, 2, 3, 4, 5 });

	EXPECT_EQ(v.front(), 6);
	EXPECT_EQ(v.back(), 5);

	EXPECT_EQ(v.data()[3], 4);
}

TEST(Vector, Iterator) {
	vector<TestClass> v{ 1, 2, 3, 4, 5 };
	EXPECT_EQ(*v.cbegin(), 1);

	v.begin()[4] = 6;
	EXPECT_EQ(*(v.cend() - 1), 6);

	std::initializer_list<TestClass> ilist{ 6, 4, 3, 2, 1 };

	std::for_each(v.crbegin(), v.crend(), [ilist](auto& x) {
		static auto i = ilist.begin();
		EXPECT_EQ(x, *i++);
	});
}

TEST(Vector, Insert) {
	vector<TestClass> v;
	v.insert(v.cend(), 5);
	v.insert(v.cbegin(), { 1, 2, 3, 4 });
	v.insert(v.cend(), 3, 6);
	
	ExpectAllEqual<TestClass>(v, { 1, 2, 3, 4, 5, 6, 6, 6 });
}

TEST(Vector, Erase) {
	vector<TestClass> v{ 1, 2, 3, 4, 5 };
	v.erase(v.cend() - 1);
	v.erase(v.cbegin(), v.cbegin() + 3);

	EXPECT_EQ(v.front(), 4);
}

TEST(Vector, Reserve) {
	vector<TestClass> v;
	v.reserve(5);

	EXPECT_EQ(v.size(), 0);
	EXPECT_EQ(v.capacity(), 5);
}

TEST(Vector, ShrinkToFit) {
	vector<TestClass> v(10);
	v.clear();
	v.shrink_to_fit();

	EXPECT_EQ(v.size(), 0);
}

TEST(Vector, Resize) {
	vector<TestClass> v;
	v.resize(5);
	
	EXPECT_EQ(v.size(), 5);
}

TEST(Vector, Swap) {
	std::initializer_list<TestClass> i1 = { 1, 2, 3, 4, 5 };
	std::initializer_list<TestClass> i2 = { 2, 3, 4, 5, 6 };

	vector<TestClass> v1(i1);
	vector<TestClass> v2(i2);

	swap(v1, v2);

	ExpectAllEqual(v1, i2);
	ExpectAllEqual(v2, i1);
}

TEST(Vector, WrapperMethod) {
	vector<TestClass> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(2);
	v.push_back(4);
	v.pop_back();
	v.pop_back();

	ExpectAllEqual<TestClass>(v, { 1, 2, 3, 4, 5 });
}
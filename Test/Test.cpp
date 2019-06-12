#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(Test)
	{
	public:
		
		TEST_METHOD(AssignTest)
		{
			vector<int> v;
			v.assign({ 1, 2, 3, 4, 5, 6, 7 });
			Assert::IsTrue(v.size() == 7 && v.capacity() == 7);

			v.assign(8, 3);
			Assert::IsTrue(v.size() == 8 && v.capacity() == 8);
		}

		TEST_METHOD(CopyAndMoveTest)
		{
			vector<int> v1{ 2, 9, 0, 1, 9, 3 };
			vector<int> v1clone = v1;
			vector<int> v2 = std::move(v1);
			Assert::IsTrue(v1clone == v2);
		}

		TEST_METHOD(IndexingTest)
		{
			vector<int> v{ 2, 9, 0, 1, 9, 3 };

			Assert::IsTrue(v[2] == 0);
			Assert::ExpectException<std::out_of_range>([v]() { v.at(10); });
			Assert::IsTrue(v.front() == 2);
			Assert::IsTrue(v.back() == 3);
		}

		TEST_METHOD(IteratorTest)
		{
			vector<int> v{ 2, 9, 0, 1, 9, 3 };

			int i = 0;
			for (auto const& x : v) {
				Assert::IsTrue(x == v[i++]);
			}
		}

		TEST_METHOD(ReverseIteratorTest)
		{
			vector<int> v{ 2, 9, 0, 1, 9, 3 };

			int i = v.size();
			std::for_each(v.crbegin(), v.crend(), [&](auto const& x) {
				Assert::IsTrue(x == v[--i]);
				});
		}

		TEST_METHOD(ReserveTest)
		{
			vector<int> v;
			Assert::ExpectException<std::length_error>([&]() { v.reserve(v.max_size()); });
			v.reserve(20);
			Assert::IsTrue(v.capacity() == 20);
			v.shrink_to_fit();
			Assert::IsTrue(v.capacity() == 0);
		}

		TEST_METHOD(ClearTest)
		{
			vector<int> v{ 2, 9, 0, 1, 9, 3 };
			v.clear();
			Assert::IsTrue(v.size() == 0);
		}

		TEST_METHOD(InsertTest)
		{
			vector<int> v;
			int i = 0;
			v.insert(v.cbegin(), { 1, 2, 3, 4, 5 });

			for (auto const& x : v)
				Assert::IsTrue(x == v[i++]);

			v.clear();
			i = 0;
			v.insert(v.cbegin(), 10, 5);
		
			for (auto const& x : v)
				Assert::IsTrue(x == v[i++]);
		}

		// push_back and emplace_back is just called emplace. So I don't have to test it.
		TEST_METHOD(EmplaceTest)
		{
			vector<int> v;
			v.emplace_back(5);
			v.push_back(2);
			v.emplace(v.cbegin(), 3);
			
			auto list = { 3, 5, 2 };

			for (decltype(v.size()) i = 0; i < v.size(); i++)
				Assert::IsTrue(v[i] == list.begin()[i]);
		}

		// pop_back is just called emplace. So I don't have to test it.
		TEST_METHOD(EraseTest)
		{
			auto test = [](vector<int> const& v, std::initializer_list<int> const& l) -> void {
				for (decltype(v.size()) i = 0; i < v.size(); i++)
					Assert::IsTrue(v[i] == l.begin()[i]);
			};

			vector<int> v{ 2, 9, 0, 1, 9, 3 };

			v.pop_back();
			test(v, { 2, 9, 0, 1, 9 });

			v.erase(v.cbegin() + 1);
			test(v, { 2, 0, 1, 9 });
		}

		TEST_METHOD(ResizeTest)
		{
			vector<int> v{ 2, 9, 0, 1, 9, 3 };

			v.resize(1);
			Assert::IsTrue(v.size() == 1);

			v.resize(10);
			Assert::IsTrue(v.size() == 10);
		}

		TEST_METHOD(SwapTest)
		{
			auto test = [](vector<int> const& v, std::initializer_list<int> const& l) -> void {
				for (decltype(v.size()) i = 0; i < v.size(); i++)
					Assert::IsTrue(v[i] == l.begin()[i]);
			};

			vector<int> v1{ 2, 3, 4, 5, 6 };
			vector<int> v2{ 1, 2, 3, 4, 5 };

			swap(v1, v2);

			test(v1, { 1, 2, 3, 4, 5 });
			test(v2, { 2, 3, 4, 5, 6 });
		}

	};
}

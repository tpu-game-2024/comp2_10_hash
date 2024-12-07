#include "pch.h"
#include "CppUnitTest.h"

#include "../include/lib_func.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(基本チェック)
	{
	public:
		_CrtMemState sStartMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif // _DEBUG
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif // _DEBUG
		}

		TEST_METHOD(一要素)
		{
			hash h;

			initialize(&h, 1);

			Assert::IsTrue(add(&h, 0, "Hello world!"));
			Assert::AreEqual("Hello world!", get(&h, 0), L"キー0の値はHello world!");

			finalize(&h);
		}

		TEST_METHOD(二要素)
		{
			hash h;

			initialize(&h, 2);

			Assert::IsTrue(add(&h, 4, "world!"));
			Assert::IsTrue(add(&h, 3, "Hello"));
			Assert::AreEqual("Hello", get(&h, 3), L"キー3の値はHello");
			Assert::AreEqual("world!", get(&h, 4), L"キー4の値はworld!");
			 
			finalize(&h);
		}

		TEST_METHOD(同じhash値はずらす)
		{
			hash h;

			initialize(&h, 2);

			Assert::IsTrue(add(&h, 0, "world!"));
			Assert::IsTrue(add(&h, 2, "Hello"));
			Assert::AreEqual<int>(1, debug_get_hash(&h, 2), L"2のキーのハッシュは1");

			finalize(&h);
		}

		TEST_METHOD(３要素)
		{
			hash h;

			initialize(&h, 3);

			Assert::IsTrue(add(&h, 6, "Hello"));
			Assert::IsTrue(add(&h, 7, "world"));
			Assert::IsTrue(add(&h, 9, "!"));
			Assert::AreEqual("Hello", get(&h, 6), L"キー6の値はHello");
			Assert::AreEqual("world", get(&h, 7), L"キー7の値はworld");
			Assert::AreEqual("!", get(&h, 9), L"キー9の値は!");

			finalize(&h);
		}

		TEST_METHOD(追加順を入れ替えても同じ結果)
		{
			hash h;

			initialize(&h, 3);

			Assert::IsTrue(add(&h, 9, "!"));
			Assert::IsTrue(add(&h, 7, "world"));
			Assert::IsTrue(add(&h, 6, "Hello"));
			Assert::AreEqual("Hello", get(&h, 6), L"キー6の値はHello");
			Assert::AreEqual("world", get(&h, 7), L"キー7の値はworld");
			Assert::AreEqual("!", get(&h, 9), L"キー9の値は!");

			finalize(&h);
		}
	};


	TEST_CLASS(品質チェック)
	{
	public:
	public:
		_CrtMemState sStartMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(NULLチェック)
		{
			initialize(NULL, 0);
			finalize(NULL);

			hash h;
			initialize(&h, 1);
			Assert::IsNull(get(&h, 0), L"何も登録していなければ検索しても何も得られない");
			finalize(&h);
		}

		TEST_METHOD(異なる要素)
		{
			hash h;

			initialize(&h, 1);

			Assert::IsTrue(add(&h, 1, "This is a pen"));
			Assert::IsNull(get(&h, 0), L"存在しないキーを検索しても何も得られない");

			finalize(&h);
		}

		TEST_METHOD(大量の要素)
		{
			hash h;

			const int NUM = 10000;
			initialize(&h, NUM);
			for (int i = 0; i < NUM; i++) {
				char c[2] = { 'a' + (i%26) };
				Assert::IsTrue(add(&h, i, c));
			}
			Assert::IsFalse(add(&h, NUM, "a"));

			for (int i = 0; i < NUM; i++) {
				Assert::AreEqual((char)('a'+(i % 26)), *get(&h, i), L"周期的にアルファベットが入出てくるはず");
			}

			finalize(&h);
		}

		TEST_METHOD(２度の解放)
		{
			hash h;

			initialize(&h, 1);
			Assert::IsTrue(add(&h, 2, "Release twice"));
			finalize(&h);
			finalize(&h);
		}
	};

}

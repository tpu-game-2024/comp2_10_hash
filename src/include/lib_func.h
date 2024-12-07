#pragma once // インクルードガード

#include <stdbool.h>

// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
extern "C" {
#endif

	struct hash_node
	{
		unsigned int key = ~0;
		char value[256] = "";
	};

	typedef struct hash_
	{
		unsigned int max_size = 0;
		hash_node* nodes = NULL;
	}hash;
	
	// ハッシュの初期化(max_sizeは~0未満)
	bool initialize(hash* h, unsigned int max_size);

	// 使用メモリの全解放
	void finalize(hash* h);

	// keyの値を見てノードを追加する(追加できなかったらfalseを返す)
	bool add(hash* h, unsigned int key, const char* value);

	// keyの値を見てノードを検索して、値を取得する(なければNULLを返す)
	const char* get(const hash* h, unsigned int key);

	// ハッシュの値を取得する(デバッグ用)
	int debug_get_hash(const hash* h, unsigned int key);

	// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
} // extern "C"
#endif

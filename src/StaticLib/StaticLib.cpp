#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"

static unsigned int hash_func(unsigned int key, int num)
{
	return key % num;
}

static unsigned int get_hash(const hash* h, unsigned int key)
{
	if (h == NULL) return ~0;
	if (key == ~0) return ~0;

	// ToDo: ハッシュ関数としてhash_funcを使った
	// オープンアドレス法によるハッシュ値を求める
	unsigned int index = hash_func(key, h->max_size);
	for (unsigned int i = 0; i < h->max_size; i++) {
		unsigned int probe = (index + i) % h->max_size;
		if (h->nodes[probe].key == key || h->nodes[probe].key == ~0) {
			return probe;
		}
	}
	return ~0;
}

// ハッシュの初期化(max_sizeは~0未満)
bool initialize(hash* h, unsigned int max_size)
{
	if (h == NULL) return false;
	if (max_size == 0 || max_size == ~0) return false;

	if (h->nodes != NULL) finalize(h);

	h->nodes = (hash_node*)malloc(max_size * sizeof(hash_node));
	if (NULL == h->nodes) return false;

	for (unsigned int i = 0; i < max_size; i++) {
		h->nodes[i].key = ~0;
		h->nodes[i].value[0] = '\0';
	}

	h->max_size = max_size;

	return true;
}

// 使用メモリの全解放
void finalize(hash* h)
{
	if (h == NULL) return;

	free(h->nodes);
	h->nodes = NULL;
	h->max_size = 0;
}

// keyの値を見てノードを追加する(追加できなかったらfalseを返す)
bool add(hash* h, unsigned int key, const char* value)
{
	if (h == NULL || h->nodes == NULL || value == NULL) return false;

	unsigned int index = hash_func(key, h->max_size);
	for (unsigned int i = 0; i < h->max_size; i++) {
		unsigned int probe = (index + i) % h->max_size;
		if (h->nodes[probe].key == ~0) { // 空きスロットに追加
			h->nodes[probe].key = key;
			// 安全な文字列コピー
			strncpy_s(h->nodes[probe].value, sizeof(h->nodes[probe].value), value, sizeof(h->nodes[probe].value) - 1);
			return true;
		}
		else if (h->nodes[probe].key == key) { // キーが既に存在する場合
			// 値を上書き
			strncpy_s(h->nodes[probe].value, sizeof(h->nodes[probe].value), value, sizeof(h->nodes[probe].value) - 1);
			return true;
		}
	}
	return false; // ハッシュテーブルが満杯の場合
}


// keyの値を見てノードを検索して、値を取得する(なければNULLを返す)
const char* get(const hash* h, unsigned int key)
{
	if (h == NULL || h->nodes == NULL || key == ~0) return NULL;

	unsigned int index = hash_func(key, h->max_size);
	for (unsigned int i = 0; i < h->max_size; i++) {
		unsigned int probe = (index + i) % h->max_size;
		if (h->nodes[probe].key == key) { // 該当するキーが見つかった
			return h->nodes[probe].value;
		}
		else if (h->nodes[probe].key == ~0) {
			break; // 空きスロットに到達したら終了
		}
	}
	return NULL; // キーが見つからない場合
}


// ハッシュの値を取得する(デバッグ用)
int debug_get_hash(const hash* h, unsigned int key)
{
	return get_hash(h, key);
}
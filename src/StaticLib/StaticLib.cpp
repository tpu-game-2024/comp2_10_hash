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

	unsigned int index = hash_func(key, h->max_size);
	unsigned int original_index = index;

	while (h->nodes[index].key != ~0 && h->nodes[index].key != key) {
		index = (index + 1) % h->max_size;
		if (index == original_index) {

			return ~0;
		}
	}
	return index;
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
	if (h == NULL) return false;
	if (h->max_size == ~0) return false;
	if (key == ~0) return NULL;

	unsigned int index = get_hash(h, key);
	if (index == ~0 || h->nodes[index].key != ~0) {
		return false;
	}

	h->nodes[index].key = key;
	strncpy_s(h->nodes[index].value, value, sizeof(h->nodes[index].value) - 1);
	h->nodes[index].value[sizeof(h->nodes[index].value) - 1] = '\0';

	return true;
}

// keyの値を見てノードを検索して、値を取得する(なければNULLを返す)
const char* get(const hash* h, unsigned int key)
{
	if (h == NULL) return NULL;
	if (key == ~0) return NULL;

	unsigned int index = get_hash(h, key);
	if (index == ~0 || h->nodes[index].key == ~0) {
		return NULL;
	}

	return h->nodes[index].value;
}

// ハッシュの値を取得する(デバッグ用)
int debug_get_hash(const hash* h, unsigned int key)
{
	return get_hash(h, key);
}
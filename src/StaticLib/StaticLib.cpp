#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"
static unsigned int hash_func(unsigned int key, int num)
{
    return key % num;  // 基本的なハッシュ関数
}

static unsigned int get_hash(const hash* h, unsigned int key)
{
    if (h == NULL) return ~0;
    if (key == ~0) return ~0;

    unsigned int index = hash_func(key, h->max_size);
    unsigned int original_index = index;  // 衝突時に元のインデックスに戻せるように保持

    // 線形探索によるオープンアドレス法
    while (h->nodes[index].key != ~0) {  // 空いている場所まで探索
        if (h->nodes[index].key == key) {  // キーが一致した場合
            return index;
        }
        index = (index + 1) % h->max_size;  // 次のインデックスへ
        if (index == original_index) break; // 全ての場所をチェックしたら終了
    }
    return ~0;  // キーが見つからない場合
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
    if (key == ~0 || value == NULL) return false;

    unsigned int index = get_hash(h, key);
    if (index != ~0) {
        // キーが既に存在している場合、上書きする（または何もしない）
        strcpy_s(h->nodes[index].value, sizeof(h->nodes[index].value), value);
        return true;
    }

    // 新たに空いている場所を探して挿入
    index = hash_func(key, h->max_size);  // ハッシュ値を求める
    unsigned int original_index = index;

    while (h->nodes[index].key != ~0) {
        index = (index + 1) % h->max_size;
        if (index == original_index) return false;  // テーブルが満杯なら失敗
    }

    // 空いている場所が見つかったので追加
    h->nodes[index].key = key;
    strcpy_s(h->nodes[index].value, sizeof(h->nodes[index].value), value);
    return true;
}

// keyの値を見てノードを検索して、値を取得する(なければNULLを返す)
const char* get(const hash* h, unsigned int key)
{
    if (key == ~0) return NULL;

    unsigned int index = get_hash(h, key);
    if (index == ~0) return NULL;

    return h->nodes[index].value;
}

// ハッシュの値を取得する(デバッグ用)
int debug_get_hash(const hash* h, unsigned int key)
{
    return get_hash(h, key);
}
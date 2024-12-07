#include <iostream>
#include "../include/lib_func.h"

int main()
{
    hash h;

    const char *word = u8"Now I won!";
    initialize(&h, (unsigned int)strlen(word));
    for (int i = 0; i < strlen(word); i++) {
        char c[2] = { word[i] };
        add(&h, (int)strlen(word) - 1 - i, c);
    }

    printf("%sを逆さに読むと", word);

    for (int i = 0; i < strlen(word); i++) {
        printf("%s", get(&h, i));
    }

    printf("\n");

    finalize(&h);
}

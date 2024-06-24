#include "../emu.h"
#include <stdio.h>

int main() {
    const uint* file_c = EMU_GetCodeFromFile("pr.em");

    for (int i = 0; i < 5; i++) {
        printf("%d\n", file_c[i]);
    }

    return 0;
}

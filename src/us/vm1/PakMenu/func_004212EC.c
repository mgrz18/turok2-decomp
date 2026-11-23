#include "common.h"
#include "globals/scene.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80405DC0_16.CPP, func_80409884_de, with Turok 2's layouts. */


char *Pak_GetBufferSize(s32 block) {
    if (block == 0) {
        return (*(char * *) &gScene.unk_30) + 0x688;
    }
    return (char *) (block << 8);
}

#include "common.h"

extern s32 D_800B4B84;
extern s32 D_800B4B8C;
extern s32 D_801309B8;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *table;
} Unk004220F0;

s32 func_004220F0(Unk004220F0 *arg0) {
    if (D_801309B8 != 0) {
        arg0->table = &D_800B4B84;
    } else {
        arg0->table = &D_800B4B8C;
    }
    return 0;
}

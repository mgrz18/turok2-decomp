#include "common.h"

s32 func_002017D4(s32, s16);                        /* extern */

typedef struct {
    /* 0x0000 */ u8 pad0[0x2B4C];
    /* 0x2B4C */ s32 unk2B4C;
} Unk00271D84;

void func_00271D84(Unk00271D84 *arg0, s16 arg1) {
    func_002017D4(func_002017D4(arg0->unk2B4C, arg1), 1);
}

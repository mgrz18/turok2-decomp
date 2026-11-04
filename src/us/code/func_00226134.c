#include "common.h"

typedef struct World {
    /* 0x00 */ u8 pad0[0x68];
    /* 0x68 */ s32 table;
} World;

s32 func_002017D4(s32, s32);

void func_00226134(World *arg0, s32 arg1) {
    func_002017D4(func_002017D4(arg0->table, arg1), 0);
}

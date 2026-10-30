#include "common.h"

typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 body[0x68 - 0x4];
} Slot00403494;

typedef struct {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ Slot00403494 slots[5];
    /* 0x210 */ u8 pad1[0x214 - 0x210];
    /* 0x214 */ s32 *info;
} State00403494;

void func_00236314(void *slot, void *arg);

void func_00403494(State00403494 *s, void *arg) {
    s32 index = s->info[5];

    if (index != -1) {
        /* &s->slots[index].body, computed as an address */
        func_00236314((void *)(index * sizeof(Slot00403494) + (s32)s + 0xC), arg);
    }
}

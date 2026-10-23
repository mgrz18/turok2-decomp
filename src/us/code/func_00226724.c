#include "common.h"

typedef struct Entry8 {
    s32 handle;
    s32 unk4;
} Entry8;

typedef struct Cache {
    u8 pad0[0x8EC];
    s32 unk8EC;           /* 0x8EC */
    u8 pad8F0[0xBF4 - 0x8F0];
    s32 unkBF4;           /* 0xBF4 */
    u8 padBF8[0x1064 - 0xBF8];
    s32 unk1064;          /* 0x1064 */
    u8 pad1068[0x14A8 - 0x1068];
    s32 count;            /* 0x14A8 */
    Entry8 entries[1];    /* 0x14AC */
} Cache;

extern void func_002051F4(s32 arg0, s32 arg1);

void func_00226724(Cache *cache) {
    s32 i;
    s32 count;
    Entry8 *entries;

    count = cache->count;
    entries = cache->entries;
    for (i = 0; i < count; i++) {
        func_002051F4(0, entries[i].handle);
    }
    cache->unk8EC = 0;
    cache->unkBF4 = 0;
    cache->unk1064 = 0;
    cache->count = 0;
}

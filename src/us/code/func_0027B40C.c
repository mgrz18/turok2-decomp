#include "common.h"

typedef struct {
    /* 0x00 */ s32 offset;
    /* 0x04 */ u8 flag;
    /* 0x05 */ u8 pad5[3];
} ChunkEntry;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ u8 pad2[6];
    /* 0x08 */ ChunkEntry entries[1];
} ChunkTable;

typedef struct {
    /* 0x00 */ ChunkTable *table;
} Resource;

typedef struct {
    /* 0x00 */ u8 *data;
    /* 0x04 */ u8 flag;
} Chunk;

extern u8 D_800F7078[];
extern u8 D_800F5618[0x90];
extern u8 *D_800B2AC0;

Resource *func_00224DF4(void *cache, s32 id, s32, s32);
void func_002051F4(s32, Resource *res);
void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */

/* Loads the 0x90-byte block of chunk 42 of resource 2 into D_800F5618 and
 * points D_800B2AC0 at it (NULL until it is loaded). */
void func_0027B40C(void) {
    Resource *res;
    Chunk chunk;
    ChunkTable *table;
    ChunkEntry *entries;
    u8 *data;
    s32 index;

    D_800B2AC0 = NULL;
    res = func_00224DF4(D_800F7078, 2, 0, 1);
    if (res != NULL) {
        table = res->table;
        index = 42 % table->count;
        chunk.data = NULL;
        entries = table->entries;
        data = (u8 *) table + entries[index].offset;
        chunk.flag = entries[index].flag;
        chunk.data = data;
        if (chunk.data != NULL) {
            chunk.data += 0x10;
        }
        func_0029E460(D_800F5618, chunk.data, 0x90);
        func_002051F4(0, res);
        D_800B2AC0 = D_800F5618;
    }
}

#include "common.h"
#include "structs/camera.h"
#include "structs/cache.h"

/* The resource lists (0x800D81E0 on): every resident resource, the locked ones, and
 * at 0xBCC the free-slot count (D_800D8DAC). The word 8 bytes before is the last
 * resource handed out (D_800D81D8), addressed through this symbol. */
typedef struct {
    /* 0x000 */ CList resident;
    /* 0x014 */ CList locked;
    /* 0x028 */ u8 pad28[0xBCC - 0x28];
    /* 0xBCC */ s32 freeCount;
} ResourceLists;

s32 CList__Remove(s32, s32);

extern s32 D_800D81D4;
extern ResourceLists D_800D81E0;
extern s32 D_800D8DAC;

#define LAST_RESOURCE (((CMemEntry **) &D_800D81E0)[-2])

/* Drops a resource from the lists and returns its slot to the free stack. */
void CCache__FreeMemEntry(s32 arg0, CMemEntry *arg1) {
    s32 *flags = &arg1->flags;

    CList__Remove((s32) &D_800D81E0.resident, (s32) arg1);
    if (arg1->flags & 0x1000) {
        CList__Remove((s32) &D_800D81E0.locked, (s32) arg1);
    }
    if (LAST_RESOURCE == arg1) {
        LAST_RESOURCE = 0;
    }
    *flags = 0;
    *(CMemEntry **)((D_800D8DAC * 4) + D_800D81D4) = arg1;
    D_800D81E0.freeCount = (s32) (D_800D81E0.freeCount + 1);
}

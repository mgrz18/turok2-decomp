#include "common.h"
#include "structs/camera.h"
#include "structs/cache.h"

/* The resource lists (0x800D81E0 on): every resident resource, the locked ones, and
 * at 0xBCC the free-slot count (gCache_freeEntryCount). The word 8 bytes before is the last
 * resource handed out (gCache_currentCompressPos), addressed through this symbol. */
typedef struct {
    /* 0x000 */ CList resident;
    /* 0x014 */ CList locked;
    /* 0x028 */ u8 pad28[0xBCC - 0x28];
    /* 0xBCC */ s32 freeCount;
} ResourceLists;

s32 CList__Remove(s32, s32);

extern s32 gCache_memEntriesFree;
extern ResourceLists gCache_memAgeList;
extern s32 gCache_freeEntryCount;

#define LAST_RESOURCE (((CMemEntry **) &gCache_memAgeList)[-2])

/* Drops a resource from the lists and returns its slot to the free stack. */
void CCache__FreeMemEntry(s32 arg0, CMemEntry *arg1) {
    s32 *flags = &arg1->flags;

    CList__Remove((s32) &gCache_memAgeList.resident, (s32) arg1);
    if (arg1->flags & 0x1000) {
        CList__Remove((s32) &gCache_memAgeList.locked, (s32) arg1);
    }
    if (LAST_RESOURCE == arg1) {
        LAST_RESOURCE = 0;
    }
    *flags = 0;
    *(CMemEntry **)((gCache_freeEntryCount * 4) + gCache_memEntriesFree) = arg1;
    gCache_memAgeList.freeCount = (s32) (gCache_memAgeList.freeCount + 1);
}

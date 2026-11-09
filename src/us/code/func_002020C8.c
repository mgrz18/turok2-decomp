#include "common.h"
#include "structs/loader.h"
#include "protos/list.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

/* Turok 3's loader.c CLoader__DeallocLoaderEntry: the used list back to the
 * free list, with interrupts off (CLoader__AllocLoaderEntry's mirror). */
void CLoader__DeallocLoaderEntry(CLoader *thisx, CLoaderEntry *entry) {
    u32 mask = __osDisableInt();

    CList__Remove(&thisx->usedList, entry);
    entry->flags = LOADERFLAGS_UNALLOCATED;
    CList__AddHead(&thisx->freeList, entry);
    __osRestoreInt(mask);
}

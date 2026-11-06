/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_4.c, func_8025661C_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/loader.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);

CLoaderEntry *CLoader__AllocLoaderEntry(CLoader *thisx) {
    u32 mask;
    CLoaderEntry *entry;

    mask = __osDisableInt();
    entry = thisx->freeList.m_pHead;
    if (entry != NULL) {
        CList__Remove(&thisx->freeList, entry);
        entry->flags = LOADERFLAGS_ALLOCATED;
        CList__AddHead(&thisx->usedList, entry);
    }
    __osRestoreInt(mask);
    return entry;
}

#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"

extern void CList__MoveToHead(CList *list, void *element);

/* Stamps a CMemEntry with this frame and moves it to the head of the age list. */
void CCache__TouchMemEntry(s32 unused, CMemEntry *entry) {
    entry->age = gCache.ageCount;
    CList__MoveToHead(&gCache.memAgeList, entry);
}

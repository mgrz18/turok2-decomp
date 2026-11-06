/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802517B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/cache.h"

extern int gCache_mutex_count;
extern void *CCache__LookupAndLock(s32 arg0, s32 arg1);
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern struct Shape_func_8021A2D4_de_2 gHashMask;
extern struct Shape_func_8021A2D4_de_2 gHashTable;
extern void CList__MoveToHead(void *, s32);
void *CCache__LookupAndLock(s32 arg0, s32 arg1) {
    extern s32 osSendMesg(OSMesgQueue *, s32, s32);
    extern struct Shape_func_8021A2D4_de_2 gCache_ageCount;
    extern OSMesgQueue gCache_mutex;
    extern s32 gCache_memAgeList;
    s32 temp_v1;
    s32 temp_v1_2;
    u32 temp_a0;
    u32 temp_v0;
    HashNode *node;
    void *value;
    void **out;
    CMemEntry *object;
    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    out = &value;
    node = (HashNode *)((s32)gHashTable.field_0 +
        ((((arg1 << 5) ^ ((u32)arg1 >> 1) ^ ((u32)arg1 >> 9) ^
           ((u32)arg1 >> 17)) & gHashMask.field_0) * 0x10));
    if (node->key != arg1) {
        goto not_initial;
    }
    value = node->value;
    goto found;
loop_found:
    *out = node->value;
    goto found;
not_initial:
    value = 0;
    if (node == 0) {
        goto found;
    }
loop:
    if (node->key == arg1) {
        goto loop_found;
    }
    node = node->next;
    if (node != 0) {
        goto loop;
    }
found:
    object = 0;
    if (value != 0) {
        object = *(void **)value;
        object->lockCount += 1;
        object->flags |= MEMENTRY_FLAG_LOCKED;
        object->age = gCache_ageCount.field_0;
        CList__MoveToHead(&gCache_memAgeList, object);
    }
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    return object;
}

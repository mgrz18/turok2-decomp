#include "common.h"

/* A cached resource (see func_002060E0): its data, its key, a reference count and flags (0x100: referenced). */
typedef struct {
    /* 0x00 */ s32 data;
    /* 0x04 */ s32 key;
    /* 0x08 */ s32 refCount;
    /* 0x0C */ s32 flags;
} CacheEntry;

/* Two cache references. */
typedef struct {
    /* 0x00 */ CacheEntry *first;
    /* 0x04 */ CacheEntry *second;
} CacheRefs;

s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 D_800D8DB0;
extern s32 D_800D8DCC;

void func_00205904(s32 arg0, CacheRefs *arg1, CacheEntry *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 temp_v1_2;
    CacheEntry *temp_a0_2;
    CacheEntry *temp_a0_3;

    temp_a0 = __osDisableInt();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg((s32) &D_800D8DB0, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_a0_2 = arg1->first;
    if (temp_a0_2 != NULL) {
        temp_v0 = temp_a0_2->refCount - 1;
        temp_a0_2->refCount = temp_v0;
        if (temp_v0 == 0) {
            temp_a0_2->flags &= ~0x100;
        }
    }
    temp_a0_3 = arg1->second;
    if (temp_a0_3 != NULL) {
        temp_v0_2 = temp_a0_3->refCount - 1;
        temp_a0_3->refCount = temp_v0_2;
        if (temp_v0_2 == 0) {
            temp_a0_3->flags &= ~0x100;
        }
    }
    arg1->first = arg2;
    temp_v0_3 = __osDisableInt();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_3);
        osSendMesg((s32) &D_800D8DB0, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_3);
}

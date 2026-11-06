#include "common.h"
#include "structs/camera.h"

/* A pool of 0x3C-byte elements: the free list, the used list and a count. */
typedef struct {
    /* 0x00 */ CList m_Free;
    /* 0x14 */ CList m_Used;
    /* 0x28 */ s32 m_nCount;
} CPool;

s32 CList__Construct(void *, s32, s32);    /* extern */
s32 CList__AddTail(void *, s32);                 /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

void RumblePool_Init(CPool *arg0, s32 arg1, s32 arg2) {
    s32 temp_s4;
    s32 var_s0;
    s32 var_s1;

    var_s1 = arg1;
    temp_s4 = __osDisableInt();
    CList__Construct(&arg0->m_Free, 0, 4);
    CList__Construct(&arg0->m_Used, 0, 4);
    var_s0 = 0;
    if (arg2 > 0) {
        do {
            CList__AddTail(&arg0->m_Free, var_s1);
            var_s0 += 1;
            var_s1 += 0x3C;
        } while (var_s0 < arg2);
    }
    arg0->m_nCount = 0;
    __osRestoreInt(temp_s4);
}

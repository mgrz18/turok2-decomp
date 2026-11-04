#include "common.h"
#include "structs/camera.h"

/* Four 0x228-byte slots and the lists that hand them out. */
typedef struct {
    /* 0x000 */ u8 m_Slots[4][0x228];
    /* 0x8A0 */ CList m_Free;
    /* 0x8B4 */ CList m_Used;
    /* 0x8C8 */ s16 m_nCount;
} CSlotPool;

void CList__Construct(s32, s32, s32);
s32 CList__AddTail(s32, s32);

void func_00468F10(CSlotPool *arg0) {
    s32 var_s0;
    s32 var_s1;

    CList__Construct((s32) &arg0->m_Used, 0, 4);
    CList__Construct((s32) &arg0->m_Free, 0, 4);
    var_s1 = 0;
    var_s0 = (s32) arg0;
    do {
        CList__AddTail((s32) &arg0->m_Free, var_s0);
        var_s1 += 1;
        var_s0 += 0x228;
    } while (var_s1 < 4);
    arg0->m_nCount = 0;
}

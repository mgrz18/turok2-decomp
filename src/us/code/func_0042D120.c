/* WRONG CUT: REAL func_0042D120 size 0x98 (splat cut it into func_0042D120 + func_0042D140). */
#include "common.h"
#include "structs/list.h"
#include "protos/list.h"

typedef struct Slot {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8 pad10[6];
    /* 0x16 */ s16 unk16;
} Slot; /* 0x18 */

typedef struct SlotLists {
    /* 0x00 */ CList used;
    /* 0x14 */ CList free;
    /* 0x28 */ Slot slots[8];
} SlotLists;


#ifdef TARGET_PORT
/* The port: on CDLightSet (the N64 view's slots have 4-byte links; on the host the lists
 * overran the set into the scene's level and warp state). */
#include "structs/particle.h"
void func_0042D120(CDLightSet *lists) {
    s32 i;
    CDLight *slot;

    CList__Construct(&lists->m_UsedList, 0, sizeof(void *));
    CList__Construct(&lists->m_FreeList, 0, sizeof(void *));
    for (i = 0; i < 8; i++) {
        slot = &lists->m_Lights[i];
        CList__AddTail(&lists->m_FreeList, slot);
        slot->m_pROMLight = NULL;
        slot->m_ColorFade = 1.0f;
        slot->m_bEnabled = 0;
    }
}
#else
void func_0042D120(SlotLists *lists) {
    s32 i;
    Slot *slot;

    CList__Construct(&lists->used, 0, sizeof(void *));
    CList__Construct(&lists->free, 0, sizeof(void *));
    for (i = 0; i < 8; i++) {
        slot = &lists->slots[i];
        CList__AddTail(&lists->free, slot);
        slot->unk8 = 0;
        slot->unkC = 1.0f;
        slot->unk16 = 0;
    }
}
#endif

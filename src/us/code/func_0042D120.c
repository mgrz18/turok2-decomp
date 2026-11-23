/* WRONG CUT: REAL CDLightSet__Construct size 0x98 (splat cut it into CDLightSet__Construct + func_0042D140). */
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


/* The port: on CDLightSet (the N64 view's slots have 4-byte links; on the host the lists
 * overran the set into the scene's level and warp state). */
#include "structs/particle.h"
void CDLightSet__Construct(CDLightSet *lists) {
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

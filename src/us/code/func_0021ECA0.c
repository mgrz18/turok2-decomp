#include "common.h"

typedef struct Slot24 {
    s32 w[6];
} Slot24;

/* A CGameObjectInstance (structs/game_object.h) seen as its m_Lights: through
 * the header's u8 m_Lights[2][0x18] the index is added the other way round
 * (addu $v0, $v0, $a0), so the slot array stays local. */
typedef struct Owner {
    u8 pad0[0x110];
    Slot24 slots[1];    /* 0x110 */
} Owner;

extern s32 gEvenOdd;
extern void CGameObjectInstance__DrawModel(Owner *arg0, s32 arg1, s32 arg2);

void CGameObjectInstance__DrawModelWithLights(Owner *arg0, s32 arg1, s32 arg2, Slot24 *src) {
    arg0->slots[gEvenOdd] = *src;
    CGameObjectInstance__DrawModel(arg0, arg1, arg2);
}

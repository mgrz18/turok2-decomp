/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0020459C.c, func_0020459C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"


typedef struct ActorDef {
    char pad0[0x20];
    s16 unk20; /* info.unkC */
} ActorDef;


s32 func_00266200(CGameObjectInstance *actor) {
    s32 v = ((ActorDef *)actor->header.m_pIntelligence)->unk20;
    if (v != 0) {
        actor->m_dwFlags |= 0x2000;
    } else {
        actor->m_dwFlags &= ~0x2000;
    }
    return v;
}

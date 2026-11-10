/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00207D34.c, func_00207D34.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


#include "protos/game.h"
typedef struct B2Info {
    u32 flags;
    u8 pad4[0x20];
    u32 flags24;
} B2Info;
typedef struct B2Def {
    s32 type;
    u8 pad4[0x10];
    B2Info info;            /* 0x14 */
} B2Def;
typedef struct B2Actor {
    u8 pad0[0x14];
    B2Def *def;             /* 0x14 in Turok 2 (0x18 in Rage Wars) */
} B2Actor;
typedef struct B2State {
    u32 flags;
} B2State;

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the intelligence from CGameObjectInstance (the B2Actor view reads it at N64 0x14). */
void Platform_BackEnter(B2Actor *actor, B2State *state) {
    B2Info *info = (B2Info *) ((u8 *) ((CGameObjectInstance *) actor)->header.m_pIntelligence + 0x14);
    GameObject_FireTriggers(actor, 0x20000, actor);
    if ((info->flags24 & 0x80) == 0) {
        state->flags &= ~0x10000000;
    }
}
#else
void Platform_BackEnter(B2Actor *actor, B2State *state) {
    B2Info *info = &actor->def->info;
    GameObject_FireTriggers(actor, 0x20000, actor);
    if ((info->flags24 & 0x80) == 0) {
        state->flags &= ~0x10000000; /* Rage Wars clears 0x20000 */
    }
}
#endif

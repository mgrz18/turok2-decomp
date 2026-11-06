/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80204E78_6.CPP, func_80205EC4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"

/* Turok 2: def 0x14, type id 0xB8, flags 0xD4 (Rage Wars 0x18 / 0xE4 / 0x100). */
/* Turok 2: update hook at 0xF0 (Rage Wars 0x108). */
typedef struct ActorMotionState {
    char pad0[0x2C];
    void *table;
    char pad30[0xC0];
    void *update;
} ActorMotionState;
void Generator_Step();
extern int gGeneratorModeTable;
void Generator_Setup(void *arg0, void *arg1) {
    CGameObjectInstance *actor = arg0;
    ActorMotionState *state = arg1;
    CDestructibleIntelligence *inner = &actor->header.m_pIntelligence->m_Destructible;
    state->table = &gGeneratorModeTable;
    state->update = &Generator_Step;
    if ((actor->m_nTypeFlag == 0x644) ||
        (inner->m_dwFlags & 8)) {
        actor->m_dwFlags &= ~0x2000;
    }
}

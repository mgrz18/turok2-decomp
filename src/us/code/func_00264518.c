/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00207B5C.c, func_00207B5C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
typedef struct Actor { /* Turok 2 layout */
                 u8 pad0[0x14];
                 CIntelligence *def;    /* 0x14 (Rage Wars: 0x18) */
} Actor;
void func_0026E110(Actor *actor, s32 flags, Actor *actor2);
void func_00264518(Actor *actor, u32 *flags) {
    CEnemyIntelligence *info = &actor->def->m_Enemy;
    func_0026E110(actor, 0x10000, actor);
    if (!(info->m_dwFlags2 & 0x40)) {
        *flags &= 0xF7FFFFFF;
    }
}

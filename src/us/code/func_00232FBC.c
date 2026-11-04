/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284980.c, func_00284980.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/particle.h"

int func_00232FBC(CParticle *o) {
    switch (o->header.m_nObjType) {
    case 8:
    case 0xD:
    case 0x12:
        return 1;
    default:
        return 0;
    }
}

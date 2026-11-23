/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002848B0.c, func_002848B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/particle.h"
s32 func_00232EEC(CParticle *o) {
    switch (o->header.m_nObjType) {
    case 2:
    case 0x56:
    case 0x111:
    case 0x126:
        return 1;
    default:
        return 0;
    }
}

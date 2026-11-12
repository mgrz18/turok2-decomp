/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80204E78_4.CPP, func_80205494_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"
#else
typedef signed char s8;
typedef signed int s32;
#endif

typedef struct Descriptor {
    char pad0[0x14];
    s32 damageFlags;            /* 0x14 */
} Descriptor;

#ifdef TARGET_PORT
/* The port (round 11 C26): the members at their host offsets (the N64 pads put the flags on
 * m_rpGeometryHeaderSize, as in Destructible_GoEnter, func_00265B58.c; the CAI byte below read
 * another member) */
typedef struct BreakableHitContext {
    union {
        struct { char _hpDef[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; Descriptor *def; };
        struct { char _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; s32 flags; };
    };
} BreakableHitContext;
#else
typedef struct BreakableHitContext {
    char pad0[0x14];
    Descriptor *def;            /* 0x14 (Rage Wars 0x18) */
    char pad18[0xD4 - 0x18];
    s32 flags;                  /* 0xD4 (Rage Wars 0x100) */
} BreakableHitContext;
#endif

typedef struct ObjectStateCC {
#ifdef TARGET_PORT
    char pad0[__builtin_offsetof(CAI, m_AnimRepeatedCount)];
#else
    char pad0[0xC7];
#endif
    s8 unk_C7;                  /* 0xC7 (Rage Wars 0xCB) */
} ObjectStateCC;

void Destructible_GoUpdate(BreakableHitContext *arg0, ObjectStateCC *arg1) {
    Descriptor *def = arg0->def;
    if (arg1->unk_C7 != 0 && (def->damageFlags & 0x4) != 0) {
        unsigned int flags = arg0->flags;
        flags &= ~0x2000;
        flags &= ~0x100;
        arg0->flags = flags;
    }
}

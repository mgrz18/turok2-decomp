/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00267540.c, func_00267540.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/scene.h"

#include "common.h"
#include "globals/game.h"
typedef struct { s32 kind; } P3Kind;
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the members at their host offsets (round 11 C26: the N64 pads put the flags (0xD4) on host m_rpGeometryHeader/m_rpObjectHeader) */
typedef struct {
    union {
        struct { u8 _hpInfo[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; P3Kind *info; };
        struct { u8 _hpId[__builtin_offsetof(CGameObjectInstance, m_nTypeFlag)]; u16 id; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; u32 flags; };
    };
} P3Actor;
#else
typedef struct {
    u8 pad0[0x14];
    P3Kind *info;           /* 0x14 (0x18 in Rage Wars) */
    u8 pad18[0xB8 - 0x18];
    u16 id;                 /* 0xB8 (0xE4) */
    u8 padBA[0xD4 - 0xBA];
    u32 flags;              /* 0xD4 (0x100) */
} P3Actor;
#endif
s32 func_00238258(P3Actor *a, P3Actor *b) {
    if (gEngineApp_m_Settings.multiplayer != 0) {
        return 1;
    }
    if (a->id == b->id || !(b->flags & 0x310000)) {
        return 0;
    }
    if (gScene.m_bStreamAll != 0 && !(a->flags & 0x300000) && !(b->flags & 0x300000)) {
        return 0;
    }
    switch (b->info->kind) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        return 1;
    }
    return 0;
}

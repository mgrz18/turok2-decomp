/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024E1F8.c, func_0024E1F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
/* The port (round 11 C26): the members at their host offsets. The N64 view's pads put the flags
 * (0xD4) on m_rpGeometryHeaderSize and the owner (0x1A8, m_AI.m_pParent) inside another CAI
 * member (as in Destructible_GoEnter, func_00265B58.c); the owner is a player, its 0xBB0
 * m_pOnEnemy. */
typedef struct ObjDef { s32 type; } ObjDef;
typedef struct Obj Obj;
typedef struct B11Own {
    u8 pad0[__builtin_offsetof(CPlayer, m_pOnEnemy)];
    Obj *unkBB0;
} B11Own;
struct Obj {
    union {
        struct { u8 type; };
        struct { u8 _hpDef[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; ObjDef *def; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; u32 flags; };
        struct { u8 _hpOwner[__builtin_offsetof(CGameObjectInstance, m_AI.m_pParent)]; B11Own *unk1A8; };
    };
};
#else
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct ObjDef { s32 type; } ObjDef;
typedef struct Obj Obj;
typedef struct B11Own { /* Turok 2: player */
    u8 pad0[0xBB0];
    Obj *unkBB0;        /* Rage Wars: 0x80C */
} B11Own;
struct Obj { /* Turok 2 layout */
    u8 type;
    u8 pad1[0x13];
    ObjDef *def;        /* 0x14 */
    u8 pad18[0xBC];
    u32 flags;          /* 0xD4 */
    u8 padD8[0xD0];
    B11Own *unk1A8;     /* 0x1A8: owner */
};
#endif
s32 Cinema_PathPlaying();
s32 func_00268210(Obj *a) {
    switch (a->def->type) {
    case 11:
        if (a->type == 1 && (a->flags & 0x300000) && (a->unk1A8->unkBB0 != 0 || Cinema_PathPlaying() != 0)) {
            return 1;
        }
        return 0;
    case 1:
    case 5:
        return 1;
    default:
        return 0;
    }
}

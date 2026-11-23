#include "structs/vector.h"
#include "globals/swoosh.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"
#include "structs/word.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205324.c, func_00205324.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct M1Info M1Info;
typedef struct M1Def M1Def;
typedef struct M1Actor M1Actor;
typedef struct M1State M1State;
typedef struct M1Ref M1Ref;
struct M1Info {
                u32 flags;
                CWord unk4;
                s8 b8;
                u8 pad9[0x3];
                s32 unkC;
                u8 pad10[0x8];
                s32 unk18;
                s32 unk1C;
                CWord unk20;
                CWord unk24;
                CWord unk28;
                f32 f2C;
                f32 f30;
                f32 f34;
                u8 pad38[0x4];
                f32 f3C;
                CWord unk40;
                f32 f44;
                CWord unk48;
                CWord unk4C;
                u8 b50[4];
                f32 f54;
                f32 f58;
                f32 f5C;
                f32 f60;
                f32 f64;
                f32 f68;
                u8 pad6C[0x4];
                u8 b70;
                u8 b71;
};
struct M1Def {
                s32 kind;
                u32 unk4;
                u8 pad8[0xC];
                M1Info info;
};
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 C26): the members at their host offsets. With the N64 pads the flags (0xD4)
 * fell 8 bytes late, after the pointer-wide def, on m_rpGeometryHeaderSize: a destroyed object's
 * geometry size lost bit 0x100 (0x160 -> 0x60), and the next model request of that object (the
 * assembly plant of level 122) loaded 0x60 of its 0x2E0 bytes: Model_Decompress read past them
 * and crashed on the decompressor thread. */
struct M1Actor {
    union {
        struct { u8 _hpPos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; CVector3 pos; };
        struct { u8 _hpDef[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; M1Def *def; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; u32 flags; };
    };
};
#else
struct M1Actor { /* Turok 2 layout */
                u8 pad0[0x4];
                CVector3 pos;          /* 0x04 */
                u8 pad10[0x4];
                M1Def *def;         /* 0x14 */
                u8 pad18[0xBC];
                u32 flags;          /* 0xD4 (Rage Wars: 0x100) */
};
#endif
struct M1State {
                u8 pad0[0x4];
                s32 unk4;
                s32 unk8;
                u8 padC[0x28];
                s8 unk34;
                s8 unk35;
                s8 unk36;
                s8 unk37;
                u8 pad38[0x4];
                u32 flags3C;
                f32 f40;
                f32 f44;
                CVector3 v48;
                CVector3 v54;
                f32 f60;
                f32 f64;
                u8 pad68[0x18];
                M1Actor *unk80;
                u8 pad84[0x18];
                f32 f9C;
                CVector3 vA0;
                u8 padAC[0x1E];
                u8 unkCA;
                s8 unkCB;
                u8 padCC[0x1];
                u8 unkCD;
                u8 padCE[0x2E];
                M1Ref *unkFC;
                u8 pad100[0x8];
                void (*fn108)();
                u8 pad10C[0x18];
                CWord unk124;
                CWord unk128;
                CWord unk12C;
                CVector3 vec130;
                f32 f13C;
};
struct M1Ref {
                u8 pad0[0xC];
                s32 unkC;
};
s32 Sound__Play(s16, CVector3, sptr, sptr);
void Actor_SpawnParticle(void *, s32, CVector3, s32); /* the definition (AI) takes three; these callers pass a fourth, 0 */
void AI_DropPickupsOnce(void *, void *, u32, u32, u32);
void SwooshSet_FreeOwner(u8 *p, M1Actor *a);
void Destructible_GoEnter(M1Actor *actor, M1State *state) {
    M1Info *info = &actor->def->info;
    s32 mode;
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 0x40000, actor);
    if (state->unk4 == 0) {
        actor->flags &= ~0x10000;
    }
    if (info->unk18 == 0) {
        actor->flags &= ~0x2000;
        actor->flags &= ~0x100;
    }
    mode = gScene.m_Mode;
    if (mode == 4) {
        if (info->unk20.i != -1) {
            Sound__Play(info->unk20.h[1], actor->pos, 0, -1);
        }
        if (info->unk1C != -1) {
            Actor_SpawnParticle(actor, info->unk1C, actor->pos, 0);
        }
        AI_DropPickupsOnce(actor, state, 4, info->unk24.i, info->unk28.i);
        SwooshSet_FreeOwner(&gEngineApp_m_SwooshSystem, actor);
        if (gScene.m_Mode != mode) goto L;
    } else {
L:
        actor->flags &= ~0x100;
        actor->flags |= 0x8000000;
    }
}

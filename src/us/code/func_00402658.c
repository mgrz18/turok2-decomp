#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Path;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} Arena;

typedef struct {
    /* 0x000 */ u8 pad0[0x214];
    /* 0x214 */ Arena *arena;
    /* 0x218 */ u8 pad218[0x250 - 0x218];
    /* 0x250 */ s32 unk250;
} Boss;

extern Boss D_8012F3A0;
extern s32 D_8012F5F4;

extern void func_00246108(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_0024599C(CGameObjectInstance *actor, CAI *ai, Path *path);
extern void func_00245BAC(CGameObjectInstance *actor, CAI *ai, f32 angle, f32 dist);
extern s32 func_002451AC(CGameObjectInstance *actor, CAI *ai, s32 visible, f32 angle);
extern void func_00243414(CGameObjectInstance *actor, CAI *ai, s32 anim);
extern f32 func_00298470(f32); /* sqrtf */

void func_00402658(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    Path path;
    CVector3 v;
    Arena *arena;
    Boss *boss = &D_8012F3A0;
    f32 turn;
    s32 side;
    s32 visible;

    arena = boss->arena;
    actor->m_dwFlags |= 0x2000;
    turn = 0.17453295f;
    if (boss->unk250) {
        turn = 0.3490659f;
    }
    func_00246108(actor, ai, &t, 1, 1);
    func_0024599C(actor, ai, &path);
    func_00245BAC(actor, ai, turn, t.m_Path.m_Angle);
    visible = t.m_Sight.m_pTarget;
    if (D_8012F5F4 == 0) {
        side = 0;
    } else {
        side = func_002451AC(actor, ai, visible, 0.785398245f);
    }
    if (side == -1) {
        func_00243414(actor, ai, 11);
    } else if (side == 1) {
        func_00243414(actor, ai, 12);
    } else if (boss->unk250) {
        v.x = actor->header.m_vPos.x;
        v.y = 0.0f;
        v.z = actor->header.m_vPos.z;
        if (func_00298470(v.x * v.x + v.z * v.z) < 204.79999f) {
            func_00243414(actor, ai, 4);
        }
    } else if (t.m_Path.m_pTarget && t.m_Sight.m_pTarget) {
        if (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle <= 0.52359885f : t.m_Sight.m_Angle <= 0.52359885f) {
            if (t.m_Path.m_Dist > 1228.7999f && arena->unkC) {
                func_00243414(actor, ai, 9);
            } else if (t.m_Path.m_Dist < 921.6f) {
                func_00243414(actor, ai, 10);
            }
        }
    }
}

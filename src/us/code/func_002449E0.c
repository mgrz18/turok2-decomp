/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00215410.c, func_80215410_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"

/* The player behind an object (owner 0x1A8 of a player-controlled one). */
typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xAAC - 0x998];
    /* 0xAAC */ f32 unkAAC;
    /* 0xAB0 */ u8 padAB0[0xB2C - 0xAB0];
    /* 0xB2C */ s32 unkB2C;
    /* 0xB30 */ u8 padB30[0xB38 - 0xB30];
    /* 0xB38 */ CGameObjectInstance *unkB38;
} OwnerPlayer;


typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 region;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ u16 flags;
} Location;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x34 - 8];
    /* 0x34 */ s8 state;
    /* 0x35 */ u8 pad35[0x68 - 0x35];
    /* 0x68 */ CGameObjectInstance *ignore;
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
    /* 0x78 */ CGameObjectInstance *forced;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ CGameObjectInstance *target;
    /* 0x84 */ u8 pad84[0x88 - 0x84];
    /* 0x88 */ CGameObjectInstance *altTarget;
    /* 0x8C */ u8 pad8C[0x94 - 0x8C];
    /* 0x94 */ s8 location;
    /* 0x95 */ u8 pad95[0x98 - 0x95];
    /* 0x98 */ f32 homeRotY;
    /* 0x9C */ u8 pad9C[0xAC - 0x9C];
    /* 0xAC */ CVector3 pos;
    /* 0xB8 */ u8 padB8[0xCA - 0xB8];
    /* 0xCA */ s8 team;
} Plan;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *target;
    /* 0x08 */ f32 angle;
    /* 0x0C */ CVector3 pos;
    /* 0x18 */ CVector3 delta;
    /* 0x24 */ f32 dist;
    /* 0x28 */ CVector3 flatPos;
    /* 0x34 */ CVector3 flatDelta;
    /* 0x40 */ f32 flatDist;
} TrackResult;

extern s32 D_800F7F78;
extern s32 D_8011001C;

CGameObjectInstance *func_00243C90(CGameObjectInstance *self, Plan *plan, s32 arg2, s32 arg3);
Location *func_0028479C(s8 *location);
f32 func_00246310(CGameObjectInstance *a, CVector3 point);              /* angle to a point */
void func_00210EF0(f32 *angle);                        /* wrap an angle */
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
f32 func_00298470(f32 x);                              /* sqrt */

/* What the plan aims at (the same classes as func_002438C0). */
static __inline__ s32 Kind(CGameObjectInstance *self, Plan *plan, CGameObjectInstance *target) {
    OwnerPlayer *player;

    if (plan->active == 0) {
        return 6;
    }
    if (target == NULL) {
        if (plan->location != 0) {
            return 4;
        }
        return 3;
    }
    if (target == plan->ignore) {
        return 2;
    }
    if (*((s32 *)target->header.m_pIntelligence) == 5) {
        return 5;
    }
    if (target->m_nTypeFlag == 0x64F) {
        return 7;
    }
    if (D_8011001C == 0) {
        if (target->m_dwFlags & 0x300000) {
            player = ((OwnerPlayer *)target->m_AI.m_pParent);
            if (player->unkAAC > 0.0f && (self->m_nTypeFlag != 0xCA || player->weapon != 0x14)) {
                return 1;
            }
            if (player->unkB38 == self && player->unkB2C == 2) {
                return 1;
            }
        }
        if ((self->m_dwHotPointFlags & 2) && self->m_nTypeFlag != 0xCA) {
            return 1;
        }
    }
    return 0;
}

/* Picks the plan's target and fills `out` with its kind, the turn to face
 * it, its position, offset and distance, in 3D and flattened. */
void func_002449E0(CGameObjectInstance *self, Plan *plan, s32 unused, TrackResult *out) {
    CVector3 pos;
    CVector3 delta;
    f32 angle;
    s32 kind;
    CGameObjectInstance *target;
    Location *loc;

    if (plan->team == D_800F7F78 && (!(plan->flags & 0x40000000) || plan->forced != NULL)) {
        plan->target = func_00243C90(self, plan, 1, 0);
    }
    if (*((s32 *)self->header.m_pIntelligence) == 1 && plan->state == 0xB && Kind(self, plan, plan->altTarget) == 4) {
        target = plan->altTarget;
        kind = 4;
    } else {
        target = plan->target;
        kind = Kind(self, plan, target);
    }
    if (target != NULL) {
        pos = target->header.m_vPos;
        angle = func_00246310(self, pos);
    } else {
        switch (kind) {
        case 4:
            loc = func_0028479C(&plan->location);
            pos = loc->pos;
            if (loc->flags & 1) {
                angle = loc->rotY - self->m_RotY;
            } else {
                angle = func_00246310(self, pos);
            }
            break;
        case 3:
            pos = plan->pos;
            angle = plan->homeRotY - self->m_RotY;
            break;
        case 6:
            pos = self->header.m_vPos;
            angle = 0.0f;
            break;
        }
    }
    func_00210EF0(&angle);
    func_0020EF2C(&delta, &pos, &self->header.m_vPos);
    out->kind = kind;
    out->target = target;
    out->angle = angle;
    out->pos = pos;
    out->delta = delta;
    out->dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0;
    out->flatPos = pos;
    out->flatDelta = delta;
    out->flatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

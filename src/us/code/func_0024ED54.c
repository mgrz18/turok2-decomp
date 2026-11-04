#include "common.h"
#include "structs/player.h"

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x00 */ s32 type;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0x140 - 0x24];
    /* 0x140 */ u32 aiFlags;
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x174 - 0x148];
    /* 0x174 */ s8 aiState;
    /* 0x175 */ s8 aiNext;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 yaw;
} Body;

extern Actor *D_800F1CE0;       /* what the player is aiming at */

f32 func_002982F0(f32);         /* sinf */
f32 func_002974C0(f32);         /* cosf */
void func_002811CC(void *camera, f32 a, f32 b, f32 c, f32 d, s32 e, Vec3f pos);
void func_00275544(s16 sound, Vec3f pos, s32, s32);
void func_00243414(Actor *actor, void *ai, s32 state);

/* While riding, button 0x8000 rams the creature in front: a live one is
 * thrown along the heading (knocked out of a state 0x2E with flag 0x400000)
 * and shakes the view hard, anything else just shakes it.
 * The single `return 1` after the if/else (not one per branch) is what gives
 * the ROMs plain bne at the 0x2E test: with two returns, reorgs liveness at
 * the fall-through keeps $v0 and annuls the slot (bnel). */
s32 func_0024ED54(CPlayer *player, Body *body) {
    Actor *aim = D_800F1CE0;
    Actor *target;
    Actor *mount;
    s32 type;
    if ((player->m_ModeFlags & 0x8000) && aim != NULL) {
        mount = player->m_pOnEnemy;
        if (aim != mount && aim->kind == 1) {
            type = aim->def->type;
            if (type == 1 && mount != NULL) {
                target = aim;
                if (target->health != 0) {
                    target->vel.x = player->m_Speed * func_002982F0(body->yaw + 3.1415927f) * 20.0f;
                    target->vel.y = player->m_Speed * 30.0f;
                    target->vel.z = player->m_Speed * func_002974C0(body->yaw + 3.1415927f) * 20.0f;
                    func_002811CC(player->m_pCamera, 50.0f, 100.0f, 50.0f, 1.0f, type, body->pos);
                    if (mount->aiState == 0x2E) {
                        target->aiFlags |= 0x400000;
                        func_00275544(0x278E, target->pos, 0, -1);
                    } else {
                        func_00275544(0x278D, target->pos, 0, -1);
                    }
                    target->aiNext = -1;
                    func_00243414(target, &target->aiFlags, 0x34);
                } else {
                func_002811CC(player->m_pCamera, 30.0f, 60.0f, 30.0f, 1.0f, type, body->pos);
                }
                return 1;
            }
        }
    }
    return 0;
}

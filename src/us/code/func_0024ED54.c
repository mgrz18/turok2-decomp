#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"


typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 yaw;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[0x51C];
    /* 0x051C */ void *camera;
    /* 0x0520 */ u8 pad520[0xA1C - 0x520];
    /* 0x0A1C */ u32 buttons;
    /* 0x0A20 */ u8 padA20[0xA78 - 0xA20];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ u8 padA7C[0xBB0 - 0xA7C];
    /* 0x0BB0 */ CGameObjectInstance *mount;
} Player;

extern CGameObjectInstance *D_800F1CE0;       /* what the player is aiming at */

f32 func_002982F0(f32);         /* sinf */
f32 func_002974C0(f32);         /* cosf */
void func_002811CC(void *camera, f32 a, f32 b, f32 c, f32 d, s32 e, CVector3 pos);
void func_00275544(s16 sound, CVector3 pos, s32, s32);
void func_00243414(CGameObjectInstance *actor, void *ai, s32 state);

/* While riding, button 0x8000 rams the creature in front: a live one is
 * thrown along the heading (knocked out of a state 0x2E with flag 0x400000)
 * and shakes the view hard, anything else just shakes it.
 * The single `return 1` after the if/else (not one per branch) is what gives
 * the ROMs plain bne at the 0x2E test: with two returns, reorgs liveness at
 * the fall-through keeps $v0 and annuls the slot (bnel). */
s32 func_0024ED54(Player *player, Body *body) {
    CGameObjectInstance *aim = D_800F1CE0;
    CGameObjectInstance *target;
    CGameObjectInstance *mount;
    s32 type;
    if ((player->buttons & 0x8000) && aim != NULL) {
        mount = player->mount;
        if (aim != mount && aim->header.m_Type == 1) {
            type = aim->header.m_pIntelligence->m_Type;
            if (type == 1 && mount != NULL) {
                target = aim;
                if (target->m_AI.m_Health != 0) {
                    target->m_vVelocity.x = player->forward * func_002982F0(body->yaw + 3.1415927f) * 20.0f;
                    target->m_vVelocity.y = player->forward * 30.0f;
                    target->m_vVelocity.z = player->forward * func_002974C0(body->yaw + 3.1415927f) * 20.0f;
                    func_002811CC(player->camera, 50.0f, 100.0f, 50.0f, 1.0f, type, body->pos);
                    if (mount->m_AI.m_Mode == 0x2E) {
                        target->m_AI.m_dwFlags |= 0x400000;
                        func_00275544(0x278E, target->header.m_vPos, 0, -1);
                    } else {
                        func_00275544(0x278D, target->header.m_vPos, 0, -1);
                    }
                    target->m_AI.m_ModeLastFrame = -1;
                    func_00243414(target, &target->m_AI.m_dwFlags, 0x34);
                } else {
                func_002811CC(player->camera, 30.0f, 60.0f, 30.0f, 1.0f, type, body->pos);
                }
                return 1;
            }
        }
    }
    return 0;
}

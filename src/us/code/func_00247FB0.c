#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/weapon.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "structs/light.h"
#include "globals/player.h"
#include "protos/game.h"

#ifdef TARGET_PORT
#include "structs/model.h"
/* the draw args (CDrawArgs) at their host offsets (soak S13: with --high-memory the model handle's
 * data pointer was read as an s32 and the player's draw crashed in the attract loop, ~27,000);
 * unkC is m_hModel (N64 0x0C), amount is m_nModel (N64 0x18) */
typedef struct Hit {
    union {
        struct { u8 _hp_unkC[__builtin_offsetof(CDrawArgs, m_hModel)]; s32 *unkC; };
        struct { u8 _hp_amount[__builtin_offsetof(CDrawArgs, m_nModel)]; s32 amount; };
        CDrawArgs _hp_whole;
    };
} Hit;
#else
typedef struct Hit {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 *unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 amount;
} Hit;
#endif

s32 *CISet__GetBlock(sptr arg0, s32 arg1);
s32 CPlayer__IsInMatchEndState(CPlayer *player);
void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *actor, sptr arg1, void *hit);
void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, sptr arg1, void *hit);
void Render_DrawModelShared(s32 *arg0, sptr arg1, s32 arg2, void *arg3, sptr arg4, s32 arg5, s32 arg6);
void Render_QueueModelInstance(s32 *arg0, sptr arg1, s32 arg2, void *arg3, sptr arg4, s32 arg5, s32 arg6);

#ifdef TARGET_PORT
#define HIT_LIMIT(hit) (*CISet__GetBlock(*(sptr *) (hit)->unkC, 2)) /* the handle holds a pointer */
#else
#define HIT_LIMIT(hit) (*CISet__GetBlock(*(hit)->unkC, 2))
#endif
#define WEAPON_INDEX(player) (gWeaponInfoTable[(player)->m_Data.m_CurrentWeapon]->model - 0x5E0)

void Player_DrawNodeCallback(CGameObjectInstance *actor, sptr arg1, Hit *hit) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);
    u8 *flags;
    f32 *color;
    s32 index;

    if (hit->amount >= HIT_LIMIT(hit)) {
        return;
    }
    flags = &gEngineApp_m_Settings.multiplayer;
    if (*flags != 0) {
        if (player->m_pOptions->m_TagMode != 0 && player->m_Data.m_Health == 0) {
            return;
        }
        if (*(s32 *) (flags + (0x1FC - 0x19)) != 0) {
            gBlendState = 3;
            switch (player->m_nDeathmatchTeamSelected) {
                case 0:
                default:
                    color = gBlendState_m_Color;
                    color[0] = 0.0f;
                    color[1] = 0.0f;
                    color[2] = 192.0f;
                    break;
                case 1:
                    color = gBlendState_m_Color;
                    color[1] = 0.0f;
                    color[2] = 0.0f;
                    color[0] = 192.0f;
                    break;
            }
            gBlendState_m_Color[3] = 70.0f;
            gBlendState_m_Color[4] = 255.0f;
            CGameObjectInstance__DrawModelImmediate(actor, arg1, hit);
            if (player->m_ShockedTimer > 0.0f && CPlayer__IsInMatchEndState(player) == 0) {
                Render_DrawModelShared(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, (s8) hit->amount, -1);
            }
            if (player->m_Data.m_nDeathmatchCharacter != 10 && player->m_Data.m_Health != 0) {
                index = WEAPON_INDEX(player);
                if (index >= 0 && index < HIT_LIMIT(hit)) {
                    Render_DrawModel(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, index, actor->m_AI.m_nTexture);
                }
            }
            return;
        }
    }
    if (player->m_ShockedTimer > 0.0f && CPlayer__IsInMatchEndState(player) == 0) {
        CGameObjectInstance__DrawModelImmediate(actor, arg1, hit);
        Render_DrawModelShared(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, (s8) hit->amount, -1);
    } else {
        CGameObjectInstance__DrawModel(actor, arg1, hit);
    }
    if (player->m_Data.m_nDeathmatchCharacter != 10 && player->m_pOptions->m_TagMode == 0 && player->m_Data.m_Health != 0 && player->m_CharacterActor.m_nTypeFlag != gTagModeModel) {
        index = WEAPON_INDEX(player);
        if (index >= 0 && index < HIT_LIMIT(hit)) {
            Render_QueueModelInstance(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, WEAPON_INDEX(player), actor->m_AI.m_nTexture);
        }
    }
}

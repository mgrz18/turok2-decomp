#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/weapon.h"



typedef struct Hit {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 *unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 amount;
} Hit;

extern s32 D_800AB8B0;       /* screen flash mode */
extern f32 D_800AB8B4[3];    /* screen flash color */
extern f32 D_800AB8C0[2];    /* screen flash range */
extern s32 D_800B6D1C;
extern s32 D_800ADB5C;

s32 *func_002017D4(s32 arg0, s32 arg1);
s32 func_002548FC(CPlayer *player);
void func_0021D928(CGameObjectInstance *actor, s32 arg1, void *hit);
void func_0021D7BC(CGameObjectInstance *actor, s32 arg1, void *hit);
void func_0020B258(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0020A33C(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0020B6A0(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);

#define HIT_LIMIT(hit) (*func_002017D4(*(hit)->unkC, 2))
#define WEAPON_INDEX(player) (D_800AF618[(player)->m_Data.m_CurrentWeapon]->model - 0x5E0)

void func_00247FB0(CGameObjectInstance *actor, s32 arg1, Hit *hit) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);
    u8 *flags;
    f32 *color;
    s32 index;

    if (hit->amount >= HIT_LIMIT(hit)) {
        return;
    }
    flags = &D_8011AAD8.multiplayer;
    if (*flags != 0) {
        if (player->m_pOptions->m_TagMode != 0 && player->m_Data.m_Health == 0) {
            return;
        }
        if (*(s32 *) (flags + (0x1FC - 0x19)) != 0) {
            D_800AB8B0 = 3;
            switch (player->m_nDeathmatchTeamSelected) {
                case 0:
                default:
                    color = D_800AB8B4;
                    color[0] = 0.0f;
                    color[1] = 0.0f;
                    color[2] = 192.0f;
                    break;
                case 1:
                    color = D_800AB8B4;
                    color[1] = 0.0f;
                    color[2] = 0.0f;
                    color[0] = 192.0f;
                    break;
            }
            D_800AB8C0[0] = 70.0f;
            D_800AB8C0[1] = 255.0f;
            func_0021D928(actor, arg1, hit);
            if (player->m_ShockedTimer > 0.0f && func_002548FC(player) == 0) {
                func_0020B258(hit->unkC, ((s32)actor->m_pmtDrawMtxs), 1, actor->m_Lights[D_800B6D1C], 0, (s8) hit->amount, -1);
            }
            if (player->m_Data.m_nDeathmatchCharacter != 10 && player->m_Data.m_Health != 0) {
                index = WEAPON_INDEX(player);
                if (index >= 0 && index < HIT_LIMIT(hit)) {
                    func_0020A33C(hit->unkC, ((s32)actor->m_pmtDrawMtxs), 1, actor->m_Lights[D_800B6D1C], 0, index, actor->m_AI.m_nTexture);
                }
            }
            return;
        }
    }
    if (player->m_ShockedTimer > 0.0f && func_002548FC(player) == 0) {
        func_0021D928(actor, arg1, hit);
        func_0020B258(hit->unkC, ((s32)actor->m_pmtDrawMtxs), 1, actor->m_Lights[D_800B6D1C], 0, (s8) hit->amount, -1);
    } else {
        func_0021D7BC(actor, arg1, hit);
    }
    if (player->m_Data.m_nDeathmatchCharacter != 10 && player->m_pOptions->m_TagMode == 0 && player->m_Data.m_Health != 0 && player->m_CharacterActor.m_nTypeFlag != D_800ADB5C) {
        index = WEAPON_INDEX(player);
        if (index >= 0 && index < HIT_LIMIT(hit)) {
            func_0020B6A0(hit->unkC, ((s32)actor->m_pmtDrawMtxs), 1, actor->m_Lights[D_800B6D1C], 0, WEAPON_INDEX(player), actor->m_AI.m_nTexture);
        }
    }
}

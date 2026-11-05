#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

/* Weapon state for weapon 0x14: drains ammo while firing, every third tick. */

extern s32 D_800B6D1C;
extern u32 D_800B6D20;

extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern s32 func_00243414(void *obj, CAI *state, s32 mode);
extern s16 *func_0025398C(CPlayer *player, s32 weapon);
extern void func_00253F40(CPlayer *player);
extern s32 func_00248720(void *player, s32 arg1);

#define USE_AMMO()                                              {                                                               ammo = &player->m_Data.m_AmmoList[*func_0025398C(player, 0x14)];         n = *ammo - 1;                                              if (n < 0) {                                                    n = 0;                                                  }                                                           *ammo = n;                                              }

void func_002584DC(void *obj, CAI *state) {
    CPlayer *player = (CPlayer *) ((CGameObjectInstance *) obj)->m_AI.m_pParent;
    s16 *ammo;
    s32 n;

    if (!(player->m_ContState.m_dwButtons & 0x2000) || func_0024E700(player, 0x14) == 0) {
        if (state->m_ModeTime < 3.0f) {
            USE_AMMO();
        }
        func_00253F40(player);
        func_00243414(obj, state, 2);
        return;
    }
    if (state->m_ModeTime > 30.0f) {
        player->m_FleeTime = 45.0f;
    }
    if (D_800B6D1C != 0) {
        state->m_ConditionTimer += 1.5f;
        func_00248720(((CGameObjectInstance *) obj)->m_AI.m_pParent, 0x14);
    }
    if (D_800B6D20 % 3 == 0) {
        USE_AMMO();
    }
}

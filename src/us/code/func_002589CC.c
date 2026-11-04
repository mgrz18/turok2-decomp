#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern u32 D_800B6D20;
extern s32 D_800F7078;
extern u8 D_801198B0[];

extern void func_0027FC2C(void *arg0, void *camera, s32 model);

/* Same body as func_00258668. Switches the weapon actor to its owner's requested weapon. */
void func_002589CC(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);
    s16 requested = *(s16 *)((u8 *)player + 0xB14);

    if (player->m_Data.m_CurrentWeapon != requested || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = *(s16 *)((u8 *)player + 0xB14);
        state->m_pModeTable = D_800AF618[requested]->modeTable;
        state->m_pfEffects = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_VarBuffer[0] = 0;
        state->m_VarBuffer[1] = 0;
        state->m_VarBuffer[7] = 0;
        state->m_VarBuffer[3] = 0;
    }
}

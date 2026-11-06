#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 MenuPool_OpenMenu(s32, s32, s32, s32, s32);

extern s32 D_00438728;

s32 TeamMenu_ChooseRed_2(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 1;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu((s32) temp_a2->m_pCamera->m_MenuPool, (s32) &D_00438728, (s32) temp_a2, (s32) temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

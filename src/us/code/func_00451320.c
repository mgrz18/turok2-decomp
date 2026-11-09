#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/camera.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenMenu_2(s32, s32, s32, s32, s32);

s32 TeamMenu_ChooseBlue_2(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 0;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu_2((sptr) temp_a2->m_pCamera->m_MenuPool, (sptr) &gCharacterMenuDesc_2, (sptr) temp_a2, (sptr) temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

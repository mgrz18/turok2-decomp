#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 TeamMenu_ChooseBlue(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 0;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu((sptr) temp_a2->m_pCamera + 0x53C, &gCharacterMenuDesc, (sptr) temp_a2, (sptr) temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

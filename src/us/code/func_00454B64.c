#include "common.h"
#include "structs/player_options.h"


void PlayerOptions_SetDefaultName_2(CPlayerOptions *w);

void PlayerOptions_SetDefaults_2(CPlayerOptions *w, u8 arg1) {
    w->m_SettingX = 0x80;
    w->m_SettingY = 0x80;
    w->m_bActive = 0;
    w->m_ControlSide = 0;
    w->m_ControlType = 0;
    w->m_bLookSpring = 1;
    w->m_bAutoAim = 0;
    w->m_nController = arg1;
    w->m_nCharacter = 0;
    w->m_WeaponSelect = 1;
    w->m_Handicap = 0;
    PlayerOptions_SetDefaultName_2(w);
}

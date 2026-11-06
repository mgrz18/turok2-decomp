#include "common.h"
#include "structs/player.h"

void SavedPlayers_Clear(void) {
    D_8011F290.m_bActive = 0;
    D_8011F290.m_State = 0;
    D_8011F290.m_Timer = 0;
}

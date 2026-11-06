#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

s32 SavedPlayers_IsActive(void) {
    return D_8011F290.m_bActive;
}

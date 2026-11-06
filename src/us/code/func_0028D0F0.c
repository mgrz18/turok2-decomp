#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011F290;

void SavedPlayers_Deactivate(void) {
    D_8011F290 = 0;
}

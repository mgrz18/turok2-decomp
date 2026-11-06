#include "common.h"
#include "structs/player.h"

extern u8 D_8011AAF1;

void CPlayer__ResetLives(CPlayer *arg0) {
    if (D_8011AAF1 != 0) {
        arg0->m_Data.m_Lives = 1;
        return;
    }
    arg0->m_Data.m_Lives = 3;
}

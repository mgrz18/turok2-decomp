#include "common.h"
#include "structs/player.h"

extern u8 D_8011AAF1;

void func_00254744(CPlayer *arg0) {
    if (D_8011AAF1 != 0) {
        arg0->m_Data.m_Lives = 1;
        return;
    }
    arg0->m_Data.m_Lives = 3;
}

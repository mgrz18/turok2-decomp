#include "common.h"

void Hud_SetupTextureMode(u8);
void COnScreen__DrawBossBar(void *, s32);
extern u8 D_8011AAEA;

void COnScreen__DrawBossBars(void *arg0) {
    u8 pad[0x100];

    Hud_SetupTextureMode(D_8011AAEA);
    if (*(s32 *)((u8 *)arg0 + 0x40) != 0) {
        COnScreen__DrawBossBar(arg0, 1);
    }
    if (*(s32 *)((u8 *)arg0 + 0x88) != 0) {
        COnScreen__DrawBossBar(arg0, 2);
    }
}

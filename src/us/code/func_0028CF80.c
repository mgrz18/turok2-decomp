#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

/* D_8011F290: the saved copy of the players (see func_0028D264). */
typedef struct {
    /* 0x0 */ s32 active;
    /* 0x4 */ s32 state;
    /* 0x8 */ s32 timer;
    /* 0xC */ s32 warp;
} SavedPlayers;

extern SavedPlayers D_8011F290;


extern u8 D_800F7078[];             /* the world */
extern s32 D_80110048;              /* world +0x18FD0 */

s32 func_0027BC34(void);
void func_0028D264(void);
CPlayer *func_002532A8(void *list);

void func_0028CF80(void) {
    SavedPlayers *save = &D_8011F290;
    s32 ok;
    CPlayer *player;
    GameSettings *settings;

    ok = !func_0027BC34();
    if (D_80119870.m_Settings.mode != 13) {
        ok = 0;
    }
    if (D_80119870.m_PlayerList.m_ActiveList.m_nCount == 0) {
        ok = 0;
    }
    if (D_800F7078 == NULL) {
        ok = 0;
    }
    if (ok) {
        switch (save->state) {
            case 0:
                save->state = 1;
                break;
            case 1:
                save->state = 2;
                break;
            case 2:
            default:
                save->state = 0;
                break;
        }
        func_0028D264();
        settings = &D_80119870.m_Settings;
        save->warp = D_80110048;
        if (settings->multiplayer == 0) {
            player = func_002532A8(&D_80119870.m_PlayerList);
            if (player != NULL) {
                if (save->state == 1) {
                    if (D_80110048 != player->m_Data.m_CheckPointWarpID || (player->m_Mode >= 19 && player->m_Mode < 31)) {
                        save->state = 2;
                    }
                }
                save->warp = player->m_Data.m_CheckPointWarpID;
            }
        }
    } else {
        save->state = 0;
    }
    if (save->state != 0) {
        save->timer = 240;
        save->active = 1;
    } else {
        save->timer = 0;
        save->active = 0;
    }
}

#include "common.h"
#include "structs/player.h"

/* D_8011F290: the saved copy of the players (see func_0028D264). */
typedef struct {
    /* 0x0 */ s32 active;
    /* 0x4 */ s32 state;
    /* 0x8 */ s32 timer;
    /* 0xC */ s32 warp;
} SavedPlayers;

extern SavedPlayers D_8011F290;
/* The settings at D_8011AAD8 (D_80119870 +0x1268). */
typedef struct {
    /* 0x00 */ u8 pad0[0x19];
    /* 0x19 */ u8 unk19;
} Settings;

/* D_80119870: the player list and game state, as this reads it. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x30];
    /* 0x0030 */ s32 unk30;
    /* 0x0034 */ u8 pad34[0x1234];
    /* 0x1268 */ Settings settings;     /* D_8011AAD8 */
    /* 0x1282 */ u8 pad1282[0x196];
    /* 0x1418 */ s32 mode;
} Game;

extern Game D_80119870;
extern u8 D_800F7078[];             /* the world */
extern s32 D_80110048;              /* world +0x18FD0 */

s32 func_0027BC34(void);
void func_0028D264(void);
CPlayer *func_002532A8(void *list);

void func_0028CF80(void) {
    SavedPlayers *save = &D_8011F290;
    s32 ok;
    CPlayer *player;
    Settings *settings;

    ok = !func_0027BC34();
    if (D_80119870.mode != 13) {
        ok = 0;
    }
    if (D_80119870.unk30 == 0) {
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
        settings = &D_80119870.settings;
        save->warp = D_80110048;
        if (settings->unk19 == 0) {
            player = func_002532A8(&D_80119870);
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

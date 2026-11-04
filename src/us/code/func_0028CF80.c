#include "common.h"
#include "structs/player.h"
#include "structs/game.h"


extern u8 D_800F7078[];             /* the world */
extern s32 D_80110048;              /* world +0x18FD0 */

s32 func_0027BC34(void);
void func_0028D264(void);
CPlayer *func_002532A8(void *list);

void func_0028CF80(void) {
    CSavedPlayers *save = &D_8011F290;
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
        switch (save->m_State) {
            case 0:
                save->m_State = 1;
                break;
            case 1:
                save->m_State = 2;
                break;
            case 2:
            default:
                save->m_State = 0;
                break;
        }
        func_0028D264();
        settings = &D_80119870.m_Settings;
        save->m_WarpID = D_80110048;
        if (settings->multiplayer == 0) {
            player = func_002532A8(&D_80119870.m_PlayerList);
            if (player != NULL) {
                if (save->m_State == 1) {
                    if (D_80110048 != player->m_Data.m_CheckPointWarpID || (player->m_Mode >= 19 && player->m_Mode < 31)) {
                        save->m_State = 2;
                    }
                }
                save->m_WarpID = player->m_Data.m_CheckPointWarpID;
            }
        }
    } else {
        save->m_State = 0;
    }
    if (save->m_State != 0) {
        save->m_Timer = 240;
        save->m_bActive = 1;
    } else {
        save->m_Timer = 0;
        save->m_bActive = 0;
    }
}

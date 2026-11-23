#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/save.h"
#include "protos/player.h"



s32 Demo_GetPlayerCount(void);
void SavedPlayers_Save(void);

void SavedPlayers_Arm(void) {
    CSavedPlayers *save = &gSavedPlayers;
    s32 ok;
    CPlayer *player;
    GameSettings *settings;

    ok = !Demo_GetPlayerCount();
    if (gEngineApp_m_Game.m_Settings.mode != 13) {
        ok = 0;
    }
    if (gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_nCount == 0) {
        ok = 0;
    }
    if (&gScene == NULL) {
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
        SavedPlayers_Save();
        settings = &gEngineApp_m_Game.m_Settings;
        save->m_WarpID = gScene_m_LevelID;
        if (settings->multiplayer == 0) {
            player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
            if (player != NULL) {
                if (save->m_State == 1) {
                    if (gScene_m_LevelID != player->m_Data.m_CheckPointWarpID || (player->m_Mode >= 19 && player->m_Mode < 31)) {
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

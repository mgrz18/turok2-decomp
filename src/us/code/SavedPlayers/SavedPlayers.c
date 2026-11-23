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

void SavedPlayers_Clear(void) {
    gSavedPlayers.m_bActive = 0;
    gSavedPlayers.m_State = 0;
    gSavedPlayers.m_Timer = 0;
}

#include "m2c_macros.h"

s32 SavedPlayers_GetWarpID(void) {
    return gSavedPlayers.m_WarpID;
}

s32 SavedPlayers_IsActive(void) {
    return gSavedPlayers.m_bActive;
}

void SavedPlayers_Deactivate(void) {
    gSavedPlayers.m_bActive = 0;
}

#include "structs/vector.h"
#include "protos/scene.h"

/* Restore every player's state saved by SavedPlayers_Save, and in state 1 also
 * the position, facing and region. */
void SavedPlayers_Restore(void) {
    CPlayerPool *list;
    CSavedPlayers *save = &gSavedPlayers;
    s32 i;

    for (i = 0; list = &gEngineApp_m_Game.m_PlayerList, i < list->m_nPlayers; i++) {
        CPlayer *player = &list->m_Players[i];

        player->m_Data = save->m_Data[i];
        if (save->m_State == 1) {
            player->m_CharacterActor.header.m_vPos = save->m_vPos[i];
            player->m_CharacterActor.m_RotY = save->m_RotY[i];
            player->m_CharacterActor.header.m_pCurrentRegion = CScene__GetRegion(&gScene, save->m_Region[i]);
        }
    }
}

void SavedPlayers_UpdateTimer(void) {
    s32 *p = (s32 *) &gSavedPlayers;

    if (p[2] != 0) {
        p[2]--;
        if (p[2] == 0) {
            p[1] = 0;
        }
    }
}

/* Save every player's state, position, facing and region. */
void SavedPlayers_Save(void) {
    CPlayerPool *list;
    CSavedPlayers *save = &gSavedPlayers;
    s32 i;

    for (i = 0; list = &gEngineApp_m_Game.m_PlayerList, i < list->m_nPlayers; i++) {
        CPlayer *player = &list->m_Players[i];

        save->m_Data[i] = player->m_Data;
        save->m_vPos[i] = player->m_CharacterActor.header.m_vPos;
        save->m_RotY[i] = player->m_CharacterActor.m_RotY;
        save->m_Region[i] = CScene__GetRegionIndex(&gScene, player->m_CharacterActor.header.m_pCurrentRegion);
    }
}

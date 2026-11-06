#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/scene.h"

s32 CScene__GetRegion(void *world, s32 region);

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

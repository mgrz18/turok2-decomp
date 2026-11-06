#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"

extern u8 gScene[];             /* the world */
s32 CScene__GetRegionIndex(void *world, s32 region);

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
        save->m_Region[i] = CScene__GetRegionIndex(gScene, player->m_CharacterActor.header.m_pCurrentRegion);
    }
}

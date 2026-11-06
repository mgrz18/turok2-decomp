#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/game.h"

extern u8 D_800F7078[];             /* the world */
s32 CScene__GetRegionIndex(void *world, s32 region);

/* Save every player's state, position, facing and region. */
void SavedPlayers_Save(void) {
    CPlayerList *list;
    CSavedPlayers *save = &D_8011F290;
    s32 i;

    for (i = 0; list = &D_80119870.m_PlayerList, i < list->m_nPlayers; i++) {
        CPlayer *player = &list->m_Players[i];

        save->m_Data[i] = player->m_Data;
        save->m_vPos[i] = player->m_CharacterActor.header.m_vPos;
        save->m_RotY[i] = player->m_CharacterActor.m_RotY;
        save->m_Region[i] = CScene__GetRegionIndex(D_800F7078, player->m_CharacterActor.header.m_pCurrentRegion);
    }
}

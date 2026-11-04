#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/game.h"

extern u8 D_800F7078[];             /* the world */
s32 func_002242F8(void *world, s32 region);

/* Restore every player's state saved by func_0028D264, and in state 1 also
 * the position, facing and region. */
void func_0028D100(void) {
    CPlayerList *list;
    CSavedPlayers *save = &D_8011F290;
    s32 i;

    for (i = 0; list = &D_80119870.m_PlayerList, i < list->m_nPlayers; i++) {
        CPlayer *player = &list->m_Players[i];

        player->m_Data = save->m_Data[i];
        if (save->m_State == 1) {
            player->m_CharacterActor.header.m_vPos = save->m_vPos[i];
            player->m_CharacterActor.m_RotY = save->m_RotY[i];
            player->m_CharacterActor.header.m_pCurrentRegion = func_002242F8(D_800F7078, save->m_Region[i]);
        }
    }
}

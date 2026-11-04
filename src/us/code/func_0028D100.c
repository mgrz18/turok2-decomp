#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"


/* D_8011F290: the saved copy of the players (0x1400 bytes). */
typedef struct {
    /* 0x0000 */ s32 active;
    /* 0x0004 */ s32 mode;
    /* 0x0008 */ s32 timer;
    /* 0x000C */ s32 warp;
    /* 0x0010 */ CPlayerLoadSaveData state[4];
    /* 0x13B0 */ CVector3 pos[4];
    /* 0x13E0 */ f32 rotY[4];
    /* 0x13F0 */ s32 region[4];
} SavedPlayers;

extern CPlayerList D_80119870;
extern SavedPlayers D_8011F290;
extern u8 D_800F7078[];             /* the world */
s32 func_002242F8(void *world, s32 region);

/* Restore every player's state saved by func_0028D264, and in state 1 also
 * the position, facing and region. */
void func_0028D100(void) {
    CPlayerList *list;
    SavedPlayers *save = &D_8011F290;
    s32 i;

    for (i = 0; list = &D_80119870, i < list->m_nPlayers; i++) {
        CPlayer *player = &list->m_Players[i];

        player->m_Data = save->state[i];
        if (save->mode == 1) {
            player->m_CharacterActor.header.m_vPos = save->pos[i];
            player->m_CharacterActor.m_RotY = save->rotY[i];
            player->m_CharacterActor.header.m_pCurrentRegion = func_002242F8(D_800F7078, save->region[i]);
        }
    }
}

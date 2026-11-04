#include "common.h"
#include "structs/player.h"

typedef CVector3 Vec3f;

/* D_80119870: the player list. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ CPlayer *players;
    /* 0x8 */ s32 count;
} PlayerList;

/* D_8011F290: the saved copy of the players (0x1400 bytes). */
typedef struct {
    /* 0x0000 */ s32 unk0[4];
    /* 0x0010 */ CPlayerLoadSaveData state[4];
    /* 0x13B0 */ Vec3f pos[4];
    /* 0x13E0 */ f32 rotY[4];
    /* 0x13F0 */ s32 region[4];
} SavedPlayers;

extern PlayerList D_80119870;
extern SavedPlayers D_8011F290;
extern u8 D_800F7078[];             /* the world */
s32 func_002242B4(void *world, s32 region);

/* Save every player's state, position, facing and region. */
void func_0028D264(void) {
    PlayerList *list;
    SavedPlayers *save = &D_8011F290;
    s32 i;

    for (i = 0; list = &D_80119870, i < list->count; i++) {
        CPlayer *player = &list->players[i];

        save->state[i] = player->m_Data;
        save->pos[i] = player->m_CharacterActor.header.m_vPos;
        save->rotY[i] = player->m_CharacterActor.m_RotY;
        save->region[i] = func_002242B4(D_800F7078, player->m_CharacterActor.header.m_pCurrentRegion);
    }
}

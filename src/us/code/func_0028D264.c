#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { s32 data[0x4E8 / 4]; } PlayerState;

/* A player (0x1328 bytes) as this reads it. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 region;
    /* 0x014 */ u8 pad14[0x3C];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ u8 pad54[0x4CC];
    /* 0x520 */ PlayerState state;
    /* 0xA08 */ u8 padA08[0x920];
} Player;

/* D_80119870: the player list. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ Player *players;
    /* 0x8 */ s32 count;
} PlayerList;

/* D_8011F290: the saved copy of the players (0x1400 bytes). */
typedef struct {
    /* 0x0000 */ s32 unk0[4];
    /* 0x0010 */ PlayerState state[4];
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
        Player *player = &list->players[i];

        save->state[i] = player->state;
        save->pos[i] = player->pos;
        save->rotY[i] = player->rotY;
        save->region[i] = func_002242B4(D_800F7078, player->region);
    }
}

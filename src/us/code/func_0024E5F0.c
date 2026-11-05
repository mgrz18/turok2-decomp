#include "common.h"
#include "structs/player.h"

typedef struct PlayerStateInfo {
    /* 0x00 */ void (*enter)(CPlayer *player, void *arg);
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u32 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
} PlayerStateInfo;

extern PlayerStateInfo D_800ADF54[];
extern u8 D_8011AAF1;

s32 func_002548FC(CPlayer *player);

/* Switches the player to `state` (only to states 16-18 while
 * func_002548FC holds in D_8011AAF1's mode): runs its enter hook and takes
 * its flags. Returns whether the player ends up in that state. */
s32 func_0024E5F0(CPlayer *player, void *arg, s32 state) {
    s16 last;
    u32 lastFlags;

    if (D_8011AAF1 != 0 && func_002548FC(player) != 0 && state != 18 && state != 16 && state != 17) {
        return 0;
    }
    last = player->m_Mode;
    lastFlags = player->m_ModeFlags;
    player->m_Mode = state;
    player->m_ModeBefore = last;
    player->m_ModeBeforeFlags = lastFlags;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    if (D_800ADF54[state].enter != NULL) {
        D_800ADF54[state].enter(player, arg);
    }
    player->m_ModeFlags = D_800ADF54[player->m_Mode].flags;
    if (D_800ADF54[player->m_Mode].unk10 != 0) {
        player->m_CharAnim = D_800ADF54[player->m_Mode].unk10;
    }
    if (player->m_Mode == state) {
        player->m_ModeTime = 0.0f;
        return 1;
    }
    return 0;
}

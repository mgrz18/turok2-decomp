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

extern PlayerStateInfo gPlayerStateTable[];
extern u8 gEngineApp_m_Settings_multiplayer;

s32 CPlayer__IsInMatchEndState(CPlayer *player);

/* Switches the player to `state` (only to states 16-18 while
 * CPlayer__IsInMatchEndState holds in gEngineApp_m_Settings_multiplayer's mode): runs its enter hook and takes
 * its flags. Returns whether the player ends up in that state. */
s32 CPlayer__SetState(CPlayer *player, void *arg, s32 state) {
    s16 last;
    u32 lastFlags;

    if (gEngineApp_m_Settings_multiplayer != 0 && CPlayer__IsInMatchEndState(player) != 0 && state != 18 && state != 16 && state != 17) {
        return 0;
    }
    last = player->m_Mode;
    lastFlags = player->m_ModeFlags;
    player->m_Mode = state;
    player->m_ModeBefore = last;
    player->m_ModeBeforeFlags = lastFlags;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    if (gPlayerStateTable[state].enter != NULL) {
        gPlayerStateTable[state].enter(player, arg);
    }
    player->m_ModeFlags = gPlayerStateTable[player->m_Mode].flags;
    if (gPlayerStateTable[player->m_Mode].unk10 != 0) {
        player->m_CharAnim = gPlayerStateTable[player->m_Mode].unk10;
    }
    if (player->m_Mode == state) {
        player->m_ModeTime = 0.0f;
        return 1;
    }
    return 0;
}

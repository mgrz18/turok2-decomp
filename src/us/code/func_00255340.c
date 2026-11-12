#include "common.h"
#include "structs/player.h"
#include "protos/player.h"


#define PLAYER_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define PLAYER_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

/* Puts the player in mode 4 when touching an actor whose intelligence has
 * flag 2 (and AI flags 0x6000 clear), if player flag 0x10 at 0xA68 is set. */
#ifdef TARGET_PORT
/* The host body on the members (round 12 a5, tools/port/n64viewcheck.py): 0xB8C, 0xA9C and 0xA68
 * are N64 offsets of CPlayer (m_SniperMode, m_TimeOffGround, m_ContState.m_dwButtonsSingle). */
s32 CPlayer__CheckJumpOn(CPlayer *player, CGameObjectInstance *actor) {
    if (player->m_SniperMode != 0) {
        return 0;
    }
    if (player->m_TimeOffGround > 11.25f) {
        return 0;
    }
    if (actor->m_AI.m_dwFlags & 0x6000) {
        return 0;
    }
    if (!(actor->header.m_pIntelligence->m_dwTypeFlags & 2)) {
        return 0;
    }
    if (!(player->m_ContState.m_dwButtonsSingle & 0x10)) {
        return 0;
    }
    CPlayer__SetState(player, actor, 4);
    return 1;
}
#else
s32 CPlayer__CheckJumpOn(CPlayer *player, CGameObjectInstance *actor) {
    if (PLAYER_S32(player, 0xB8C) != 0) {
        return 0;
    }
    if (PLAYER_F32(player, 0xA9C) > 11.25f) {
        return 0;
    }
    if (actor->m_AI.m_dwFlags & 0x6000) {
        return 0;
    }
    if (!(actor->header.m_pIntelligence->m_dwTypeFlags & 2)) {
        return 0;
    }
    if (!(PLAYER_S32(player, 0xA68) & 0x10)) {
        return 0;
    }
    CPlayer__SetState(player, actor, 4);
    return 1;
}
#endif

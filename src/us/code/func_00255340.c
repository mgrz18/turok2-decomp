#include "common.h"
#include "structs/player.h"

extern void func_0024E5F0(CPlayer *player, CGameObjectInstance *other, s32 mode);

#define PLAYER_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define PLAYER_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Intelligence;

/* Puts the player in mode 4 when touching an actor whose intelligence has
 * flag 2 (and AI flags 0x6000 clear), if player flag 0x10 at 0xA68 is set. */
s32 func_00255340(CPlayer *player, CGameObjectInstance *actor) {
    if (PLAYER_S32(player, 0xB8C) != 0) {
        return 0;
    }
    if (PLAYER_F32(player, 0xA9C) > 11.25f) {
        return 0;
    }
    if (actor->m_AI.m_dwFlags & 0x6000) {
        return 0;
    }
    if (!(((Intelligence *)actor->header.m_pIntelligence)->flags & 2)) {
        return 0;
    }
    if (!(PLAYER_S32(player, 0xA68) & 0x10)) {
        return 0;
    }
    func_0024E5F0(player, actor, 4);
    return 1;
}

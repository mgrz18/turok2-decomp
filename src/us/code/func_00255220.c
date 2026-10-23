#include "common.h"
#include "structs/player.h"

extern void func_0024E5F0(CPlayer *player, CGameObjectInstance *other, s32 mode);

/* s16 player mode at 0xA08 (LibTEngine's CPlayer has m_Mode at 0xA0C). */
#define PLAYER_MODE(p) (*(s16 *)((u8 *)(p) + 0xA08))

static __inline__ s32 InMode10To12(CPlayer *player) {
    s32 mode = PLAYER_MODE(player);

    if (mode == 0x12 || mode == 0x10 || mode == 0x11) {
        return 1;
    }
    return 0;
}

/* Puts the player in mode 0xC when hit by an actor with AI flag 0x800,
 * unless already in modes 0xC, 0xD or 0x10-0x12. */
s32 func_00255220(CPlayer *player, CGameObjectInstance *actor) {
    if (InMode10To12(player)) {
        return 0;
    }
    if (actor->m_AI.m_dwFlags & 0x800) {
        if (PLAYER_MODE(player) == 0xC) {
            return 0;
        }
        if (PLAYER_MODE(player) == 0xD) {
            return 0;
        }
        func_0024E5F0(player, actor, 0xC);
        return 1;
    }
    return 0;
}

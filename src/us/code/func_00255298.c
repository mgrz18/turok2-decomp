#include "common.h"
#include "structs/player.h"

extern void func_0024E5F0(CPlayer *player, CGameObjectInstance *other, s32 mode);

/* s16 player mode at 0xA08. */
#define PLAYER_MODE(p) (*(s16 *)((u8 *)(p) + 0xA08))
#define PLAYER_FLAGS_A1C(p) (*(s32 *)((u8 *)(p) + 0xA1C))

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Intelligence;

static __inline__ s32 InMode10To12(CPlayer *player) {
    s32 mode = PLAYER_MODE(player);

    if (mode == 0x12 || mode == 0x10 || mode == 0x11) {
        return 1;
    }
    return 0;
}

/* Puts the player in mode 0xE when touching an actor whose intelligence has
 * flag 1 and whose AI has flag 0x1000 (and not 0x80). */
s32 func_00255298(CPlayer *player, CGameObjectInstance *actor) {
    if (InMode10To12(player)) {
        return 0;
    }
    if (((Intelligence *)actor->header.m_pIntelligence)->flags & 1) {
        if (PLAYER_MODE(player) == 0x23) {
            return 0;
        }
        if (PLAYER_FLAGS_A1C(player) & 0x8000) {
            return 0;
        }
        if (actor->m_AI.m_dwFlags & 0x80) {
            return 0;
        }
        if (PLAYER_MODE(player) == 0xE) {
            return 0;
        }
        if (!(actor->m_AI.m_dwFlags & 0x1000)) {
            return 0;
        }
        func_0024E5F0(player, actor, 0xE);
        return 1;
    }
    return 0;
}

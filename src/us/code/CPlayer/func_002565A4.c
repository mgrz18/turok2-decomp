#include "common.h"
#include "structs/player.h"

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#endif

void CPlayer__ResetHeadSway(CPlayer *player) {
    P_S32(player, 0xABC) = 0;
    P_F32(player, 0xAB4) = P_F32(player, 0xAB0);
    P_F32(player, 0xAB8) = 0.4014257788658142f; /* 23 degrees */
}

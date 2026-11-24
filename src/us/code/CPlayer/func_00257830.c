#include "common.h"
#include "globals/strings.h"

typedef struct {
    /* 0x00 */ s32 *block;
    /* 0x04 */ s32 players;
    /* 0x08 */ s32 count;
} PlayerPool;

void *memset(void *, s32, uptr);

/* Allocates and clears count player records (0x1328 bytes each). */
void CPlayerPool__AllocPlayers(PlayerPool *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s0;

    temp_s0 = arg1 * 0x1328;
    arg0->count = arg1;
    temp_v0 = CCache__Allocate(0, temp_s0, 0x23, &sCPlayerPoolCacheDesc_Players);
    arg0->block = temp_v0;
    temp_a0 = *temp_v0;
    arg0->players = temp_a0;
    memset(temp_a0, 0, temp_s0);
}

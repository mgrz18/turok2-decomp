#include "common.h"

/* Pool of 0x1328-byte entries (4904 bytes each) starting at base. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u32 base;
} Pool0025340C;

#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/player_list.h"
/* Host: the player array pointer-wide, and the host CPlayer's size (the N64 view read the array
 * as a u32 and divided by the N64 size 0x1328: a kill in high memory or a PIE indexed
 * m_KilledBy far out of range, STABILITY.md 7.2 S22). */
uptr CPlayerPool__GetIndex(Pool0025340C *arg0, uptr arg1) {
    return (arg1 - (uptr) ((CPlayerPool *) arg0)->m_Players) / sizeof(CPlayer);
}
#else
uptr CPlayerPool__GetIndex(Pool0025340C *arg0, uptr arg1) {
    return (arg1 - arg0->base) / 4904;
}
#endif

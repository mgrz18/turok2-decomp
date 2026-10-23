#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

/* Menu state: the current player pointer is the word at 0x20 (D_80119890). */
extern u8 D_80119870[];
extern char D_800C1E54[];

extern void func_00416644(void *a, char *text, CPlayer *player, s32 d, void *e);

void func_00255E4C(CPlayer *player) {
    CPlayer *current = *(CPlayer **)(D_80119870 + 0x20);

    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    if (player == current) {
        func_00416644(D_80119870 + 0x5BC, D_800C1E54, player, P_S32(player, 0xA50), NULL);
    }
}

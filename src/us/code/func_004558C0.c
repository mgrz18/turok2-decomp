#include "common.h"

/* Reset the current player state and open a menu page on the player's controller. */

typedef struct {
    u8 pad0[0x1C];
    u8 *player; /* 0x1C: CPlayer */
} PlayerEvent;

extern u8 D_80119870[];
extern u8 D_0043B5EC[];

void func_0025340C(void *state, void *player);
void func_00421170(void);
void func_00416644(void *menu, void *page, void *player, void *controller, s32 flags);

s32 func_004558C0(s32 arg0, PlayerEvent *event) {
    u8 *player = event->player;

    func_0025340C(D_80119870, player);
    func_00421170();
    func_00416644(D_80119870 + 0x5BC, D_0043B5EC, player, *(void **)(player + 0xA50), 0);
    return 1;
}

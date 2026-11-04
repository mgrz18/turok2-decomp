/* Second VM module copy of func_0042E9A4 (same source, linked 0x0042E8C4). */
#include "common.h"
#include "structs/player.h"

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x288];
    /* 0x0288 */ u8 unk288[0x3C8 - 0x288];
    /* 0x03C8 */ u8 unk3C8[0x518 - 0x3C8];
    /* 0x0518 */ CPlayerOptions *options;
} Player;

extern s32 D_8011ACA8;

extern void func_0042DB38(Player *p, s32 arg1, s32 arg2);
extern void func_00243414(void *, void *, s32);
extern void func_00248BA8(Player *p, s32 warp, s32 arg2, s32 respawn);

void func_004678C4(Player *p, s32 arg1) {
    p->options->m_bActive = 1;
    func_0042DB38(p, arg1, 1);
    func_00243414(p->unk288, p->unk3C8, 2);
    func_00248BA8(p, D_8011ACA8, 0, 1);
}

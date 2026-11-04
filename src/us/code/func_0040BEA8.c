#include "common.h"
#include "structs/game.h"

/* Calls func_002535E4 on the players with 900, then queues func_0040C7DC. */

extern void func_002535E4(void *players, f32 value);
extern void func_0040C7DC(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

void func_0040BEA8(void) {
    func_002535E4(&D_80119870.m_PlayerList, 900.0f);
    func_00283FF8(0xE6, func_0040C7DC, NULL);
}

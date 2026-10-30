/* WRONG CUT: the function starts at 0x0042C758 (versions/cuts.us.txt calls its two first words,
 * the prologue addiu sp,-24 / sw s0, a stray; the body at 0x42C760 saves ra at 20(sp) and pops 24).
 * REAL func_0042C758 size 0x44 at func_0042C700+0x58 */
#include "common.h"

typedef struct LevelLights {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 flags;
} LevelLights;

typedef struct MenuState {
    /* 0x00000 */ u8 pad0[0x18E50];
    /* 0x18E50 */ LevelLights *lights;
} MenuState;

extern s32 D_800AB8A0;

extern void func_0042AA80(MenuState *menu);

void func_0042C758(MenuState *menu) {
    func_0042AA80(menu);
    D_800AB8A0 = (menu->lights->flags >> 2) & 1;
}

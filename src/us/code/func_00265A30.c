/* Boundary fixed in versions/cuts.us.txt: splat's func_00265954 (0x12C bytes) holds two functions:
 *   0x00265954  func_00265954, the animation check (0xD0 bytes), then a nop;
 *   0x00265A28  two stray words (beq $18, $16, ...; sll $2, $18, 3), never run;
 *   0x00265A30  func_00265A30, an init callback (0x50 bytes; its pointer is in
 *               the data as 0x00265A30).
 * The check is the same as the inlined r01_check of func_002653F8 and
 * func_002651FC, here out of line. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0020486C.c, r01_check, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xB8];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ s16 unkBA;
    /* 0x0BC */ u8 padBC[0x18];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ f32 animFrame;
    /* 0x0DC */ s16 anim;
    /* 0x0DE */ s16 nextAnim;
    /* 0x0E0 */ s16 animLength;
    /* 0x0E2 */ u8 padE2[0x1E];
    /* 0x100 */ s16 blendLength;
    /* 0x102 */ u8 pad102[6];
    /* 0x108 */ f32 blendFrame;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void *table;
    /* 0x30 */ u8 pad30[0x96];
    /* 0xC6 */ u8 anim;
    /* 0xC7 */ s8 active;
    /* 0xC8 */ u8 padC8[0x28];
    /* 0xF0 */ void (*update)(Actor *, void *);
    /* 0xF4 */ u8 padF4[4];
    /* 0xF8 */ void (*onEvent)(Actor *, void *);
} ActionState;

extern u8 D_800B2330[];
extern void func_00265AF8(Actor *actor, void *state);
extern void func_00265EEC(Actor *actor, void *state);
extern void func_004088B0(void);

void func_00265A30(Actor *actor, ActionState *state) {
    state->table = D_800B2330;
    state->update = func_00265AF8;
    state->onEvent = func_00265EEC;
    if (actor->typeId == 0x64D) {
        func_004088B0();
    }
}

#include "common.h"

/* The player (func_002532A8(&D_80119870)); include/structs/player.h does not
 * line up here (its m_Mode is at 0xA0C, the code has the state at 0xA08), so
 * only the flag set here is named. */
typedef struct {
    /* 0x000 */ u8 pad0[0x9EC];
    /* 0x9EC */ s32 unk9EC; /* next to unk9F0 (func_00440D6C) */
} Player;

/* The record at D_80119870 + 0x1268: flags at 4. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 flags;
} PlayerState1268;

s32 func_00243414(s32, s32, s32);
s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(s32 *);                     /* extern */
extern s32 D_80119870;
extern s32 D_8012FB70;

void func_004457DC(void) {
    Player *temp_v0;
    PlayerState1268 *temp_v1;

    func_00243414(D_8012FB70, D_8012FB70 + 0x140, 1);
    temp_v0 = func_002532A8(&D_80119870);
    if (temp_v0 != NULL) {
        temp_v1 = (PlayerState1268 *) ((s8 *) &D_80119870 + 0x1268);
        temp_v0->unk9EC = 1;
        temp_v1->flags |= 4;
        func_00248BA8((s32) temp_v0, 0x1B57, 2, 0);
    }
}

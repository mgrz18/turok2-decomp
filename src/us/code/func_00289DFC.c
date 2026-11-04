#include "common.h"

s32 func_002759C4(s32);
s32 func_00284080(void);
s32 func_00285410(s32);
s32 func_0042F514(void);

s32 func_00275904(s32);                     /* extern */
s32 func_00275A08();                            /* extern */
extern s32 D_800B6D78;
extern s32 D_8011B110;

/* The frame/game state block passed to the per-mode handlers (the
 * handler table D_800B6D78/D_800B6D7C has 0xC-byte rows indexed by mode). */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FD0];
    /* 0x23FD0 */ s32 unk23FD0;
    /* 0x23FD4 */ s32 prevMode;
    /* 0x23FD8 */ s32 mode;
    /* 0x23FDC */ s32 nextMode;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[0x23FE4 - 0x23FE2];
    /* 0x23FE4 */ f32 fadeTime;
    /* 0x23FE8 */ u8 pad23FE8[0x23FF0 - 0x23FE8];
    /* 0x23FF0 */ s32 unk23FF0;
} GameState;

void func_00289DFC(GameState *arg0) {
    s32 (*temp_v0)(GameState *);
    s32 temp_v1;

    if ((D_8011B110 == 2) && (arg0->unk23FF0 == 0)) {
        arg0->unk23FD0 = 0;
        arg0->mode = arg0->nextMode;
        func_00284080();
        func_00275904(-1);
        temp_v1 = arg0->nextMode;
        if ((temp_v1 != 8) && (temp_v1 != 0xC)) {
            if ((arg0->prevMode != 8) || (arg0->mode != 0xD)) {
                func_002759C4(-1);
                func_00275A08();
                goto block_7;
            }
        } else {
block_7:
            if (arg0->mode != 0xD) {
                func_0042F514();
            }
        }
        func_00285410(0);
        temp_v0 = *(((__typeof__(&D_800B6D78))((s8 *)&D_800B6D78 + (arg0->mode * 0xC))));
        if (temp_v0 != NULL) {
            temp_v0(arg0);
        }
    }
}

#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xCC];
    /* 0x0CC */ s32 unkCC;
    /* 0x0D0 */ u8 padD0[0x224 - 0xD0];
} Controller;

extern Controller D_800F5EC0[4]; /* controllers */
extern s32 D_80130910[4];   /* saved per-controller words */
extern u8 D_800F5A50[];
extern void func_00285304(void);
extern void func_0026EDA8(void *);

/* Restores the per-controller words saved by func_0041A254. */
void func_0041A32C(void) {
    s32 i;

    func_00285304();
    for (i = 0; i < 4; i++) {
        D_800F5EC0[i].unkCC = D_80130910[i];
    }
    func_0026EDA8(D_800F5A50);
}

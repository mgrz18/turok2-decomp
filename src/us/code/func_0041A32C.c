#include "common.h"
#include "structs/controller.h"


extern s32 D_80130910[4];   /* saved per-controller words */
extern u8 D_800F5A50[];
extern void func_00285304(void);
extern void func_0026EDA8(void *);

/* Restores the per-controller words saved by func_0041A254. */
void func_0041A32C(void) {
    s32 i;

    func_00285304();
    for (i = 0; i < 4; i++) {
        D_800F5EC0[i].rumbleOn = D_80130910[i];
    }
    func_0026EDA8(D_800F5A50);
}

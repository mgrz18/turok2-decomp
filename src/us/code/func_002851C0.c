/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_8.c, func_80264104_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0026ECF0(void *arg0);
extern int D_80110028;
extern u32 func_00298E10(struct Triple *);
extern float D_800A9CF8;
extern f32 func_0026E760(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 D_800F5A50;
extern s32 D_8011ACA4;
extern void func_0026EDA8(s32 *);
extern unsigned int func_00298FD4(void *arg0);
void func_002851C0(ControllerProfile *arg0) {
    Triple zero;
    void *object;
    s32 *global;
    f32 first;
    if (arg0->rumblePak != 0) {
        zero.x = 0;
        zero.y = 0;
        zero.z = 0;
        if (arg0->rumbleOn == 0) {
            goto reset;
        }
        global = &D_8011ACA4;
        if ((*global != 0) || (D_80110028 != 0) || (global[-6] != 13)) {
reset:
            arg0->rumbleLevel = 0.0f;
            func_0026EDA8(arg0->rumblePool);
        } else {
            object = arg0->rumblePool;
            func_0026ECF0(object);
            first = func_0026E760(&D_800F5A50, arg0->unk8, arg0->unkC, arg0->unk10);
            arg0->rumbleLevel = first + func_0026E760(object, zero.x, zero.y, zero.z);
        }
        if (arg0->rumblePak != 0) {
            if (arg0->rumbleLevel != 0.0f) {
                arg0->rumbleTimer += arg0->rumbleLevel;
                if (arg0->rumbleTimer >= D_800A9CF8) {
                    arg0->rumbleTimer -= D_800A9CF8;
func_00298FD4
((void *)arg0->rumblePfs);
                    return;
                }
            }
((unsigned int (*)(void *))func_00298E10)
((void *)arg0->rumblePfs);
        }
    }
}

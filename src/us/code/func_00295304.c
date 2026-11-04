/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_7.c, func_802B43A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

typedef s32 (*FuncPtr)(void *);
extern void func_00296340(void *arg0, void *a, void *b, s32 c);
extern s32 func_002912A0(s32, s32, void *, s32, s32);
extern void func_0029F190(void);
extern char func_0029FB64;
void func_00295304(void *arg0, FuncPtr arg1, s32 arg2) {
    func_00296340(arg0, (void *) func_0029F190, &func_0029FB64, 0);
    ((ALLoadFilter *)arg0)->state = func_002912A0(0, 0, arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->lstate = func_002912A0(0, 0, arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->dma = arg1(&((ALLoadFilter *)arg0)->dmaState);
    ((ALLoadFilter *)arg0)->lastsam = 0;
    ((ALLoadFilter *)arg0)->first = 1;
    ((ALLoadFilter *)arg0)->memin = 0;
}

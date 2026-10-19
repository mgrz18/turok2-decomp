/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254F9C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 D_800D81D0;
extern int D_800D8210[];
extern s32 D_800D8DA4[];
extern s32 D_800D8DD0;
extern int D_800AB0C0;
extern void osSendMesg(void *arg0, s32 arg1, s32 arg2);
void func_002067C4(s32 unused, s32 arg1) {
    s32 mask;
    u32 count;
    s32 new_var;
    u32 new_var2;
    s32 offset;
    u32 limit;
    if (D_800D8210[arg1] != 0) {
        D_800D8210[arg1] = 0;
        osSendMesg(&D_800D8DD0, 0, 1);
    }
    mask = -0x201;
    if (arg1 != 0) {
        mask = -0x401;
    }
    do { count = 0; limit = D_800AB0C0; } while (0);
    new_var2 = limit;
    if (limit != 0) {
        offset = count;
        do {
            new_var = offset + D_800D81D0;
            count += 1;
            *(s32 *)&((Node80254C10 *)new_var)->flags &= mask;
            offset += 0x28;
        } while (count < new_var2);
    }
    do { D_800D8DA4[arg1] = 0; } while (0);
}

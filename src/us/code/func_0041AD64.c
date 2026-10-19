/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043CDF8_de.c, func_8043CDF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_80110060;
extern CheatCode D_00439770[];
extern u8 D_800C1F8C[];
extern struct IntPair D_8011AAD8;
extern s32 D_80130920;
extern s32 func_0029E4C0(u8 *);
extern void func_00275624(s32);
s32 func_0041AD64(void)
{
    u32 i;
    s32 j;
    s32 match;
    s32 hasSound;
    struct IntPair *settings;
    u8 *code;
    D_80130920 = -1;
    for (i = 0; i < 12; i++) {
        code = D_00439770[i].code;
        j = 0;
        if (func_0029E4C0(code) != func_0029E4C0(D_800C1F8C)) {
            match = 0;
        } else {
            for (;;) {
                if (j < func_0029E4C0(code)) {
                    if ((code[j] ^ j) != D_800C1F8C[j]) {
                        match = 0;
                        break;
                    }
                    j++;
                } else {
                    match = 1;
                    break;
                }
            }
        }
        if (match) {
            D_8011AAD8.field_4 |= D_00439770[i].flags;
            settings = &D_8011AAD8;
            do {
                D_80110060 = settings->field_4;
            } while (0);
            hasSound = D_00439770[i].sound != 0;
            D_80130920 = i;
            if (hasSound) {
                func_00275624((s16)D_00439770[i].sound);
            }
        }
    }
    return 0;
}

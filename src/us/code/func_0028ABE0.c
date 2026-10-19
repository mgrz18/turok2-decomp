/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F544_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_0028ABE0(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void *func_0029A6E0();
extern s32 D_8011B098;
extern s32 D_8011B0A0;
s32 func_0028ABE0(Ctx_func_8028F544_de *arg0, Node_func_8028F544_de **arg1, Node_func_8028F544_de **arg2, s32 arg3) {
    Node_func_8028F544_de *temp_s0;
    Node_func_8028F544_de *temp_v1_0;
    Node_func_8028F544_de *temp_v0;
    Node_func_8028F544_de *var_v1;
    s32 temp_s5;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_s1;
    var_s1 = arg3;
    temp_s0 = arg0->unk2E8;
    temp_v1_0 = arg0->unk2E4;
    temp_s5 = var_s1;
    if ((arg0->unk300 != 0) && (var_s1 & 2)) {
        if ((temp_s0 == 0) || !(temp_s0->unk8 & 0x10)) {
            *arg1 = temp_v1_0;
            arg0->unk300 = 0;
            temp_v0 = arg0->unk2E4->unk0;
            var_s1 &= ~2;
            arg0->unk2E4 = temp_v0;
            if (temp_v0 == 0) {
                arg0->unk2EC = 0;
            }
        } else {
            *arg1 = temp_s0;
            var_s1 &= ~2;
        }
    } else {
        if (temp_s0 == 0) {
            var_v1 = 0;
        } else if (temp_s0->unkC == D_8011B0A0) {
            var_v1 = 0;
        } else if (temp_s0->unkC == func_0029A6E0()) {
            var_v1 = 0;
        } else if (temp_s0->unkC == D_8011B098) {
            var_v1 = 0;
        } else {
            var_v1 = temp_s0;
        }
        if (var_v1 != 0) {
            temp_v0_2 = temp_s0->unk8 & 7;
            switch (temp_v0_2) {
            case 1:
            case 4:
            case 5:
                break;
            case 3:
                if (temp_s0->unk4 & 0x20) {
                    if (var_s1 & 2) {
                        *arg1 = temp_s0;
                        var_s1 &= ~2;
                        if (temp_s0->unk4 & 1) {
                            *arg2 = temp_s0;
                            var_s1 &= ~1;
                        }
                        arg0->unk2E8 = arg0->unk2E8->unk0;
                        if (arg0->unk2E8 == 0) {
                            arg0->unk2F0 = 0;
                        }
                    }
                } else if (var_s1 == 3) {
                    *arg2 = temp_s0;
                    *arg1 = temp_s0;
                    arg0->unk2E8 = arg0->unk2E8->unk0;
                    var_s1 = 0;
                    if (arg0->unk2E8 == 0) {
                        arg0->unk2F0 = 0;
                    }
                }
                break;
            case 2:
            case 6:
            case 7:
                temp_v1 = temp_s0->unk4;
                if (temp_v1 & 2) {
                    if (var_s1 & 2) {
                        *arg1 = temp_s0;
                        var_s1 &= ~2;
                    }
                } else if ((temp_v1 & 1) && (var_s1 & 1)) {
                    *arg2 = temp_s0;
                    arg0->unk2E8 = arg0->unk2E8->unk0;
                    var_s1 &= ~1;
                    if (arg0->unk2E8 == 0) {
                        arg0->unk2F0 = 0;
                    }
                }
                break;
            }
        }
    }
    if (var_s1 != temp_s5) {
        var_s1 = func_0028ABE0(arg0, arg1, arg2, var_s1);
    }
    return var_s1;
}

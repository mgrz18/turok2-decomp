/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_2.CPP, func_8040B700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 func_0041DD90(Menu *action, s32 channel, s32 arg2);
extern s32 func_0041DD90(Menu *, s32, s32);
extern s32 D_8013099C;
extern s32 func_0028591C(s32);
extern s32 func_0041DD90(struct Menu *, s32, s32);
static __inline__ s32 func_80409BDC_de(struct Menu *record) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return record->controller->port;
}
s32 func_00423478(struct Menu *record) {
    if (D_8013099C == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (func_0028591C(func_80409BDC_de(record)) != 0) {
                if (func_0041DD90(record, func_80409BDC_de(record), 0) != 0) {
                    D_8013099C = 1;
                }
            }
        }
    }
    return D_8013099C;
}

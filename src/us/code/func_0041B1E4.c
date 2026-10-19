/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043D278_de_6.c, func_8043D278_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 D_8011AAD8;
extern s32 D_8011AADC;
extern s32 D_800B57AC;
extern s32 D_800B57C8;
s32 func_0041B1E4(struct Func8043D458Arg *arg0) {
    if (D_8011AADC & 1) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (D_8011AAD8 & 1) {
        arg0->text = &D_800B57AC;
    } else {
        arg0->text = &D_800B57C8;
    }
    return 0;
}
extern s32 D_8011AAD8;

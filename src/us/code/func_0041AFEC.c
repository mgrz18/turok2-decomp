/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043D080_de.c, func_8043D080_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
struct Func8043D458Arg;
struct Func8043D458Arg {
    s8 pad0[8];
    s32 flags;
    s8 padC[8];
    s32 *text;
};
extern s32 D_8011AAD8;
extern s32 D_8011AADC;
s32 func_0041AFEC(struct Func8043D458Arg *arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    if (D_8011AADC & arg1) arg0->flags |= 0x01000000;
    else arg0->flags &= 0xFEFFFFFF;
    if (D_8011AAD8 & arg1) arg0->text = arg2;
    else arg0->text = arg3;
    return 0;
}

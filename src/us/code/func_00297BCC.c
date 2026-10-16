/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_21.c, func_802B6C6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
extern float D_800AA7C8;
void func_00297BCC(u8 *arg0) {
    u8 *diag;
    u8 *elem;
    s32 r;
    s32 c;
    f32 one;
    r = 0;
    one = D_800AA7C8;
    diag = arg0;
    do {
        c = 0;
        elem = arg0;
        do {
            if (r == c) {
                *(f32 *)diag = one;
            } else {
                *(s32 *)elem = 0;
            }
            c += 1;
            elem += 4;
        } while (c < 4);
        diag += 0x14;
        r += 1;
        arg0 += 0x10;
    } while (r < 4);
}

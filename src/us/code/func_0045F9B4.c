/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00405454.c, func_00405454.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
extern s32 D_80130C60[];
s32 func_00425D94(s32 pad, s32 idx, s32 *valid, u8 *name, u8 *ext, s32 *blocks, u8 *company, u8 *game);
s32 func_0045F9B4(s32 pad, u8 *ext) {
    s32 mask = 0;
    s32 i;
    u8 company[3];
    u8 game[5];
    u8 name[16];
    u16 t;
    s32 valid;
    s32 blocks;
    if (D_80130C60[pad] != 3) {
        return -2;
    }
    for (i = 0; i < 16; i++) {
        if (func_00425D94(pad, i, &valid, name, ext, &blocks, company, game) != 0) {
            return -1;
        }
        if (valid != 0) {
            mask |= 1 << (ext[0] - 0x41);
        }
    }
    i = 0;
    ext[0] = 0x41;
    ext[1] = 0;
    ext[2] = 0;
    ext[3] = 0;
    while (i < 16) {
        t = mask & (1 << i);
        if (t == 0) {
            break;
        }
        ext[0]++;
        i++;
    }
    if (i >= 16) {
        return -1;
    }
    return 0;
}

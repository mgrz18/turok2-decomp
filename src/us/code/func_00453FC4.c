/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043C730.c, func_0043D2D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u8 pad0[8]; u32 f8; u8 padC[8]; void *f14; } Q4Z;
extern s32 D_80130924;
extern u8 D_800C1144[];
extern u8 D_800B5B2C[], D_800B5B48[], D_800B5B64[], D_800B5B80[], D_800B5B9C[], D_800B5BB8[], D_800B5BD4[], D_800B5BF0[], D_800B5C0C[], D_800B5C28[], D_800B5C44[];
s32 CheatMenu_WarpText_2(Q4Z *o) {
    switch (D_80130924) {
    default: o->f14 = D_800C1144; break;
    case 0: o->f14 = D_800B5B2C; break;
    case 1: o->f14 = D_800B5B48; break;
    case 2: o->f14 = D_800B5B64; break;
    case 3: o->f14 = D_800B5B80; break;
    case 4: o->f14 = D_800B5B9C; break;
    case 5: o->f14 = D_800B5BB8; break;
    case 6: o->f14 = D_800B5BF0; break;
    case 7: o->f14 = D_800B5BD4; break;
    case 8: o->f14 = D_800B5C0C; break;
    case 9: o->f14 = D_800B5C28; break;
    case 10: o->f14 = D_800B5C44; break;
    }
    return 0;
}

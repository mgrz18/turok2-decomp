/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028DC18.c, func_0028DC18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
/* Turok 2 level/game state: only the fields used here (offsets differ from Rage Wars). */
typedef struct Level {
    u8 pad0[0x64];
    s32 *unk64; /* 0x64 */
    u8 pad68[0x4];
    s32 *unk6C; /* 0x6C */
    s32 *unk70; /* 0x70 */
    u8 pad74[0x18FA8 - 0x74];
    s32 unk18FA8; /* 0x18FA8 */
} Level;
s32 *func_002017D4(s32 *tbl, s32 idx);
void func_00201818(s32 *tbl, s32 idx);
s32 func_00226928(Level *lv, s32 sel, s32 c, s32 b, s32 bit) {
    s32 *t;
    s32 *base;
    u8 *bits;
    s32 m;
    if (sel != 0) {
        base = lv->unk6C;
    } else {
        base = lv->unk70;
    }
    t = func_002017D4(func_002017D4(func_002017D4(base, 0), b), c);
    func_002017D4(t, 0);
    func_00201818(t, 1);
    bits = (u8 *)func_002017D4(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}

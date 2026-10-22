/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BAF8.c, func_0028BAF8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Level { /* Turok 2 layout */
    u8 pad0[0x6C];
    s32 *unk6C;         /* save-data table (Rage Wars: 0x80) */
    u8 pad70[0x18FA8 - 0x70];
    s32 unk18FA8;       /* current slot index (Rage Wars: 0x1B40C) */
} Level;
typedef struct { u8 pad0[0x13]; u8 unk13; } Src;
s32 *func_002017D4(s32 *tbl, s32 idx);
void func_00201818(s32 *tbl, s32 idx);
void func_00224A68(Level *lv, Src *s, s32 set) {
    s32 bit = s->unk13;
    s32 idx = lv->unk18FA8;
    s32 *t = func_002017D4(func_002017D4(func_002017D4(lv->unk6C, 0), idx), 2);
    u8 *bits;
    u8 *b2;
    s32 m;
    func_002017D4(t, 0);
    func_00201818(t, 1);
    bits = (u8 *)func_002017D4(t, 1);
    b2 = bits;
    m = 1 << (bit & 7);
    if (set != 0) {
        bits[bit / 8] |= m;
    } else {
        b2[bit / 8] &= ~m;
    }
}

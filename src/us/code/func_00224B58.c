/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BBE8.c, func_0028BBE8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

/* Turok 2 layout (Rage Wars: levels at 0x80, level at 0x1B40C). */
typedef struct Level {
    u8 pad0[0x6C];
    s32 *levels;            /* 0x6C */
    u8 pad70[0x18FA8 - 0x70];
    s32 level;              /* 0x18FA8 */
} Level;
typedef struct { u8 pad0[0x13]; u8 unk13; } Src;
s32 *func_002017D4(s32 *tbl, s32 idx);
void func_00201818(s32 *tbl, s32 idx);
s32 func_00224B58(Level *lv, Src *s) {
    s32 bit = s->unk13;
    s32 idx = lv->level;
    s32 *t = func_002017D4(func_002017D4(func_002017D4(lv->levels, 0), idx), 2);
    u8 *bits;
    s32 m;
    func_002017D4(t, 0);
    func_00201818(t, 1);
    bits = (u8 *)func_002017D4(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}

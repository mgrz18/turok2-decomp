/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B964.c, func_0028B964.
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
typedef struct { u8 pad0[0x16E]; u16 unk16E; } Src;
s32 *func_002017D4(s32 *tbl, s32 idx);
void func_00201818(s32 *tbl, s32 idx);
s32 func_002248D4(Level *lv, Src *s) {
    s32 bit = s->unk16E;
    s32 idx = lv->unk18FA8;
    s32 *t = func_002017D4(func_002017D4(func_002017D4(lv->unk6C, 0), idx), 1);
    u8 *bits;
    s32 m;
    func_002017D4(t, 0);
    func_00201818(t, 1);
    bits = (u8 *)func_002017D4(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}

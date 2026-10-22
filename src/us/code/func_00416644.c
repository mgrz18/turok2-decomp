/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004426E4.c, func_004426E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { s16 unk0; u8 pad[0x22]; } O06I;
typedef struct { O06I *unk0; s16 unk4; } O06B;
typedef struct { u8 pad[0x14]; s16 unk14; } O06A;
/* Extra allocation per item: kind 3 items carry a 0x330-byte block (0x480 in Rage Wars). */
static __inline__ s32 O06_ItemSize(s16 *a) {
    s32 n = 0;
    if (*a == 3) n = 0x330;
    return n;
}
extern s32 *func_00204EDC();
extern void func_00414EA8();
extern void CList__AddHead(O06A *a, s32 b);
extern u8 D_800C0C50[];
s32 func_00416644(O06A *a, O06B *b, s32 c, s32 d, s32 e) {
    s32 size, i;
    s32 *r;
    s32 s;
    size = b->unk4 * 40 + 0x1D8;
    for (i = 0; i < b->unk4; i++) size += O06_ItemSize(&b->unk0[i].unk0);
    r = func_00204EDC(0, size, 0x3B, D_800C0C50 + 4);
    if (r == 0) return 0;
    s = *r;
    if (s == 0) return 0;
    func_00414EA8(s, r, b, 0, c, d, e, a);
    CList__AddHead(a, s);
    { s16 t = a->unk14 + 1; a->unk14 = t; if (t >= 4) a->unk14 = 0; }
    return s;
}

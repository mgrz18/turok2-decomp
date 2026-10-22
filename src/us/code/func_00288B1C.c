/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293378.c, func_00293378.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct B21Task {
               s32 unk0;
               s32 unk4;
               s32 unk8;
               s32 unkC;
               s32 unk10;
               s32 unk14;
               s32 unk18;
               s32 unk1C;
               s32 unk20;
               s32 unk24;
               s32 unk28;
               s32 unk2C;
               s32 unk30;
               s32 unk34;
               s32 unk38;
               s32 unk3C;
               s32 unk40;
               s32 unk44;
               s32 unk48;
               s32 unk4C;
               s32 unk50;
               s32 unk54;
               u8 pad58[0x68 - 0x58];
} B21Task;
typedef struct B21Slot {
                u8 pad0[0x20];
                B21Task task[2];
                u8 unkF0[0x20];
                s32 unk110;
                s32 unk114;
                s32 unk118;
                s32 unk11C;
                s32 unk120;
                u8 pad124[4];
                s32 unk128;
                u8 pad12C[0x140 - 0x12C];
} B21Slot;
typedef struct B21Game { /* Turok 2 layout */
    u8 pad0[0x23FE4];
    f32 unk23FE4;       /* fade timer (Rage Wars: 0x26DC4) */
} B21Game;
extern f32 D_800A9DD8;
extern f32 D_800A9DDC;
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_002883E0(B21Game *g, u8 *col, s32 a, s32 b, s32 c, s32 d);
void func_00288B1C(B21Game *g) {
    u8 col[4];
    f32 t;
    s32 v;
    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = g->unk23FE4;
    if (t < 0.0f) goto c255;
    if (D_800A9DD8 < t) { v = 0; goto done; }
    if (t < 0.0f) { c255: v = 255; } else { v = ~(s32)(t * D_800A9DDC); }
    done:;
    col[3] = v;
    func_002883E0(g, col, 0, 0, D_800C2030, D_800C2034);
}

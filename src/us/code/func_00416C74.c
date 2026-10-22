/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00442D18.c, func_00442D18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct { u32 w0; u32 w1; } O06Gfx;
typedef struct { u8 pad[0x288]; f32 unk288; f32 unk28C; f32 unk290; f32 unk294; } O06V; /* fields at 0x288 in Turok 2 (0x29C in Rage Wars) */
typedef struct { u8 pad[0x40]; O06V *unk40; u8 pad2[0x184]; s32 unk1C8; s32 unk1CC; } O06A;
extern void func_00277F6C(void);
extern void func_00207FEC(s32 a);
extern void func_0027B320(s32 a, s32 b, s16 c, s16 d, f32 e, f32 f, s32 g);
extern O06Gfx *D_800D8E24;
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_00416C74(O06A *a) {
    s32 v, t;
    f32 x, y;
    O06Gfx *g;
    if (a->unk1CC == 0) return;
    t = a->unk1C8;
    v = (t < 0x18) ? t : 0x2F - t;
    func_00277F6C();
    func_00207FEC(0x15);
    x = a->unk40->unk288;
    y = a->unk40->unk28C;
    g = D_800D8E24++;
    g->w0 = 0xFB000000; g->w1 = 0xFF0000FF;
    g = D_800D8E24++;
    g->w0 = 0xFA000000; g->w1 = 0xE00000FF;
    func_0027B320(0x67, v, (s32)a->unk40->unk290, (s32)a->unk40->unk294,
                  x / (f32)D_800C2030 * 5.0f,
                  y / (f32)D_800C2034 * 4.0f, 1);
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040293C.c, func_0040293C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct O01Dl { u32 w0; u32 w1; } O01Dl;
typedef struct O01CtxG {
               u8 pad0[0x1C];
               f32 unk1C;
               u8 pad20[0x10];
               f32 unk30;
               f32 unk34;
               u8 pad38[0x6C];
               f32 unkA4;
} O01CtxG;
extern O01CtxG *D_800C1BB0;
extern s32 D_800C2030;
extern s32 D_800C2034;
extern const f32 D_800C0A6C;
extern const f32 D_800C0A70;
extern const f32 D_800C0A74;
extern const f32 D_800C0A78;
extern const f32 D_800C0A7C;
extern O01Dl *D_800D8E24;
s32 func_00207FEC(s32 a);
s32 func_00207A70(s32 a);
void func_004110CC(s32 a);
void func_0044AC94(void) {
    f32 fs;
    f32 k;
    f32 q;
    s32 sh;
    fs = ((f32)D_800C2034 - (f32)D_800C2030 / D_800C1BB0->unkA4) * D_800C0A70;
    q = D_800C0A74;
    k = D_800C0A6C;
    if (D_800C1BB0->unk1C < D_800C1BB0->unk30) {
        k = D_800C1BB0->unk30 - D_800C1BB0->unk1C;
        k = k / q;
        if (D_800C0A78 < k) {
            k = D_800C0A78;
        }
        k = D_800C0A78 - k;
    }
    if (D_800C1BB0->unk1C > D_800C1BB0->unk34) {
        k = D_800C1BB0->unk1C - D_800C1BB0->unk34;
        k = k / q;
        if (D_800C0A7C < k) {
            k = D_800C0A7C;
        }
        k = D_800C0A7C - k;
    }
    fs = fs * k;
    { O01Dl *_g = D_800D8E24++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { O01Dl *_g = D_800D8E24++; _g->w0 = (0xE3000A01); _g->w1 = (0); }
    func_00207FEC(0x13);
    func_00207A70(0x19);
    { O01Dl *_g = D_800D8E24++; _g->w0 = (0xFA00FFFF); _g->w1 = (0xFF); }
    { O01Dl *_g = D_800D8E24++; _g->w0 = (0xF6000000 | ((((D_800C2030) > (0) ? (D_800C2030) : (0)) & 0x3FF) << 14) | (((((s32)fs) > (0) ? ((s32)fs) : (0)) & 0x3FF) << 2)); _g->w1 = (0); }
    { O01Dl *_g = D_800D8E24++; _g->w0 = (0xF6000000 | ((((D_800C2030) > (0) ? (D_800C2030) : (0)) & 0x3FF) << 14) | ((((D_800C2034) > (0) ? (D_800C2034) : (0)) & 0x3FF) << 2)); _g->w1 = (({ f32 _y = (f32)D_800C2034 - fs; if (!(0.0f <= _y)) _y = 0.0f; (((u32)_y) & 0x3FF) << 2; })); }
    sh = (s32)fs;
    func_004110CC(sh);
}

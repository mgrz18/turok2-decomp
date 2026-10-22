/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00441D50.c, func_00441D50.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Q04Item {
    s32 idx;
    s16 w4;
    u8 pad6[2];
    u32 flags;
    u16 wC;
    u16 wE;
    u8 b10, b11, b12, b13;
    s32 f14;
    void *def;
    s32 f1C;
    void *p20;
    void *owner;
} Q04Item;
typedef struct Q04Info {
    s32 f0, f4, f8;
    f32 fC, f10;
    s32 x0, x1, y0, y1;
} Q04Info;
typedef struct { u8 pad0[2]; s16 unk2; s16 unk4; u8 pad6[6]; Q04Item *unkC; s32 unk10; u8 pad14[0x30 - 0x14]; f32 unk30; u8 pad34[0x40 - 0x34]; void *unk40; } Q04W;
typedef struct { u8 pad[0x288]; f32 unk288; f32 unk28C; f32 unk290; f32 unk294; } Q04V; /* fields at 0x288 in Turok 2 (0x29C in Rage Wars) */
typedef struct { u8 pad[8]; u32 unk8; } Q04B;
extern void func_0041363C(Q04Item *it, void *info);
extern void func_0027AF4C(void);
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_00415CE8(Q04W *a, Q04B *b, s32 c) {
    struct { s32 f, w, h, g; } sz;
    struct { s32 f, x0, x1, y0, y1; } r;
    Q04Info info;
    Q04V *cam;
    s32 i, v, v2, v3, v4;
    f32 fu; register f32 base asm("$f1"); f32 half, t, y0, k;
    u32 al;
    cam = a->unk40;
    switch (b->unk8 & 0x3FE0) {
    case 0x20: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    case 0x40: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    case 0x200: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    default: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    }
    info.f4 = 0; info.f8 = 0; info.x0 = 0; info.x1 = 0; info.y0 = 0; info.y1 = 0;
    for (i = 0; i < a->unk10; i++) {
        func_0041363C(a->unkC + i, &info);
        v = info.x0; if (r.x0 < v) v = r.x0; r.x0 = v;
        v2 = info.x1; if (v2 < r.x1) v2 = r.x1; r.x1 = v2;
        v3 = info.y0; if (r.y0 < v3) v3 = r.y0; r.y0 = v3;
        v4 = info.y1; if (v4 < r.y1) v4 = r.y1; r.y1 = v4;
    }
    sz.w = r.x1 - r.x0;
    sz.h = r.y1 - r.y0;
    if (sz.w >= 0 && sz.h >= 0) {
        base = cam->unk290 + cam->unk288 * 0.5f + (f32)a->unk2;
        sz.w = D_800C2030;
        half = (f32)(D_800C2030 / 2);
        r.x0 = (s32)(base - half);
        r.x1 = (s32)(base + half);
        y0 = (f32)D_800C2034;
        t = y0 * 0.21621622f;
        t -= (D_800C2034 >= 0xDF) ? 13.0f : 9.0f;
        if (c != 0) y0 = (cam->unk294 + (f32)D_800C2034 + (f32)a->unk4) - t;
        else { register f32 pa asm("$f0"); f32 pb; pa = cam->unk294; pb = (f32)a->unk4; y0 = pa + pb; }
        r.y0 = (s32)y0;
        r.y1 = (s32)((f32)r.y0 + t);
        sz.h = r.y1 - r.y0;
        func_0027AF4C();
        func_002778A8(r.x0, r.y0, r.x1, r.y1, 1, 0, 0, 0, 0, (u32)(a->unk30 * 192.0f) & 0xFF);
    }
}

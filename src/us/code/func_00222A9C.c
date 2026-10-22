/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028994C.c, func_0028994C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct B19bV3f { f32 x, y, z; } B19bV3f;
typedef struct B19bG8 { u32 flags; u8 pad4[0x8]; u32 fC; } B19bG8;
extern B19bG8 D_8011AAD8[];
typedef struct B19bLObj {
              u8 pad0[4];
              f32 f4;
              B19bV3f pos;
               u8 pad14[0xA4];
               u8 fB8;
               u8 padB9;
               u8 fBA;
} B19bLObj;
void func_0020F85C(void *, void *, void *);
s32 func_002123D4(void *, B19bV3f, void *, f32 *);
void func_0020EEF8(void *, void *, void *);
void func_0020F9D4(void *, void *, void *);
void func_0020EF2C(void *, void *, void *);
void func_0020F040(void *);
void func_0020EF60(void *, void *, f32);
f32 func_00213E90(void *);
void func_00222A9C(B19bLObj **ents, s32 n, f32 *pos, u8 *out) {
    f32 w[128];
    B19bV3f dirs[128];
    B19bV3f t0, a, b, c, d, e, f, g, h, i, j;
    B19bLObj *last;
    B19bLObj *o;
    f32 x2, u0, u1, x, scale, s0, s1, s2;
    s32 k, v;
    f32 *wp, *w1, *wb, *pw;
    B19bV3f *dd;
    f32 zero;
    B19bV3f *db;
    B19bV3f *dp;
    B19bLObj **ep;
    f32 m1_0, m2_0;
    f32 m1_1, m2_1;
    f32 m1_2, m2_2;
    f32 m1_3, m2_3;
    f32 m1_4, m2_4;
    f32 m1_5, m2_5;
    scale = (f32)D_8011AAD8->fC * 0.003921569f;
    last = 0;
    if (D_8011AAD8->flags & 0x4000) {
        out[0] = out[4] = 0;
        out[1] = out[5] = 0;
        out[2] = out[6] = 0;
        out[8] = out[0xC] = 0;
        out[9] = out[0xD] = 0;
        out[0xA] = out[0xE] = 0;
        out[0x10] = 0x7F;
        out[0x11] = 0;
        out[0x12] = 0;
        return;
    }
    k = 0;
    a.x = a.y = a.z = 0.0f;
    c.x = c.y = c.z = 0.0f;
    d.x = d.y = d.z = 0.0f;
    e.x = e.y = e.z = 0.0f;
    if (n > 0) {
        wb = w;
        db = dirs;
        do {
            o = ents[k];
            func_0020F85C((u8 *)o + 0x20, pos, &t0);
            if (func_002123D4(o, t0, &g, &wb[k]) != 0) {
                func_0020EEF8(&h, &g, &t0);
                func_0020F9D4((u8 *)o + 0x60, &h, &g);
                func_0020EF2C((&db[k]), pos, &g);
                func_0020F040((&db[k]));
                b = o->pos;
                if (o->fBA != 0) {
                    i.z = i.y = i.x = func_00213E90(o);
                    func_0020EF60(&f, &i, wb[k] * o->f4);
                    last = o;
                    if (o->fB8 != 0) func_0020EF60(&f, &f, -1.0f);
                    func_0020EEF8(&c, &c, &f);
                }
                u1 = o->f4 * 0.5f;
                u0 = 1.0f - u1;
                func_0020EF60(&f, &b, wb[k] * u1);
                if (o->fB8 != 0) func_0020EF60(&f, &f, -1.0f);
                func_0020EEF8(&c, &c, &f);
                wb[k] *= u0;
                x = b.x;
                if (x < 0.0f) x = -x;
                if (b.y < 0.0f) x2 = x - b.y; else x2 = x + b.y;
                if (b.z < 0.0f) u0 = x2 - b.z; else u0 = x2 + b.z;
                func_0020EF60(&f, (&db[k]), wb[k]);
                func_0020EF60(&f, &f, u0);
                func_0020EEF8(&a, &a, &f);
                if (o->fB8 != 0) wb[k] = -wb[k];
            } else {
                (&db[k])->x = 0.0f;
                (&db[k])->y = 0;
                (&db[k])->z = 0;
                wb[k] = 0.0f;
            }
                        k++;
        } while (k < n);
    }
    func_0020F040(&a);
    do {
        k = 0;
    } while (0);
    if (n > 0) {
        wp = w;
        s1 = 1.0f;
        zero = 0.0f;
        do {
            o = ents[k];
            pw = &wp[k];
            s2 = a.x * ((B19bV3f *)(wp + 128))[k].x + a.y * ((B19bV3f *)(wp + 128))[k].y + a.z * ((B19bV3f *)(wp + 128))[k].z;
            s0 = s1 - s2;
            if (s2 < zero) s0 = s1;
            s0 = s0 * 0.5f;
            b = o->pos;
            s2 = s1 - s0;
            if (o == last) {
                i.z = i.y = i.x = func_00213E90(o);
                func_0020EF60(&j, &i, s2 * *pw);
                func_0020EEF8(&e, &e, &j);
                func_0020EF60(&j, &i, s0 * *pw);
                func_0020EEF8(&d, &d, &j);
            }
            func_0020EF60(&f, &b, s2 * *pw);
            func_0020EEF8(&e, &e, &f);
            func_0020EF60(&f, &b, s0 * *pw);
            func_0020EEF8(&d, &d, &f);
            k++;
        } while (k < n);
    }
    func_0020EF60(&d, &d, 0.5f);
    func_0020EEF8(&c, &c, &d);
    if (c.x < 0.0f) e.x += c.x;
    if (c.y < 0.0f) e.y += c.y;
    if (c.z < 0.0f) e.z += c.z;
    m1_0 = c.x;
    if (!(m1_0 <= 255.0f)) m1_0 = 255.0f;
    m2_0 = m1_0;
    if (!(m2_0 >= 5.0f)) m2_0 = 5.0f;
    c.x = m2_0 * (1.0f - scale) + scale * 255.0f;
    m1_1 = c.y;
    if (!(m1_1 <= 255.0f)) m1_1 = 255.0f;
    m2_1 = m1_1;
    if (!(m2_1 >= 5.0f)) m2_1 = 5.0f;
    c.y = m2_1 * (1.0f - scale) + scale * 255.0f;
    m1_2 = c.z;
    if (!(m1_2 <= 255.0f)) m1_2 = 255.0f;
    m2_2 = m1_2;
    if (!(m2_2 >= 5.0f)) m2_2 = 5.0f;
    c.z = m2_2 * (1.0f - scale) + scale * 255.0f;
    m1_3 = e.x;
    if (!(m1_3 <= 255.0f)) m1_3 = 255.0f;
    m2_3 = m1_3;
    if (!(m2_3 >= 5.0f)) m2_3 = 5.0f;
    e.x = m2_3 * (1.0f - scale) + scale * 255.0f;
    m1_4 = e.y;
    if (!(m1_4 <= 255.0f)) m1_4 = 255.0f;
    m2_4 = m1_4;
    if (!(m2_4 >= 5.0f)) m2_4 = 5.0f;
    e.y = m2_4 * (1.0f - scale) + scale * 255.0f;
    m1_5 = e.z;
    if (!(m1_5 <= 255.0f)) m1_5 = 255.0f;
    m2_5 = m1_5;
    if (!(m2_5 >= 5.0f)) m2_5 = 5.0f;
    e.z = m2_5 * (1.0f - scale) + scale * 255.0f;
    v = (u32)c.x;
    out[0] = out[4] = v;
    v = (u32)c.y;
    out[1] = out[5] = v;
    v = (u32)c.z;
    out[2] = out[6] = v;
    v = (u32)e.x;
    out[8] = out[0xC] = v;
    v = (u32)e.y;
    out[9] = out[0xD] = v;
    v = (u32)e.z;
    out[0xA] = out[0xE] = v;
    out[0x10] = (s32)(a.x * 127.0f);
    out[0x11] = (s32)(a.y * 127.0f);
    out[0x12] = (s32)(a.z * 127.0f);
}

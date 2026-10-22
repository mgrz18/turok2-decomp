/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023B708.c, func_0023B708.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct B09EDef {
               s16 id;
               u8 pad2[6];
               f32 f8;
               u8 padC[4];
               f32 f10;
               f32 f14;
               u8 pad18[4];
               f32 f1C;
               f32 f20;
               u8 pad24[5];
               u8 flags29;
} B09EDef;
typedef struct B09Emitter B09Emitter;
struct B09Emitter {
                u8 pad0[4];
                B09Emitter *next;
                u8 pad8[4];
                B09EDef *def;
                u16 mask;
                u8 pad12[2];
                u8 pad14[8];
                f32 f1C;
                f32 f20;
                u8 pad24[0x1E4];
                f32 f208;
                f32 f20C;
                f32 key;
                f32 f214;
                f32 f218;
                f32 f21C;
                f32 f220;
};
extern f32 D_800B6D28;
extern f32 D_800A9CB0;
extern f32 D_800B6D34;
extern f32 D_800A9CB4;
extern const f32 D_800A9CB8;
extern const f32 D_800A9CBC;
s32 func_00283550(B09Emitter *e, s32 mask, f32 f2, s32 flag, f32 f4, u8 *ctx) {
    B09EDef *def = e->def;
    f32 x;
    f32 step;
    f32 t;
    if (e->mask & mask) {
        f32 up = e->f208;
        up += def->f20 * D_800B6D28;
        if (D_800A9CB0 < up) {
            up = D_800A9CB0;
        }
        e->f208 = up;
    } else {
        f32 dn;
        if (e->f208 == 0.0f) {
            return 1;
        }
        dn = e->f208 - def->f20 * D_800B6D28;
        dn = (dn < 0.0f) ? 0.0f : dn;
        e->f208 = dn;
    }
    e->f20C = e->f20C + D_800B6D28 * def->f10 * e->f214;
    step = D_800B6D28 * D_800B6D34 * def->f8;
    t = e->f1C + step * D_800A9CB4;
    e->f1C = t;
    if (D_800A9CB8 < t) {
        e->f1C = t - D_800A9CB8;
    }
    t = e->f20 + step * D_800A9CBC;
    e->f20 = t;
    if (D_800A9CB8 < t) {
        e->f20 = t - D_800A9CB8;
    }
    x = def->f14 + e->f21C;
    if (flag != 0 && *(s32 *)(ctx + 0x24) == 0) {
        x += f4;
    } else {
        u8 b = def->flags29;
        if (b & 1) {
            x += *(f32 *)(ctx + 0x118) * def->f1C;
        } else if (b & 2) {
            x += f2;
        }
    }
    e->key = x;
    return 0;
}

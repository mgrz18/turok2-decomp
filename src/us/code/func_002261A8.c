/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D268.c, func_0028D268.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

/* Turok 2 layout: point table at 0x68 (0x7C in Rage Wars), 0x1C-byte records. */
typedef struct Level {
    u8 pad0[0x68];
    s32 *points;            /* 0x68 */
} Level;
void func_0020EF2C(Vec3f *out, Vec3f *a, u8 *b);
void *func_002017D4();
s32 func_002261A8(Level *lv, s32 unused, Vec3f *pt) {
    Vec3f d;
    f32 bestIdx = -1.0f;
    f32 bestD = 0.0f;
    s32 i = 0;
    s32 *t;
    s32 n;
    u8 *p;
    t = func_002017D4(func_002017D4(lv->points), 1);
    n = t[1];
    p = (u8 *)(t + 2);
    if (n > 0) {
        do {
            f32 dd;
            func_0020EF2C(&d, pt, p);
            dd = d.x * d.x + d.y * d.y + d.z * d.z;
            if (bestIdx < 0.0f || dd < bestD) {
                bestIdx = (f32)i;
                bestD = dd;
            }
            i++;
            p += 0x1C; /* 0x38 in Rage Wars */
        } while (i < n);
    }
    return (s32)bestIdx;
}

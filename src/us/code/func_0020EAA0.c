/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00271B18.c, func_00271B18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
f32 func_00298470(f32 x);
f32 func_0021153C(f32 x);
f32 func_0020EAA0(Vec3f *v) {
    f32 x = v->x;
    f32 z = v->z;
    f32 len, r, res, t;
    len = x * x + z * z;
    if (len == 0.0f) {
        return 0.0f;
    }
    len = func_00298470(len);
    if (len == 0.0f) {
        return 0.0f;
    }
    if (((x) < 0.0f ? -(x) : (x)) < ((z) < 0.0f ? -(z) : (z))) {
        x /= len;
        r = func_0021153C(((-1.0f) > (((1.0f) < (x) ? (1.0f) : (x))) ? (-1.0f) : (((1.0f) < (x) ? (1.0f) : (x)))));
        res = -r;
        if (z < 0.0f) {
            res = r;
        }
        res -= 1.5707964f;
    } else {
        z /= len;
        t = ((-1.0f) > (((1.0f) < (z) ? (1.0f) : (z))) ? (-1.0f) : (((1.0f) < (z) ? (1.0f) : (z))));
        r = func_0021153C(-t);
        res = -r;
        if (x < 0.0f) {
            res = r;
        }
    }
    return res;
}

typedef float f32;

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Point on segment a-b at height y. */
void func_0020F0C8(Vec3f *out, Vec3f *a, Vec3f *b, f32 y) {
    f32 dy = b->y - a->y;
    f32 t;

    if (dy == 0.0f) {
        *out = *a;
        return;
    }
    t = (y - a->y) / dy;
    out->x = a->x + t * (b->x - a->x);
    out->y = y;
    out->z = a->z + t * (b->z - a->z);
}

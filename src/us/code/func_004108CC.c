/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80401564_de.c, func_80401564_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct func_80203E78_S1;
struct func_80203E78_S1 {
    char pad0[0x4];
    s32 unk4;
};
struct func_80203E78_S1;
typedef struct func_80203E78_S1 func_80203E78_S1;
struct PositionKey;
typedef struct PositionKey PositionKey;
struct PositionKey;
struct PositionKey {
    f32 x;
    f32 y;
    f32 z;
    f32 pad;
    f32 time;
};
extern Vec3 func_004108CC(f32 t);
extern func_80203E78_S1 *D_800C1BB0;
extern s32 *func_002017D4(s32 resource, s32 index);
extern void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
extern void func_0020EEF8(Vec3 *out, Vec3 *a, Vec3 *b);
Vec3 func_004108CC(f32 t) {
    s32 *track;
    PositionKey *key;
    s32 count;
    f32 u;
    Vec3 result;
    Vec3 a;
    Vec3 b;
    track = func_002017D4(D_800C1BB0->unk4, 1);
    count = track[1];
    key = (PositionKey *)(track + 2);
    if (count == 0) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return result;
    } else if (t <= key[0].time) {
        result.x = key[0].x;
        result.y = key[0].y;
        result.z = key[0].z;
        return result;
    } else if (key[count - 1].time <= t) {
        result.x = key[count - 1].x;
        result.y = key[count - 1].y;
        result.z = key[count - 1].z;
        return result;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        a.x = key[-1].x;
        a.y = key[-1].y;
        a.z = key[-1].z;
        b.x = key->x;
        b.y = key->y;
        b.z = key->z;
        func_0020EF60(&a, &a, (1.0f) - u);
        func_0020EF60(&b, &b, u);
        func_0020EEF8(&result, &a, &b);
    }
    return result;
}

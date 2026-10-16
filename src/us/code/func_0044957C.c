/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80401214_de.c, func_80401214_de.
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
struct KeyRecAt1C;
typedef struct KeyRecAt1C KeyRecAt1C;
struct KeyRecAt1C;
struct KeyRecAt1C {
    char pad0[0x1C];
    f32 time;
    char pad20[4];
};
extern func_80203E78_S1 *D_800C1BB0;
extern s32 *func_002017D4(s32 *node, s32 index);
extern Vec3 func_004101B8(KeyRecAt1C *keys, s32 count, f32 t);
Vec3 func_0044957C(f32 t) {
    s32 i;
    s32 idx;
    s32 count;
    KeyRecAt1C *keys;
    s32 *clip;
    f32 start;
    f32 end;
    start = 0.0f;
    count = 0;
    keys = 0;
    idx = 0;
    i = 0;
    end = start;
    while (i < *func_002017D4((s32 *)D_800C1BB0->unk4, 0)) {
        idx = i;
        clip = func_002017D4(func_002017D4(func_002017D4((s32 *)D_800C1BB0->unk4, 0), idx), 0);
        count = clip[1];
        keys = (KeyRecAt1C *)(clip + 2);
        start = end;
        end = start + keys[count - 1].time;
        if (idx == 0 && t <= start) {
            break;
        }
        if (idx == *func_002017D4((s32 *)D_800C1BB0->unk4, 0) - 1 && end < t) {
            break;
        }
        i = idx + 1;
        if (start <= t && t <= end) {
            break;
        }
    }
    if (*func_002017D4(func_002017D4(func_002017D4((s32 *)D_800C1BB0->unk4, 0), idx), 2) != 0) {
        t = start;
    }
    return func_004101B8(keys, count, t - start);
}

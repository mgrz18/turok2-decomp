/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040184C_de.c, func_8040184C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct D_800C7470_Pair;
struct D_800C7470_Pair {
    f32 first;
    f32 second;
};
struct D_800C7470_Pair;
typedef struct D_800C7470_Pair D_800C7470_Pair;
struct func_80203E78_S1;
struct func_80203E78_S1 {
    char pad0[0x4];
    s32 unk4;
};
struct func_80203E78_S1;
typedef struct func_80203E78_S1 func_80203E78_S1;
extern float D_800C0998;
struct D_800C7470_Pair;
struct D_800C7470_Pair;
extern f32 D_800C0990[];
extern func_80203E78_S1 *D_800C1BB0;
extern s32 *func_002017D4(s32 resource, s32 index);
f32 func_00410BB4(f32 t) {
    s32 *track;
    D_800C7470_Pair *key;
    s32 count;
    s32 i;
    f32 u;
    track = func_002017D4(D_800C1BB0->unk4, 3);
    key = (D_800C7470_Pair *)(track + 2);
    count = track[1];
    for (i = 0; i < count; i++) {
    }
    if (count == 0) {
        return D_800C0990[1];
    }
    if (t <= key[0].second) {
        return key[0].first;
    }
    if (key[count - 1].second <= t) {
        return key[count - 1].first;
    }
    while (key->second < t) {
        key++;
    }
    u = (t - key[-1].second) / (key->second - key[-1].second);
    u = u * (u * D_800C0998) - 2.0f * u * u * u;
    return key[-1].first * ((*(&D_800C0998 + 1)) - u) + key->first * u;
}

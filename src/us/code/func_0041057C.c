/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80401214_de.c, func_80401214_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern func_80203E78_S1 *D_800C1BB0;
extern s32 *func_002017D4(s32 *node, s32 index);
extern Vec3 func_004101B8(KeyRecAt1C *keys, s32 count, f32 t);
Vec3 func_0041057C(f32 t) {
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

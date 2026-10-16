/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80400000_de_11.c, func_804039C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
extern f32 func_00412D04(void);
struct State_func_804039C4_de;
typedef struct State_func_804039C4_de State_func_804039C4_de;
struct State_func_804039C4_de;
struct State_func_804039C4_de {
    s32 a;
    s32 *unk4;
};
f32 func_00412D04(void) {
    s32 *func_002017D4(s32 *, s32);
    extern State_func_804039C4_de *D_800C1BB0;
    f32 temp_f0;
    f32 var_f20;
    s32 *temp_v0;
    s32 var_s0;
    var_f20 = 0.0f;
    var_s0 = 0;
loop_1:
    if (var_s0 < *func_002017D4(D_800C1BB0->unk4, 0)) {
        temp_v0 = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->unk4, 0), var_s0), 0);
        temp_v0 = (s32 *)((char *)temp_v0 + temp_v0[1] * 0x24);
        temp_f0 = *(f32 *)temp_v0;
        var_s0 += 1;
        var_f20 += temp_f0;
        goto loop_1;
    }
    return var_f20;
}

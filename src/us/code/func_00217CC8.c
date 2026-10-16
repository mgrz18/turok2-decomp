/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_80260828_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern void func_00217CC8(u32 *arg0, u32 arg1, u32 arg2);
struct func_80203E78_S1;
struct func_80203E78_S1 {
    char pad0[0x4];
    s32 unk4;
};
struct func_80203E78_S1;
typedef struct func_80203E78_S1 func_80203E78_S1;
void func_00217CC8(u32 *arg0, u32 arg1, u32 arg2) {
    u32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    u32 temp_a1;
    u32 temp_a2;
    u32 temp_shift;
    u32 var_a2;
    u32 var_a3;
    u32 var_v1;
    u32 value;
    value = *arg0;
    temp_a1 = (1 << arg1) - 1;
    var_v1 = temp_a1;
    temp_v0 = value & 0x1F;
    temp_shift = temp_v0;
    temp_a2 = arg2 & temp_a1;
    if (temp_v0 == 0) {
        var_a3 = temp_a2;
        temp_a1 = 0;
        var_a2 = 0;
    } else {
        var_v1 = temp_a1 << temp_shift;
        temp_v0_2 = 0x20 - temp_shift;
        temp_a1 >>= temp_v0_2;
        var_a3 = temp_a2 << temp_shift;
        var_a2 = temp_a2 >> temp_v0_2;
    }
    temp_a0 = (value & 0xF0000000) | ((u32)(value & 0x0FFFFFE0) >> 3);
    *(s32 *)temp_a0 = (*(s32 *)temp_a0 & ~var_v1) | var_a3;
    ((struct func_80203E78_S1 *) temp_a0)->unk4 = (((struct func_80203E78_S1 *) temp_a0)->unk4 & ~temp_a1) | var_a2;
    *arg0 += arg1;
}

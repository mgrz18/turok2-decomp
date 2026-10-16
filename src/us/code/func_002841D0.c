/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802457E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct func_802457D0_S1;
typedef struct func_802457D0_S1 func_802457D0_S1;
extern s32 func_002841D0(void);
struct func_802457D0_S1;
struct func_802457D0_S1 {
    char pad0[0x1C];
    f32 unk1C;
    char pad1C[0x34 - 0x1C - sizeof(f32)];
    f32 unk34;
    char pad34[0x38 - 0x34 - sizeof(f32)];
    s32 unk38;
};
s32 func_002841D0(void) {
    extern func_802457D0_S1 *D_800C1BB0;
    if (D_800C1BB0->unk38 != 0) {
        if (D_800C1BB0->unk1C > D_800C1BB0->unk34) {
            return 1;
        }
        return 0;
    }
    return 1;
}

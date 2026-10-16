/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80400000_de_11.c, func_8040385C_de.
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
struct StridedTable;
struct StridedTable {
    s32 stride;
    s32 count;
    char data[1];
};
struct StridedTable;
typedef struct StridedTable StridedTable;
extern Vec3 func_00412B9C(void);
struct State_func_8040385C_de;
typedef struct State_func_8040385C_de State_func_8040385C_de;
struct State_func_8040385C_de;
struct State_func_8040385C_de {
    char a[4];
    void *unk4;
    char b[0x30];
    s32 unk38;
};
void func_004101B8(Vec3 *, void *, s32, s32);
Vec3 func_00412B9C(void) {
    void *func_002017D4(void *, s32);
    extern State_func_8040385C_de *D_800C1BB0;
    Vec3 sp10;
    Vec3 sp20;
    StridedTable *temp_v0;
    if (D_800C1BB0->unk38 == 0) {
        sp20.x = 0;
        sp20.y = 0;
        sp20.z = 0;
        return sp20;
    }
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->unk4, 0), 0), 0);
    func_004101B8(&sp10, temp_v0->data, temp_v0->count, 0);
    return sp10;
}

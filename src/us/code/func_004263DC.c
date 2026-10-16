/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_5.c, func_80404D84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct Shape_typemap_165;
struct Pack;
struct Shape_typemap_165 {
    union { int field_0; struct Pack *pack; void *data; };
    int field_4;
    int field_8;
    int field_C;
};
void func_004263DC(void);
struct Shape_typemap_165;
struct Shape_typemap_165;
struct Shape_typemap_165;
struct Pack;
struct Pack {
    s32 count;
    s32 offsets[1];
};
struct Pack;
typedef struct Pack Pack;
struct Shape_typemap_165;
extern int *D_800C2228;
extern s32 D_800C2220;
struct Shape_typemap_165 *func_00204EDC(s32, s32, u32, void *);
void *func_00266C5C(void *, s32, u32);
extern s32 D_800C18A0;
extern s32 D_800C2224;
extern int D_800C2220;
extern int D_800C2224;
extern int *D_800C2228;
void func_004263DC(void) {
    struct Shape_typemap_165 *temp_v0;
    s32 temp_a0;
    temp_v0 = func_00204EDC(0, 0x810, 0x23, &D_800C18A0);
    temp_a0 = temp_v0->field_0;
    D_800C2228 = (s32 *)temp_v0;
    D_800C2224 = temp_a0;
    func_00266C5C((void *)temp_a0, 0, 0x810);
    D_800C2220 = 1;
}

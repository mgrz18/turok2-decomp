/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040385C.c, func_0040385C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct O01Ctx {
               u8 pad0[4];
               void *tree;
               u8 pad8[0x10];
               s32 unk18;
               u8 pad1C[0x1C];
               s32 unk38;
               u8 pad3C[0x1C];
               s32 unk58;
               u8 pad5C[0x58];
               s32 unkB4;
               u8 padB8[0x20];
               s32 unkD8;
               s32 unkDC;
               s32 unkE0;
} O01Ctx;
typedef struct O01Node {
              s32 unk0;
              s32 unk4;
} O01Node;
typedef struct O01Vec3 {
    f32 x, y, z;
} O01Vec3;
extern O01Ctx *D_800C1BB0;
void *func_002017D4(void *tree, s32 idx);
O01Vec3 func_004101B8(void *keys, s32 n, f32 t);
O01Vec3 func_0044BB9C(void) {
    O01Vec3 t;
    O01Vec3 v;
    O01Node *n;
    if (D_800C1BB0->unk38 == 0) {
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    n = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->tree, 0), 0), 0);
    t = func_004101B8((u8 *)n + 8, n->unk4, 0.0f);
    return t;
}

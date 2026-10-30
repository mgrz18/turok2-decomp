/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004039C4.c, func_004039C4.
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
typedef struct O01Rec36 { f32 f0; u8 pad[32]; } O01Rec36;
extern O01Ctx *D_800C1BB0;
void *func_002017D4(void *tree, s32 idx);
f32 func_0044BD04(void) {
    f32 sum = 0.0f;
    s32 i = 0;
    while (i < ((O01Node *)func_002017D4(D_800C1BB0->tree, 0))->unk0) {
        O01Node *n = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->tree, 0), i), 0);
        n = (O01Node *)((O01Rec36 *)n + n->unk4);
        sum += *(f32 *)n;
        i++;
    }
    return sum;
}

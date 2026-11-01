/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00401564.c, func_00401564.
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
typedef struct O01K5 {
               f32 x, y, z;
               f32 w;
               f32 time;
} O01K5;
extern O01Ctx *D_800C1BB0;
void *func_002017D4(void *tree, s32 idx);
void func_0020EF60(O01Vec3 *dst, O01Vec3 *src, f32 s);
void func_0020EEF8(O01Vec3 *dst, O01Vec3 *a, O01Vec3 *b);
O01Vec3 *func_004498CC(O01Vec3 *out, f32 t) {
    O01Vec3 res;
    O01Vec3 a;
    O01Vec3 b;
    O01Node *n = func_002017D4(D_800C1BB0->tree, 1);
    O01K5 *keys = (O01K5 *)((u8 *)n + 8);
    s32 cnt = n->unk4;
    O01K5 *k;
    f32 u;
    if (cnt == 0) {
        res.x = 0;
        res.y = 0;
        res.z = 0;
    } else if (t <= keys[0].time) {
        res.x = keys[0].x;
        res.y = keys[0].y;
        res.z = keys[0].z;
    } else if (keys[cnt - 1].time <= t) {
        res.x = keys[cnt - 1].x;
        res.y = keys[cnt - 1].y;
        res.z = keys[cnt - 1].z;
    } else {
        n = 0;
        if (keys[0].time < t) {
            keys++;
            while (keys->time < t) {
                keys++;
            }
        }
        u = (t - keys[-1].time) / (keys->time - keys[-1].time);
        a.x = keys[-1].x;
        a.y = keys[-1].y;
        a.z = keys[-1].z;
        b.x = keys->x;
        b.y = keys->y;
        b.z = keys->z;
        func_0020EF60(&a, &a, 1.0f - u);
        func_0020EF60(&b, &b, u);
        func_0020EEF8(&res, &a, &b);
    }
    do { *out = res; } while (0);
    return out;
}

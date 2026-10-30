/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00403458.c, func_00403458.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct O01Node {
              s32 unk0;
              s32 unk4;
} O01Node;
typedef struct O01CtxD {
               u8 pad0[4];
               void *tree;
               u8 pad8[0x14];
               f32 unk1C;
               u8 pad20[0x14];
               f32 unk34;
               u8 pad38[0x18];
               s32 unk50;
               u8 pad54[4];
               s32 unk58;
} O01CtxD;
typedef struct O01Kind {
               u8 pad0[0xE];
               u8 unkE;
               u8 padF[2];
               u8 kind;
               u8 val;
} O01Kind;
typedef struct O01EntF {
              f32 time;
              s32 unk4;
              s32 unk8;
} O01EntF;
typedef struct O01G {
                 u8 pad0[0x1178];
                 u8 *unk1178;    /* 0x58 earlier than Rage Wars */
                 u8 *unk117C;
} O01G;
extern O01CtxD *D_800C1BB0;
extern O01G D_800F7078[];
extern f32 D_800C0AD0;
extern f32 D_800C0AD4;
s32 func_00284174(void);
void *func_002017D4(void *tree, s32 idx);
void func_00412314(s32 a, O01Kind *k);
void func_0026E210(O01Kind *k);
void func_00224A68(O01G *g, O01Kind *k, s32 z);
/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline s32 O01KindOk(O01Kind *a) {
    s32 r = 0;
    switch (a->kind) {
    case 1:
        if (a->val == 5) r = 1;
        break;
    case 9:
        if (a->val == 4) r = 1;
        break;
    case 10:
        if (a->val == 3) r = 1;
        break;
    default:
        r = 0;
        break;
    }
    return r;
}
void func_0044B798(void) {
    f32 now = D_800C1BB0->unk1C;
    s32 flag = 0;
    O01Node *n;
    O01EntF *e;
    O01EntF *ents;
    O01Kind *k;
    s32 cnt;
    s32 i;
    if (func_00284174() != 0) {
        if (D_800C1BB0->unk50 != 0) {
            n = func_002017D4(D_800C1BB0->tree, 2);
            cnt = n->unk4;
            ents = (O01EntF *)((u8 *)n + 8);
            if (cnt != 0) {
                if (D_800C1BB0->unk34 <= now) {
                    flag = 1;
                }
                for (i = 0; i < cnt; i++) {
                    e = &ents[i];
                    if (e->time <= D_800C0AD0) {
                        continue;
                    }
                    if (now < e->time) {
                        if (flag == 0) {
                            continue;
                        }
                    }
                    if (e->unk4 == 0) {
                        k = (O01Kind *)(D_800F7078->unk1178 + e->unk8 * 20);
                    } else {
                        k = (O01Kind *)(D_800F7078->unk117C + e->unk8 * 20);
                    }
                    if (O01KindOk(k) != 0) {
                        func_00412314(0, k);
                        D_800C1BB0->unk58 = 0;
                    } else {
                        func_0026E210(k);
                    }
                    e->time = D_800C0AD4;
                    k->unkE = (k->unkE | 1) & 0xEF;
                    func_00224A68(D_800F7078, k, 0);
                }
            }
        }
    }
}

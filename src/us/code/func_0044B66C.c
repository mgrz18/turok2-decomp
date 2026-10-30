/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040332C.c, func_0040332C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
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
typedef struct O01Kind {
               u8 pad0[0x11];
               u8 kind;
               u8 val;
} O01Kind;
typedef struct O01Ent12 {
              s32 unk0;
              s32 unk4;
              s32 unk8;
} O01Ent12;
typedef struct O01G {
    u8 pad0[0x1178];
    u8 *recA;               /* 0x1178 (0x11D0 in Rage Wars) */
    u8 *recB;               /* 0x117C (0x11D4) */
} O01G;
extern O01Ctx *D_800C1BB0;
extern O01G D_800F7078[];
void *func_002017D4(void *tree, s32 idx);
/* A separate function in Rage Wars; inlined here in Turok 2. */
static inline s32 O01KindOk(O01Kind *k) {
    s32 r = 0;
    switch (k->kind) {
    case 1:
        if (k->val == 5) r = 1;
        break;
    case 9:
        if (k->val == 4) r = 1;
        break;
    case 10:
        if (k->val == 3) r = 1;
        break;
    default:
        r = 0;
        break;
    }
    return r;
}
s32 func_0044B66C(void) {
    s32 any = 0;
    s32 i;
    O01Node *n = func_002017D4(D_800C1BB0->tree, 2);
    s32 cnt = n->unk4;
    O01Ent12 *ents = (O01Ent12 *)((u8 *)n + 8);
    O01Ent12 *e;
    O01Kind *k;
    for (i = 0; i < cnt; i++) {
        e = &ents[i];
        if (e->unk4 == 0) {
            k = (O01Kind *)(D_800F7078->recA + e->unk8 * 20);
        } else {
            k = (O01Kind *)(D_800F7078->recB + e->unk8 * 20);
        }
        if (O01KindOk(k) != 0) {
            any = 1;
        }
    }
    return any ^ 1;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023BC64.c, func_0023BC64.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef struct B09LNode B09LNode;
struct B09LNode {
               B09LNode *next;
               B09LNode *prev;
               u16 slot;
               u8 id;
               u8 tlb;
               s32 unkC;
};
typedef struct B09SlotA {
            u16 key;
            u8 a;
            u8 b;
} B09SlotA;
typedef struct B09SlotCtl {
            u16 cur;
            u16 start;
} B09SlotCtl;
extern B09SlotA D_80121318[];
extern B09LNode D_80120FC0[];
extern B09SlotCtl D_80121390;
s32 func_0028DAEC(void) {
    s16 idx = D_80121390.cur++;
    if ((s16)D_80121390.cur >= 0x18) {
        D_80121390.cur = D_80121390.start;
    }
    if (D_80121318[idx].a != 0xFF) {
        D_80120FC0[D_80121318[idx].a].tlb = 0xFF;
        D_80121318[idx].a = 0xFF;
    }
    if (D_80121318[idx].b != 0xFF) {
        D_80120FC0[D_80121318[idx].b].tlb = 0xFF;
        D_80121318[idx].b = 0xFF;
    }
    return idx;
}

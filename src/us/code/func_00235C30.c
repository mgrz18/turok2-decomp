/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A6170.c, func_002A6170.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct DlCmd {
    u32 w0;
    u32 w1;
} DlCmd;
typedef struct VInfo {
    u8 pad0[0x18];
    s16 x;
    s16 y;
    s16 z;
} VInfo;
typedef struct VNode VNode;
struct VNode {
               u8 pad0[4];
               VNode *next;
               VInfo info;
};
typedef struct VList {
    VNode *head;
    u8 pad4[0x10];
} VList;
typedef struct VCtx {
                 u8 pad0[0x94D8];
                 VList lists[10];
                 u8 pad95A0[0x10];
                 s32 mode;
} VCtx;
typedef struct Vec3fv { f32 x, y, z; } Vec3fv;
extern DlCmd *D_800D8E24;
extern u8 D_800AB580[];
extern u8 D_800ACFC8[];
extern u8 D_8011AD48[];
extern f32 D_800F5A10[6];
void func_0020B5B8(void);
void func_00207A54(void);
void CList__Remove(VList *, VNode *);
void CList__AddTail(void *, VNode *);
void func_00235C30(VCtx *c) {
    s32 i;
    VList *l;
    VNode *n, *next;
    Vec3fv v;
    VInfo *p;
    if (c->mode == 100) {
        return;
    }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xE3000A01); _g->w1 = (0x100000); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xFD900000); _g->w1 = ((u32)D_800AB580); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xF5900000); _g->w1 = (0x07080200); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xE6000000); _g->w1 = (0); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xF3000000); _g->w1 = (0x0707F400); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xF5880400); _g->w1 = (0x00080200); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xF2000000); _g->w1 = (0x0003C03C); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xFA000000); _g->w1 = (-0x38); }
    { DlCmd *_g = D_800D8E24++; _g->w0 = (0xDE000000); _g->w1 = ((u32)D_800ACFC8); }
    for (i = 0; i < 10; i++) {
        l = &c->lists[i];
        for (n = l->head; n != ((void *)0); n = next) {
            next = n->next;
            p = &n->info;
            v.x = p->x;
            v.y = p->y;
            v.z = p->z;
            if (!(v.x < D_800F5A10[3] && v.y < D_800F5A10[4] && v.z < D_800F5A10[5] && D_800F5A10[0] <= v.x && D_800F5A10[1] <= v.y && D_800F5A10[2] <= v.z)) {
                CList__Remove(l, n);
                CList__AddTail((u8 *)c + 0x95A0, n);
            } else {
                { DlCmd *_g = D_800D8E24++; _g->w0 = (0xDA380003); _g->w1 = ((u32)p); }
                { DlCmd *_g = D_800D8E24++; _g->w0 = (0x01004008); _g->w1 = ((u32)D_8011AD48); }
                { DlCmd *_g = D_800D8E24++; _g->w0 = (0x06000204); _g->w1 = (0x00040600); }
            }
        }
    }
    func_0020B5B8();
    func_00207A54();
}

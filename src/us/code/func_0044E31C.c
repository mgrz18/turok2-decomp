/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00441384.c, func_00441384.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Q04Item {
    s32 idx;
    s16 w4;
    u8 pad6[2];
    u32 flags;
    u16 wC;
    u16 wE;
    u8 b10, b11, b12, b13;
    s32 f14;
    void *def;
    s32 f1C;
    void *p20;
    void *owner;
} Q04Item;
typedef struct { u8 pad[0xB0]; u32 btn; } Q04Ctl;
typedef struct { u8 pad[4]; u32 flags; u8 pad8[0x1C - 8]; s32 (*f1C)(void *, void *, void *); } Q04Def2;
typedef struct { u8 pad[0x14]; s32 (*f14)(void); } Q04Hdr2;
typedef struct {
    s16 cur;
    u8 pad2[0xA];
    Q04Item *items;
    s32 n;
    Q04Hdr2 *hdr;
    u8 pad18[8];
    Q04Ctl *ctl;
    u8 pad24[4];
    s16 state;
    u8 pad2A[2];
    f32 f2C, f30, f34;
} Q04Pan2;
extern f32 D_800B6D28;
extern s32 func_002855E8(void *c);
extern s32 func_00285628(void *c);
extern s32 func_00285670(void *c);
extern s32 func_002856B8(void *c);
extern s32 func_00285700(void *c);
extern void func_004151B4(Q04Pan2 *p, s32 a);
extern void func_00416C28(Q04Pan2 *p);
s32 func_0044E31C(Q04Pan2 *p, void *a1) {
    Q04Item *it;
    s32 s4, i, s0, t;
    u32 s3;
    s4 = 0;
    if (p->hdr->f14 != 0) s4 = p->hdr->f14();
    i = p->n;
    it = p->items;
    i--;
    if (i != -1) {
        do {
            void (*cb)(void *, void *, void *) = ((void (**)(void *, void *, void *))it->def)[6];
            if (cb != 0) cb(it, p, a1);
            i--;
            it++;
        } while (i != -1);
    }
    it = (Q04Item *)((u8 *)p->items + p->cur * 0x28);
    p->f2C = p->f2C + D_800B6D28;
    s3 = it->flags & 0x400000;
    func_00416C28(p);
    switch (p->state) {
    case 0:
        p->f30 = p->f30 + D_800B6D28;
        if (1.0f <= p->f30) { p->f30 = 1.0f; p->state = 1; }
        break;
    case 1:
        p->f34 = p->f34 + D_800B6D28 * 0.4f;
        if (1.0f <= p->f34) { p->f34 = 1.0f; p->state = 2; }
        break;
    case 3:
        p->f34 = p->f34 - D_800B6D28 * 0.4f;
        if (p->f34 <= 0.0f) { p->f34 = 0.0f; p->state = 4; }
        break;
    case 4:
        p->f30 = p->f30 - D_800B6D28;
        if (p->f30 <= 0.0f) { p->f30 = 0.0f; return 1; }
        break;
    }
    if (p->state < 3) if (p->state >= 0) {
        if (s4 != 0) p->state = 4;
        if (!(it->flags & 8)) {
            s0 = 1;
            if (it->w4 == 2) {
                t = p->ctl->btn;
                if (t & 0x20202) {
                    s0 = 0;
                    it->f1C -= 2;
                    if (it->f1C < 0) it->f1C = 0;
                } else if (t & 0x40101) {
                    it->f1C += 2;
                    s0 = 0;
                    if (it->f1C >= 0x100) it->f1C = 0xFF;
                }
            }
            if (s3 == 0) {
                if (func_00285628(p->ctl) != 0) func_004151B4(p, 0x10);
                if (func_00285670(p->ctl) != 0) func_004151B4(p, 0x11);
            } else if (p->ctl->btn & 0x60303) {
                s0 = 0;
            }
            if (s0 != 0) {
                if (func_002856B8(p->ctl) != 0) func_004151B4(p, 0x12);
                if (func_00285700(p->ctl) != 0) func_004151B4(p, 0x13);
            }
            if (p->state == 2 && (func_002855E8(p->ctl) != 0 || (func_00285628(p->ctl) != 0 && s3 != 0) || (func_00285670(p->ctl) != 0 && s3 != 0))) {
                s32 (*cb2)(void *, void *, void *) = ((Q04Def2 *)it->def)->f1C;
                if (cb2 == 0 || cb2(it, p, a1) != 0) {
                    p->state = 3;
                }
            }
        }
    }
    return 0;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct O01Save {
              u16 w[4];
              u8 b0;
              u8 b1;
              u8 b2;
              u8 b3;
              u8 padC[8];
               u32 crc;
} O01Save;
typedef struct O01SaveSrc {
               u16 w[4];
               u8 pad8[0x28];
               u8 b0;           /* 0x30 */
               u8 pad31[2];
               u8 tail[8];      /* 0x33 */
} O01SaveSrc;
typedef struct O01Actor {
                u8 pad0[0x518];
                O01SaveSrc *save;   /* 0x518: player options */
                u8 *unk51C;
                u8 pad520[0x530];
                s32 unkA50;         /* 0xA50 (Rage Wars 0x698) */
} O01Actor;
typedef struct O01SubC {
               u8 pad0[4];
               s8 unk4;
} O01SubC;
typedef struct O01OwnC {
               u8 pad0[0x1C];
               O01Actor *unk1C;
               O01SubC *unk20;
} O01OwnC;
typedef struct O01Sel { s32 v; s32 w; } O01Sel;
extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2020;
extern s32 D_800C2028;
extern O01Save D_80130968;
extern u8 D_80119E2C[];
extern u8 D_0043BB68[];
extern u8 D_0043BF7C[];
extern u8 D_0043BB20[];
extern u8 D_0043B5EC[];
void func_004263A0(s32 ch);
s32 func_0042647C(s32 ch);
s32 func_004264B4(s32 ch);
s32 func_00426AF8(s32 ch);
void func_0041D948(O01OwnC *o);
u32 func_00426D98(u8 *data, s32 len, u32 crc);
void func_00416644(u8 *a, void *b, void *c, s32 d, void *e);
extern u8 D_80130974[8];
extern u8 D_0043ABA0[];
extern u8 D_0043ABC4[];
extern u8 D_0043ABE8[];
extern u8 D_0043AC0C[];
/* Static inlines in the Rage Wars source file (dropped by the importer). */
static inline u8 *O01Pick(O01Sel *sel) {
    u8 *p;
    switch (sel->v) {
    default:
    case 0:
        p = D_0043ABA0;
        break;
    case 1:
        p = D_0043ABC4;
        break;
    case 2:
        p = D_0043ABE8;
        break;
    case 3:
        p = D_0043AC0C;
        break;
    }
    return p;
}
static inline void O01Capture(O01Actor *a) {
    O01SaveSrc *p = a->save;
    s32 i;
    D_80130968.b0 = p->b0;
    D_80130968.w[0] = p->w[0];
    D_80130968.w[1] = p->w[1];
    D_80130968.w[2] = p->w[2];
    D_80130968.w[3] = p->w[3];
    D_80130968.b1 = 0;
    D_80130968.b2 = 0;
    D_80130968.b3 = 0;
    for (i = 0; i < 8; i++) {
        D_80130974[i] = p->tail[i];
    }
}
void func_00456E70(O01OwnC *own) {
    O01Actor *plr;
    O01Sel sel;
    u8 *msg;
    s32 pad;
    O01Actor *pp;
    s32 pv;
    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 1;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    pv = own->unk20->unk4;
    pp = own->unk1C;
    sel.v = pv;
    plr = pp;
    msg = O01Pick(&sel);
    if (D_800C2020 != 0) {
        D_801309BC = 1;
        func_00416644(own->unk1C->unk51C + 0x53C, msg, plr, plr->unkA50, 0);
    } else {
        if (D_80130990 != 0) {
            pad = D_800C2028;
        } else {
            pad = own->unk20->unk4;
        }
        func_004263A0(pad);
        if (func_0042647C(sel.v) == 0) {
            O01Capture(plr);
            D_80130968.crc = func_00426D98((u8 *)&D_80130968, 0x14, 4);
            func_0041D948(own);
        } else {
            D_801309BC = 1;
            if (func_004264B4(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043BB68, plr, plr->unkA50, msg);
            } else if (func_00426AF8(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043BF7C, plr, plr->unkA50, D_0043B5EC);
            } else {
                func_00416644(D_80119E2C, D_0043BB20, plr, plr->unkA50, msg);
            }
        }
    }
}

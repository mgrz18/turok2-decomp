/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407290.c, func_00407290.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct O01Plr3 {
                u8 pad0[0xA50];
                s32 unkA50;     /* 0xA50 in Turok 2 (0x698 in Rage Wars) */
} O01Plr3;
typedef struct O01Sub9 {
               u8 pad0[4];
               s8 unk4;
} O01Sub9;
typedef struct O01OwnJ {
               u8 pad0[0x14];
               s32 unk14;
               u8 pad18[4];
               O01Plr3 *unk1C;
               O01Sub9 *unk20;
} O01OwnJ;
typedef struct O01Pad { s32 v; s32 freeb; s32 cnt; } O01Pad;
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_80130984;
extern u8 D_80130968[];
extern char D_800B45C0[];
extern char D_800B45D8[];
extern u8 D_80119E2C[];
extern u8 D_0043BBDC[];
extern u8 D_0043BD68[];
extern u8 D_0043BDF8[];
s32 func_00426E6C(s32 n);
s32 func_004267A0(s32 pad, s32 *out);
s32 func_0042670C(s32 pad, s32 *out);
s32 func_00426A94(s32 pad, u8 *ext);
s32 func_00426164(s32 pad, s32 size, u8 *buf, char *n1, u8 *ext, char *n2);
void func_00416644(u8 *a, void *b, void *c, s32 d, void *e);
extern s32 D_800C2010;
extern s32 D_800C2014;
extern s32 D_800C2018;
extern s32 D_800C201C;
extern u8 D_0043AC80[];
extern u8 D_0043ACA4[];
extern u8 D_0043ACC8[];
extern u8 D_0043ACEC[];
void func_002053A8(s32 a);
void func_002052D8(s32 a, s32 b);
/* Static inlines in the Rage Wars source file (dropped by the importer). */
static inline u8 *O01Pick(O01Pad *sel) {
    u8 *p;
    switch (sel->v) {
    default:
    case 0:
        p = D_0043AC80;
        break;
    case 1:
        p = D_0043ACA4;
        break;
    case 2:
        p = D_0043ACC8;
        break;
    case 3:
        p = D_0043ACEC;
        break;
    }
    return p;
}
static inline void O01FreeAll(void) {
    if (D_800C2010 != 0 || D_800C2014 != 0) {
        func_002053A8(0);
    }
    if (D_800C2010 != 0) {
        func_002052D8(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        func_002052D8(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
}
s32 func_0041EDE0(s32 unused, O01OwnJ *own, s32 unused3) {
    O01Plr3 *plr;
    O01Plr3 *pp;
    s32 pv;
    u8 ext[8];
    O01Pad pad;
    s32 r = 0;
    s32 ok = 1;
    s32 flag = 0;
    s32 need;
    u32 nv;
    if (D_80130990 != 0) {
        pv = D_800C2028;
        pp = 0;
    } else {
        pv = own->unk20->unk4;
        pp = own->unk1C;
    }
    pad.v = pv;
    plr = pp;
    if (D_80130984 != 0) {
        need = func_00426E6C(0x18);
        r = func_004267A0(pad.v, &pad.freeb);
        if (r == 0) {
            if (pad.freeb >= need) {
                r = func_0042670C(pad.v, &pad.cnt);
                if (r == 0 && pad.cnt != 0) {
                    ok = 0;
                    r = func_00426A94(pad.v, ext);
                    if (r == 0) {
                        flag = 1;
                        nv = pad.v;
                        r = func_00426164(nv, 0x18, D_80130968, D_800B45C0, ext, D_800B45D8);
                    }
                }
            }
            if (ok != 0) {
                func_00416644(D_80119E2C, D_0043BBDC, plr, plr->unkA50, (void *)own->unk14);
                return 0;
            }
        }
    }
    O01FreeAll();
    if (flag != 0) {
        if (r == 0) {
            func_00416644(D_80119E2C, D_0043BD68, plr, plr->unkA50, O01Pick(&pad));
        } else {
            func_00416644(D_80119E2C, D_0043BDF8, plr, plr->unkA50, O01Pick(&pad));
        }
    }
    return 1;
}

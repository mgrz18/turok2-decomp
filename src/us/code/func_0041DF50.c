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
    u8 tail[8];             /* 0x0C */
    u32 crc;                /* 0x14 */
} O01Save;
/* Player options (Turok 2 layout). */
typedef struct O01Options {
    u16 w[4];
    u8 pad8[0x28];
    u8 b30;
    u8 pad31[2];
    u8 tail[8];             /* 0x33 */
} O01Options;
typedef struct O01Actor {
    u8 pad0[0x518];
    O01Options *options;    /* 0x518 (0x5D8 in Rage Wars) */
    u8 *unk51C;             /* 0x51C (0x5DC) */
    u8 pad520[0xA50 - 0x520];
    s32 unkA50;             /* 0xA50 (0x698) */
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
extern u8 D_0043BC48[];
extern u8 D_0043C05C[];
extern u8 D_0043BC00[];
extern u8 D_0043B6CC[];
extern u8 D_0043AC80[];
extern u8 D_0043ACA4[];
extern u8 D_0043ACC8[];
extern u8 D_0043ACEC[];
void func_00426480(s32 ch);
s32 func_0042655C(s32 ch);
s32 func_00426594(s32 ch);
s32 func_00426BD8(s32 ch);
void func_0041DA28(O01OwnC *o);
u32 func_00426E78(u8 *data, s32 len, u32 crc);
void func_00416644(u8 *a, void *b, void *c, s32 d, void *e);
/* O01Pick and O01Capture are separate functions in Rage Wars; inlined here in Turok 2. */
static inline u8 *O01Pick(O01Sel *sel) {
    u8 *msg;
    switch (sel->v) {
    case 0:
    default:
        msg = D_0043AC80;
        break;
    case 1:
        msg = D_0043ACA4;
        break;
    case 2:
        msg = D_0043ACC8;
        break;
    case 3:
        msg = D_0043ACEC;
        break;
    }
    return msg;
}
static inline void O01Capture(O01Actor *a) {
    O01Options *o = a->options;
    s32 i;
    D_80130968.b0 = o->b30;
    D_80130968.w[0] = o->w[0];
    D_80130968.w[1] = o->w[1];
    D_80130968.w[2] = o->w[2];
    D_80130968.w[3] = o->w[3];
    D_80130968.b1 = 0;
    D_80130968.b2 = 0;
    D_80130968.b3 = 0;
    for (i = 0; i < 8; i++) {
        D_80130968.tail[i] = o->tail[i];
    }
}
void func_0041DF50(O01OwnC *own) {
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
        func_00426480(pad);
        if (func_0042655C(sel.v) == 0) {
            O01Capture(plr);
            D_80130968.crc = func_00426E78((u8 *)&D_80130968, 0x14, 4);
            func_0041DA28(own);
        } else {
            D_801309BC = 1;
            if (func_00426594(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043BC48, plr, plr->unkA50, msg);
            } else if (func_00426BD8(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043C05C, plr, plr->unkA50, D_0043B6CC);
            } else {
                func_00416644(D_80119E2C, D_0043BC00, plr, plr->unkA50, msg);
            }
        }
    }
}

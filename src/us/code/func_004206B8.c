/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00408C78.c, func_00408C78.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed int s32;
typedef struct O01Save {         /* player save block (Turok 2 layout) */
               u16 w[4];
               u8 pad8[0x28];
               u8 b30;
               u8 pad31[2];
               u8 tail[8];      /* 0x33 */
} O01Save;
typedef struct O01SaveSrc {      /* saved copy kept in a global */
               u16 w[4];
               u8 b8;
               u8 pad9[3];
               u8 tail[8];      /* 0xC */
} O01SaveSrc;
typedef struct O01Actor {        /* Turok 2 layout */
                u8 pad0[0x518];
                O01Save *save;  /* 0x518 (Rage Wars: 0x5D8) */
                u8 *unk51C;     /* 0x51C (Rage Wars: 0x5DC) */
} O01Actor;
typedef struct O01Sub8 {
               u8 pad0[4];
               s8 unk4;
} O01Sub8;
typedef struct O01Own8 {
               u8 pad0[0x1C];
               O01Actor *unk1C;
               O01Sub8 *unk20;
               s32 unk24;
} O01Own8;
extern s8 D_80130950[];
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130960;
extern s32 D_801309C4;
extern u8 D_80119870[];
extern u8 D_80119E2C[];
extern u8 D_00438748[];
extern u8 D_0043BDB0[];
extern u8 D_0043BE1C[];
s32 func_0041DD90(O01Own8 *o, s32 pad, s32 z);
O01Actor *func_0025342C(u8 *world, O01Sub8 *s);
void func_0042E430(O01Actor *a, s32 b);
void func_00416894(u8 *a, O01Own8 *o, u8 *c);
s32 func_00426064(s32 pad, s32 idx);
void func_00416644(u8 *a, void *b, void *c, void *d, void *e);
extern O01SaveSrc D_80130968;
/* Turok 2 inlines this (Rage Wars calls it) */
static inline void O01Restore(O01Actor *a) {
    O01Save *s = a->save;
    s32 i;
    s->b30 = D_80130968.b8;
    s->w[0] = D_80130968.w[0];
    s->w[1] = D_80130968.w[1];
    s->w[2] = D_80130968.w[2];
    s->w[3] = D_80130968.w[3];
    for (i = 0; i < 8; i++) {
        s->tail[i] = D_80130968.tail[i];
    }
}
s32 func_004206B8(s32 unused, O01Own8 *own) {
    s32 pad;
    O01Actor *plr;
    if (D_80130990 != 0) {
        pad = D_800C2028;
    } else {
        pad = own->unk20->unk4;
    }
    if (func_0041DD90(own, pad, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_80130960 != 0) {
        plr = func_0025342C(D_80119870, own->unk20);
        own->unk1C = plr;
        O01Restore(plr);
        func_0042E430(plr, D_80130950[0]);
        func_00416894(plr->unk51C + 0x53C, own, D_00438748);
        return 1;
    }
    if (func_00426064(pad, D_801309C4) == 0) {
        func_00416644(D_80119E2C, D_0043BDB0, own->unk1C, own->unk20, (void *)own->unk24);
    } else {
        func_00416644(D_80119E2C, D_0043BE1C, own->unk1C, own->unk20, (void *)own->unk24);
    }
    return 1;
}

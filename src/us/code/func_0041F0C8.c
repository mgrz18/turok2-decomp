/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00407578.c, func_00407578, with Turok 2's layouts. */
/* The result is tested through a copy (err = r): it stops jump threading before cse1, so the second r == 0 test survives as in the ROM. */
#include "common.h"
#include "structs/player.h"

/* Controller pak save: when the pak has room and an existing note can be replaced, writes the save note;
 * stops the pak sounds and shows the result (or the "not enough space" message). */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 pad;
} MenuPad;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ CPlayer *player;
    /* 0x20 */ MenuPad *pad;
} MenuOwner;

typedef struct {
    s32 v;
    s32 freeb;
    s32 cnt;
} PakInfo;

extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern s32 D_800F70A8;
extern s32 D_80130984;
extern s32 D_800C2010;
extern s32 D_800C2014;
extern s32 D_800C2018;
extern s32 D_800C201C;
extern char D_800B45CC[];
extern char D_800B45D8[];
extern u8 D_80119E2C[];
extern u8 D_0043C1C4[];
extern u8 D_0043BD68[];
extern u8 D_0043BDF8[];
extern u8 D_0043BEF4[];
u32 func_00426E6C(u32 n);
s32 func_004267A0(s32 pad, s32 *out);
s32 func_0042670C(s32 pad, s32 *out);
s32 func_00426A94(s32 pad, u8 *ext);
s32 func_00426164(s32 pad, s32 size, s32 buf, char *n1, u8 *ext, char *n2);
void func_002053A8(s32 a);
void func_002052D8(s32 a, s32 b);
void func_00412B14(void);
void func_00412438(s32 a);
void func_00416644(u8 *a, void *b, void *c, s32 d, s32 e);

static __inline__ void FreeAll(void) {
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

s32 func_0041F0C8(s32 unused, MenuOwner *own) {
    u8 ext[8];
    PakInfo pad;
    CPlayer *plr;
    s32 r = 0;
    s32 err;
    s32 ok = 1;
    s32 written = 0;
    s32 padv;
    s32 size;
    s32 need;
    s32 pv;
    CPlayer *pp;

    if (D_80130990 != 0) {
        pv = D_800C2028;
        pp = NULL;
    } else {
        pv = own->pad->pad;
        pp = own->player;
    }
    do {
        pad.v = pv;
    } while (0);
    plr = pp;
    size = D_800F70A8 + 0x688;
    if (D_80130984 != 0) {
        need = func_00426E6C(size);
        r = func_004267A0(pad.v, &pad.freeb);
        if (r == 0) {
            if (pad.freeb >= need) {
                r = func_0042670C(pad.v, &pad.cnt);
                if (r == 0 && pad.cnt != 0) {
                    ok = 0;
                    r = func_00426A94(pad.v, ext);
                    if (r == 0) {
                        written = 1;
                        padv = pad.v;
                        r = func_00426164(padv, size, D_800C2018, D_800B45CC, ext, D_800B45D8);
                    }
                }
            }
            if (ok != 0) {
                func_00416644(D_80119E2C, D_0043C1C4, plr, plr->m_ContState.m_pContState, own->unk14);
                return 0;
            }
        }
    }
    FreeAll();
    if (!written) {
        return 1;
    }
    if (r == 0) {
        func_00412B14();
        func_00412438(0x191);
    }
    err = r;
    if (err == 0) {
        func_00416644(D_80119E2C, D_0043BD68, plr, plr->m_ContState.m_pContState, (s32)D_0043BEF4);
    } else {
        func_00416644(D_80119E2C, D_0043BDF8, plr, plr->m_ContState.m_pContState, (s32)D_0043BEF4);
    }
    return 1;
}

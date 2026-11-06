/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407290.c, func_00407290.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/save.h"

typedef struct O01Pad { s32 v; s32 freeb; s32 cnt; } O01Pad;
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_80130984;
extern char D_80130968[];
extern char D_800B45C0[];
extern u8 D_800B45D8[];
extern u8 D_80119E2C[];
extern u8 D_0043BAFC[];
extern u8 D_0043BC88[];
extern u8 D_0043BD18[];
extern char D_0043ABA0[];
extern char D_0043ABC4[];
extern char D_0043ABE8[];
extern char D_0043AC0C[];
extern void *D_800C2010;
extern void *D_800C2014;
s32 Pak_BytesToPages_2(s32 n);
s32 Pak_GetFreePages_2(s32 pad, s32 *out);
s32 Pak_CountEmptyNotes_2(s32 pad, s32 *out);
s32 Pak_FindFreeExtLetter_2(s32 pad, u8 *ext);
s32 Pak_SaveNoteVerified_2(s32 pad, s32 size, char *n1, char *n2, u8 *ext, u8 *buf);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, void *d, void *e);
void CCache__Flush(s32 a);
void CCache__Deallocate(s32 a, void *p);
/* Controller-pak name for the selected pad (inlined). */
static __inline__ char *O01Pick(O01Pad *sel) {
    switch (sel->v) {
    case 0:
    default:
        return D_0043ABA0;
    case 1:
        return D_0043ABC4;
    case 2:
        return D_0043ABE8;
    case 3:
        return D_0043AC0C;
    }
}
/* Release the menu buffers (inlined). */
static __inline__ void O01FreeAll(void) {
    if (D_800C2010 != 0 || D_800C2014 != 0) {
        CCache__Flush(0);
    }
    if (D_800C2010 != 0) {
        CCache__Deallocate(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        CCache__Deallocate(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
}
s32 PakMenu_WriteProfile_2(s32 unused, Menu *own, s32 unused3) {
    CPlayer *plr;
    CPlayer *pp;
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
        pv = own->controller->port;
        pp = own->player;
    }
    pad.v = pv;
    plr = pp;
    if (D_80130984 != 0) {
        need = Pak_BytesToPages_2(0x18);
        r = Pak_GetFreePages_2(pad.v, &pad.freeb);
        if (r == 0) {
            if (pad.freeb >= need) {
                r = Pak_CountEmptyNotes_2(pad.v, &pad.cnt);
                if (r == 0 && pad.cnt != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter_2(pad.v, ext);
                    if (r == 0) {
                        flag = 1;
                        nv = pad.v;
                        r = Pak_SaveNoteVerified_2(nv, 0x18, D_80130968, D_800B45C0, ext, D_800B45D8);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu_2(D_80119E2C, D_0043BAFC, plr, plr->m_ContState.m_pContState, own->desc);
                return 0;
            }
        }
    }
    O01FreeAll();
    if (flag != 0) {
        if (r == 0) {
            MenuPool_OpenMenu_2(D_80119E2C, D_0043BC88, plr, plr->m_ContState.m_pContState, O01Pick(&pad));
        } else {
            MenuPool_OpenMenu_2(D_80119E2C, D_0043BD18, plr, plr->m_ContState.m_pContState, O01Pick(&pad));
        }
    }
    return 1;
}

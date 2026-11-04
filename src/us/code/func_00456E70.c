/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"

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
extern PakProfile D_80130968;
extern u8 D_80119E2C[];
extern u8 D_0043BB68[];
extern u8 D_0043BF7C[];
extern u8 D_0043BB20[];
extern u8 D_0043B5EC[];
void func_004263A0(s32 ch);
s32 func_0042647C(s32 ch);
s32 func_004264B4(s32 ch);
s32 func_00426AF8(s32 ch);
void func_0041D948(Menu *o);
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
static inline void O01Capture(CPlayer *a) {
    CPlayerOptions *p = a->m_pOptions;
    s32 i;
    D_80130968.character = p->m_nCharacter;
    D_80130968.suicides = p->m_Suicides;
    D_80130968.deaths = p->m_Deaths;
    D_80130968.kills = p->m_Kills;
    D_80130968.tags = p->m_Tags;
    D_80130968.reserved[0] = 0;
    D_80130968.reserved[1] = 0;
    D_80130968.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        D_80130974[i] = p->m_Name[i];
    }
}
void func_00456E70(Menu *own) {
    CPlayer *plr;
    O01Sel sel;
    u8 *msg;
    s32 pad;
    CPlayer *pp;
    s32 pv;
    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 1;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    pv = own->controller->port;
    pp = own->player;
    sel.v = pv;
    plr = pp;
    msg = O01Pick(&sel);
    if (D_800C2020 != 0) {
        D_801309BC = 1;
        func_00416644((u8 *)own->player->m_pCamera + 0x53C, msg, plr, plr->m_ContState.m_pContState, 0);
    } else {
        if (D_80130990 != 0) {
            pad = D_800C2028;
        } else {
            pad = own->controller->port;
        }
        func_004263A0(pad);
        if (func_0042647C(sel.v) == 0) {
            O01Capture(plr);
            D_80130968.checksum = func_00426D98((u8 *)&D_80130968, 0x14, 4);
            func_0041D948(own);
        } else {
            D_801309BC = 1;
            if (func_004264B4(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043BB68, plr, plr->m_ContState.m_pContState, msg);
            } else if (func_00426AF8(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043BF7C, plr, plr->m_ContState.m_pContState, D_0043B5EC);
            } else {
                func_00416644(D_80119E2C, D_0043BB20, plr, plr->m_ContState.m_pContState, msg);
            }
        }
    }
}

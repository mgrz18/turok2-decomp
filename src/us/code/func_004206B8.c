/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00408C78.c, func_00408C78.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/game.h"

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs
/pak.h"

extern s8 D_80130950[];
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130960;
extern s32 D_801309C4;
extern u8 D_80119E2C[];
extern u8 D_00438748[];
extern u8 D_0043BDB0[];
extern u8 D_0043BE1C[];
s32 func_0041DD90(Menu *o, s32 pad, s32 z);
CPlayer *func_0025342C(CPlayerList *world, MenuController *s);
void func_0042E430(CPlayer *a, s32 b);
void func_00416894(u8 *a, Menu *o, u8 *c);
s32 func_00426064(s32 pad, s32 idx);
void func_00416644(u8 *a, void *b, void *c, void *d, void *e);
extern PakProfile D_80130968;
/* Turok 2 inlines this (Rage Wars calls it) */
static inline void O01Restore(CPlayer *a) {
    CPlayerOptions *s = a->m_pOptions;
    s32 i;
    s->m_nCharacter = D_80130968.character;
    s->m_Suicides = D_80130968.suicides;
    s->m_Deaths = D_80130968.deaths;
    s->m_Kills = D_80130968.kills;
    s->m_Tags = D_80130968.tags;
    for (i = 0; i < 8; i++) {
        s->m_Name[i] = D_80130968.name[i];
    }
}
s32 func_004206B8(s32 unused, Menu *own) {
    s32 pad;
    CPlayer *plr;
    if (D_80130990 != 0) {
        pad = D_800C2028;
    } else {
        pad = own->controller->port;
    }
    if (func_0041DD90(own, pad, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_80130960 != 0) {
        plr = func_0025342C(&D_80119870.m_PlayerList, own->controller);
        own->player = plr;
        O01Restore(plr);
        func_0042E430(plr, D_80130950[0]);
        func_00416894((u8 *)plr->m_pCamera + 0x53C, own, D_00438748);
        return 1;
    }
    if (func_00426064(pad, D_801309C4) == 0) {
        func_00416644(D_80119E2C, D_0043BDB0, own->player, own->controller, (void *)own->context);
    } else {
        func_00416644(D_80119E2C, D_0043BE1C, own->player, own->controller, (void *)own->context);
    }
    return 1;
}

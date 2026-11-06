/* WRONG CUT: real function 0x0041E1D8, size 0x19C (through 0x0041E374): splat cut it in two
 * (PakMenu_InitLoadProfile, func_0041E294) at a jal's delay slot. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_801309A0;
extern s32 D_801309B4;
extern s32 D_801309BC;
extern s32 D_800C2020;
extern s32 D_800C2024;
extern u8 D_80119E2C[];   /* menu pool */
extern u8 D_0043882C[];
extern u8 D_0043B6A8[];
extern u8 D_0043BC00[];
extern u8 D_0043BC6C[];
extern u8 D_0043C05C[];

extern void MenuPool_OpenMenu(void *pool, void *tmpl, CPlayer *player, ControllerProfile *ctl, void *back);
extern void Pak_ProbeSlot(s32 kind);
extern s32 Pak_GetStatus(s32 kind);
extern s32 Pak_IsRumblePak(s32 kind);
extern s32 Pak_NeedsRepair(s32 kind);
extern void PakMenu_UpdateNoteRows(Menu *m);

void PakMenu_InitLoadProfile(Menu *m) {
    s32 kind;

    D_80130984 = 0;
    D_801309A0 = 0;
    D_80130994 = 1;
    D_801309B4 = 1;
    D_800C2024 = 1;
    D_801309BC = 0;
    D_80130990 = 0;
    m->items[20].flags &= ~0x1000000;
    kind = m->controller->port;
    if (D_800C2020) {
        D_801309BC = 1;
        MenuPool_OpenMenu(((u8 *)m->player->m_pCamera) + 0x53C, D_0043882C, m->player, m->controller, NULL);
        return;
    }
    Pak_ProbeSlot(kind);
    if (!Pak_GetStatus(kind)) {
        PakMenu_UpdateNoteRows(m);
        return;
    }
    D_801309BC = 1;
    if (Pak_IsRumblePak(kind)) {
        MenuPool_OpenMenu(D_80119E2C, D_0043BC6C, m->player, m->controller, D_0043882C);
    } else if (Pak_NeedsRepair(kind)) {
        MenuPool_OpenMenu(D_80119E2C, D_0043C05C, m->player, m->controller, D_0043B6A8);
    } else {
        MenuPool_OpenMenu(D_80119E2C, D_0043BC00, m->player, m->controller, D_0043882C);
    }
}

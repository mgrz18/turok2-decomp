/* First-module twin of src/us/code/PakMenu_SelectCancel_2.c. WRONG CUT: REAL PakMenu_SelectCancel size 0x2B8 (0x0042019C-0x00420454); splat cut its tail into func_004203B0. Matches whole (checked with the size overridden). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004085E0.c, func_004085E0, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/save.h"

/* Controller pak menu: stops the pak sounds and shows the message for the pak's state. */

extern s32 D_800C2010;
extern s32 D_800C2014;
extern s32 D_80130940;
extern s32 D_80130990;
extern s32 D_801309B4;
extern s32 D_80130994;
extern s32 D_80130984;
extern s32 D_801309BC;
extern s32 D_800C2028; /* pad */
extern s32 D_8011F148;
extern s32 D_8011B108;
extern s32 D_8011F144;
extern s32 D_8011B114;
extern u8 D_80119E2C[];
extern u8 D_0043B738[];
extern u8 D_00438808[];
extern u8 D_00439190[];
extern u8 D_0043AC80[];
extern u8 D_0043ACA4[];
extern u8 D_0043ACC8[];
extern u8 D_0043ACEC[];
extern u8 D_0043BEF4[];
void CCache__Flush(s32 a);
void CCache__Deallocate(s32 a, s32 b);
CPlayer *CPlayerPool__FindByController(void *a, void *b);
void MenuPool_OpenMenu(void *a, void *b, s32 c, s32 d, s32 e);

s32 PakMenu_SelectCancel(s32 a0, Menu *arg) {
    struct {
        s32 m;
        s32 pad;
    } mm;
    u8 *tbl;
    CPlayer *plr;
    s32 sw;
    CPlayer *u;

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
    D_80130940 = 0;
    if (D_80130990 != 0) {
        mm.m = D_800C2028;
    } else {
        mm.m = arg->controller->port;
    }
    if (D_80130990 != 0) {
        D_8011F148 = 0;
        D_8011B108 = 0;
        D_8011F144 = 1;
        D_8011B114 = 0;
        MenuPool_OpenMenu(D_80119E2C, D_0043B738, 0, (s32)arg->controller, 0);
        return 1;
    }
    if (D_801309B4 != 0) {
        if (D_80130994 != 0) {
            D_801309BC = 1;
            plr = CPlayerPool__FindByController(&D_80119870.m_PlayerList, arg->controller);
            MenuPool_OpenMenu(((u8 *)plr->m_pCamera) + 0x53C, D_00438808, (s32)plr, plr->m_ContState.m_pContState, mm.m);
            return 1;
        }
        if (D_80130984 != 0) {
            D_801309BC = 1;
            u = (CPlayer *)arg->player;
            sw = arg->controller->port;
            mm.m = sw;
            plr = u;
            switch (sw) {
            case 0:
            default:
                tbl = D_0043AC80;
                break;
            case 1:
                tbl = D_0043ACA4;
                break;
            case 2:
                tbl = D_0043ACC8;
                break;
            case 3:
                tbl = D_0043ACEC;
                break;
            }
            MenuPool_OpenMenu(((u8 *)plr->m_pCamera) + 0x53C, tbl, (s32)plr, plr->m_ContState.m_pContState, mm.m);
            return 1;
        }
        return 0;
    }
    plr = CPlayerPool__FindByController(&D_80119870.m_PlayerList, arg->controller);
    if (D_80130994 != 0) {
        D_801309BC = 1;
        MenuPool_OpenMenu(D_80119870.m_MenuPool, D_00439190, (s32)plr, plr->m_ContState.m_pContState, mm.m);
        return 1;
    }
    if (D_80130984 == 0) {
        return 0;
    } else {
        D_801309BC = 1;
        MenuPool_OpenMenu(((u8 *)plr->m_pCamera) + 0x53C, D_0043BEF4, (s32)plr, plr->m_ContState.m_pContState, mm.m);
        D_80130940 = 0;
        return 1;
    }
}

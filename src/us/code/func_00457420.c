/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004069F4.c, func_004069F4, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "structs/game.h"
#include "globals/menu_desc.h"

/* Controller pak menu, load: finds a pad with a controller pak (trying each pad in turn), checks it for the
 * game's notes and free space and opens the load menu, or shows why the pak cannot be used. */

extern s32 D_8011F144;
extern s32 D_8011B114;
extern s32 D_800F70A8;
s32 Controller_AnyConnected(void);
void CEngineApp__UpdateControllers(u8 *a);
s32 Str_Compare_2(u8 *a, char *b);
void Pak_ProbeSlot_2(s32 a);
s32 Pak_GetStatus_2(s32 a);
s32 Pak_CountEmptyNotes_2(s32 a, s32 *b);
s32 Pak_GetFreePages_2(s32 a, s32 *b);
void Pak_GetNoteGameName_2(s32 a, s32 b, sptr *c);
s32 Pak_NeedsRepair_2(s32 a);
void func_0045FBA8(sptr a, u8 *b, s32 c);
s32 Pak_BytesToPages_2(s32 a);
void PakMenu_UpdateNoteRows_2(Menu *o);
s32 PakMenu_CheckSlotForSave_2(Menu *o, s32 a, s32 *b, s32 *c);
void MenuPool_OpenMenu_2(u8 *a, u8 *b, sptr c, ControllerProfile *d, void *e);

#define PAK_ERROR()                                                        \
    gPakMenuLeaving = 1;                                                        \
    D_8011F144 = 0;                                                        \
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgCannotSaveDesc_2, arg0->player, arg0->controller, NULL); \
    gPakCheckHasPak = 0;                                                        \
    sPakMenuStartLatched = 1;

void PakMenu_InitSaveGameAnyPak_2(Menu *arg0) {
    u8 buf[16];
    s32 sp28, sp2C;
    sptr sp30, sp34;
    s32 sp38, sp3C;
    s32 s0, s1, s2, s3, f1, v0, m1, r2;

    gPakMenuIsSave = 0;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 0;
    gPakMenuMode = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 1;
    if (Controller_AnyConnected() == 0) {
        gPakMenuLeaving = 1;
        D_8011F144 = 0;
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgNoControllerDesc_2, arg0->player, arg0->controller, NULL);
        sPakMenuStartLatched = 1;
        gPakCheckHasPak = 0;
        return;
    }
    s0 = 0;
    if (gPakMenuPort == -1) {
        s1 = 0;
        do {
            Pak_ProbeSlot_2(s0);
            if (Pak_GetStatus_2(s0) != -2) {
                break;
            }
            s0++;
            if (s0 >= 4) {
                s0 = 0;
            }
            s1++;
        } while (s1 < 4);
        if (s1 == 4) {
            s0 = -1;
        }
        gPakMenuPort = s0;
        if (s0 == -1) {
            PAK_ERROR()
            return;
        }
    }
    if (Pak_NeedsRepair_2(gPakMenuPort) != 0) {
        goto blk59;
    }
    CEngineApp__UpdateControllers(&gEngineApp);
    if ((arg0->controller->buttons & 0x1000) && sPakMenuStartLatched == 0) {
        gPakMenuStartPressed = 1;
        sPakMenuStartLatched = 1;
    }
    sPakMenuStartLatched = 1;
    if (D_8011B114 != 0 || gPakMenuStartPressed != 0) {
        Pak_ProbeSlot_2(gPakMenuPort);
        if (Pak_GetStatus_2(gPakMenuPort) != -2) {
            goto fin;
        }
        PAK_ERROR()
        return;
    }
    s3 = 0;
    s2 = s3;
    sp2C = -1;
    sp28 = gPakMenuPort;
    do {
        if (gPakMenuPort != -1) {
            s3 = PakMenu_CheckSlotForSave_2(arg0, gPakMenuPort, &sp28, &sp2C);
            if (s3 != 0) {
                goto found;
            }
        }
        s0 = 0;
        if (gPakMenuPort != -1) {
            s0 = gPakMenuPort + 1;
            if (s0 >= 4) {
                s0 = 0;
            }
        }
        s1 = 0;
        do {
            Pak_ProbeSlot_2(s0);
            if (Pak_GetStatus_2(s0) != -2) {
                break;
            }
            s0++;
            if (s0 >= 4) {
                s0 = 0;
            }
            s1++;
        } while (s1 < 4);
        if (s1 == 4) {
            s0 = -1;
        }
        gPakMenuPort = s0;
        s2++;
    } while (s2 < 4);
    if (s3 == 0) {
        gPakMenuPort = sp28;
        Pak_ProbeSlot_2(sp28);
        if (Pak_GetStatus_2(gPakMenuPort) == -2) {
            PAK_ERROR()
            return;
        }
    }
found:
    s1 = gPakMenuPort;
    s0 = 0;
    do {
        Pak_GetNoteGameName_2(s1, s0, &sp30);
        func_0045FBA8(sp30, buf, 0x10);
        if (Str_Compare_2(buf, gPakCharacterNoteName) == 0) {
            f1 = 1;
            goto L2;
        }
        s0++;
    } while (s0 < 0x10);
    f1 = 0;
L2:
    s1 = gPakMenuPort;
    s0 = 0;
    do {
        Pak_GetNoteGameName_2(s1, s0, &sp34);
        func_0045FBA8(sp34, buf, 0x10);
        if (Str_Compare_2(buf, gPakGameNoteName) == 0) {
            v0 = 1;
            goto L3;
        }
        s0++;
    } while (s0 < 0x10);
    v0 = 0;
L3:
    if (f1 != 0) {
        goto fin;
    }
    if (v0 != 0) {
        goto fin;
    }
    if (Pak_GetFreePages_2(gPakMenuPort, &sp38) != 0) {
        goto blk59;
    }
    s0 = Pak_CountEmptyNotes_2(gPakMenuPort, &sp3C);
    if (s0 != 0) {
        goto blk59;
    }
    m1 = Pak_BytesToPages_2(D_800F70A8 + 0x688);
    r2 = Pak_BytesToPages_2(0x18);
    if (sp3C == 0 || sp38 < r2) {
        goto L56;
    }
    if (sp38 < m1) {
        gPakMenuLeaving = 1;
        gPakMenuStartPressed = 1;
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakConfirmNoSpaceAllDesc_2, arg0->player, arg0->controller, (u8 *)&gPakSaveGameAnyPakMenuDesc_2);
        goto after;
    }
    if (sp38 < r2) {
        goto L56;
    }
    goto after;
L56:
    gPakMenuLeaving = 1;
    gPakMenuStartPressed = 1;
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakConfirmNoSpaceDesc_2, arg0->player, arg0->controller, (u8 *)&gPakSaveGameAnyPakMenuDesc_2);
after:
    if (s0 == 0) {
        goto fin;
    }
blk59:
    gPakMenuLeaving = 1;
    gPakMenuStartPressed = 1;
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gDamagedPakAtBootMenuDesc_2, arg0->player, arg0->controller, (u8 *)&gPakSaveGameAnyPakMenuDesc_2);
fin:
    if (gPakMenuLeaving == 0) {
        PakMenu_UpdateNoteRows_2(arg0);
    }
    gPakCheckHasPak = 0;
    sPakMenuStartLatched = 1;
}

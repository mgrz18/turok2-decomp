/* REAL PakMenu_InitSaveGameAnyPak size 0x584 (0x0041E500-0x0041EA84); splat cut it into PakMenu_InitSaveGameAnyPak + func_0041E7D0 + func_0041E824 + func_0041E97C. Matches whole (checked with the size overridden). Twin of src/us/code/PakMenu_InitSaveGameAnyPak_2.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004069F4.c, func_004069F4, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

/* Controller pak menu, load: finds a pad with a controller pak (trying each pad in turn), checks it for the
 * game's notes and free space and opens the load menu, or shows why the pak cannot be used. */

extern s32 gPakMenuIsSave;
extern s32 gPakMenuWrites;
extern s32 gPakMenuIsLoad;
extern s32 gPakMenuIsProfile;
extern s32 gPakMenuMode;
extern s32 gPakMenuLeaving;
extern s32 gPakMenuAnyPak;
extern s32 D_8011F144;
extern s32 sPakMenuStartLatched;
extern s32 gPakCheckHasPak;
extern s32 gPakMenuPort; /* pad */
extern s32 gPakMenuStartPressed;
extern s32 D_8011B114;
extern char gPakCharacterNoteName[];
extern char gPakGameNoteName[];
extern s32 D_800F70A8;
extern u8 gEngineApp[];
extern u8 gPakMsgNoControllerDesc[];
extern u8 gPakMsgCannotSaveDesc[];
extern u8 gDamagedPakAtBootMenuDesc[];
extern u8 gPakSaveGameAnyPakMenuDesc[];
extern u8 gPakConfirmNoSpaceAllDesc[];
extern u8 gPakConfirmNoSpaceDesc[];
s32 Controller_AnyConnected(void);
void CEngineApp__UpdateControllers(u8 *a);
s32 Str_Compare(u8 *a, char *b);
void Pak_ProbeSlot(s32 a);
s32 Pak_GetStatus(s32 a);
s32 Pak_CountEmptyNotes(s32 a, s32 *b);
s32 Pak_GetFreePages(s32 a, s32 *b);
void Pak_GetNoteGameName(s32 a, s32 b, s32 *c);
s32 Pak_NeedsRepair(s32 a);
void Pak_DecodeText(s32 a, u8 *b, s32 c);
s32 Pak_BytesToPages(s32 a);
void PakMenu_UpdateNoteRows(Menu *o);
s32 PakMenu_CheckSlotForSave(Menu *o, s32 a, s32 *b, s32 *c);
void MenuPool_OpenMenu(u8 *a, u8 *b, s32 c, ControllerProfile *d, void *e);

#define PAK_ERROR()                                                        \
    gPakMenuLeaving = 1;                                                        \
    D_8011F144 = 0;                                                        \
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakMsgCannotSaveDesc, arg0->player, arg0->controller, NULL); \
    gPakCheckHasPak = 0;                                                        \
    sPakMenuStartLatched = 1;

void PakMenu_InitSaveGameAnyPak(Menu *arg0) {
    u8 buf[16];
    s32 sp28, sp2C, sp30, sp34, sp38, sp3C;
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
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakMsgNoControllerDesc, arg0->player, arg0->controller, NULL);
        sPakMenuStartLatched = 1;
        gPakCheckHasPak = 0;
        return;
    }
    s0 = 0;
    if (gPakMenuPort == -1) {
        s1 = 0;
        do {
            Pak_ProbeSlot(s0);
            if (Pak_GetStatus(s0) != -2) {
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
    if (Pak_NeedsRepair(gPakMenuPort) != 0) {
        goto blk59;
    }
    CEngineApp__UpdateControllers(gEngineApp);
    if ((arg0->controller->buttons & 0x1000) && sPakMenuStartLatched == 0) {
        gPakMenuStartPressed = 1;
        sPakMenuStartLatched = 1;
    }
    sPakMenuStartLatched = 1;
    if (D_8011B114 != 0 || gPakMenuStartPressed != 0) {
        Pak_ProbeSlot(gPakMenuPort);
        if (Pak_GetStatus(gPakMenuPort) != -2) {
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
            s3 = PakMenu_CheckSlotForSave(arg0, gPakMenuPort, &sp28, &sp2C);
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
            Pak_ProbeSlot(s0);
            if (Pak_GetStatus(s0) != -2) {
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
        Pak_ProbeSlot(sp28);
        if (Pak_GetStatus(gPakMenuPort) == -2) {
            PAK_ERROR()
            return;
        }
    }
found:
    s1 = gPakMenuPort;
    s0 = 0;
    do {
        Pak_GetNoteGameName(s1, s0, &sp30);
        Pak_DecodeText(sp30, buf, 0x10);
        if (Str_Compare(buf, gPakCharacterNoteName) == 0) {
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
        Pak_GetNoteGameName(s1, s0, &sp34);
        Pak_DecodeText(sp34, buf, 0x10);
        if (Str_Compare(buf, gPakGameNoteName) == 0) {
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
    if (Pak_GetFreePages(gPakMenuPort, &sp38) != 0) {
        goto blk59;
    }
    s0 = Pak_CountEmptyNotes(gPakMenuPort, &sp3C);
    if (s0 != 0) {
        goto blk59;
    }
    m1 = Pak_BytesToPages(D_800F70A8 + 0x688);
    r2 = Pak_BytesToPages(0x18);
    if (sp3C == 0 || sp38 < r2) {
        goto L56;
    }
    if (sp38 < m1) {
        gPakMenuLeaving = 1;
        gPakMenuStartPressed = 1;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakConfirmNoSpaceAllDesc, arg0->player, arg0->controller, gPakSaveGameAnyPakMenuDesc);
        goto after;
    }
    if (sp38 < r2) {
        goto L56;
    }
    goto after;
L56:
    gPakMenuLeaving = 1;
    gPakMenuStartPressed = 1;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakConfirmNoSpaceDesc, arg0->player, arg0->controller, gPakSaveGameAnyPakMenuDesc);
after:
    if (s0 == 0) {
        goto fin;
    }
blk59:
    gPakMenuLeaving = 1;
    gPakMenuStartPressed = 1;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc, arg0->player, arg0->controller, gPakSaveGameAnyPakMenuDesc);
fin:
    if (gPakMenuLeaving == 0) {
        PakMenu_UpdateNoteRows(arg0);
    }
    gPakCheckHasPak = 0;
    sPakMenuStartLatched = 1;
}

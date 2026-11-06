/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004082FC.c, func_004082FC, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

/* Controller pak menu, confirm: checks the pak, then saves (replacing the game's note when one exists, with a
 * message when it is in use) or, without a pak, looks for the next pad that has one. */

extern s32 gPakMenuIsSave;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuIsProfile;
extern s32 gPakMenuLeaving;
extern s32 gPakNoteIndex; /* note index */
extern char gPakGameNoteName[];
extern s32 gPakCheckDone;
extern u8 gEngineApp_m_Game_m_MenuPool[];
extern u8 gPakConfirmOverwriteDesc_2[];
extern u8 gPakSaveGameMenuDesc_2[];
s32 Str_Compare_2(u8 *a, char *b);
void Pak_ProbeSlot_2(s32 a);
s32 Pak_GetStatus_2(s32 a);
void Pak_GetNoteGameName_2(s32 a, s32 b, s32 *c);
void func_0045FBA8(s32 a, u8 *b, s32 c);
void PakMenu_UpdateNoteRows_2(Menu *o);
s32 PakMenu_CheckPak_2(Menu *o, s32 a, s32 b);
void PakMenu_WriteProfile_2(s32 a, Menu *o, s32 c);
void PakMenu_WriteSaveGame_2(s32 a, Menu *o, s32 c);
void MenuPool_OpenMenu_2(void *a, void *b, s32 c, ControllerProfile *d, void *e);

s32 PakMenu_SelectPak_2(s32 a0, Menu *o, s32 a2) {
    u8 buf[16];
    struct {
        s32 pad;
        s32 v;
    } L;
    s32 s0, s1, found;

    L.pad = 0;
    if (gPakMenuIsSave != 0) {
        if (gPakMenuAnyPak != 0) {
            L.pad = gPakMenuPort;
        } else {
            L.pad = o->controller->port;
        }
    }
    if (gPakMenuAnyPak != 0) {
        s1 = gPakMenuPort;
    } else {
        s1 = L.pad;
    }
    if (PakMenu_CheckPak_2(o, s1, 0) != 0) {
        goto done;
    }
    if (gPakMenuIsProfile != 0) {
        if (gPakMenuIsSave != 0) {
            PakMenu_WriteProfile_2(a0, o, a2);
            return 1;
        }
    } else if (gPakMenuIsSave != 0) {
        s32 *index;

        gPakNoteIndex = -1;
        /* Probably the optional out-pointer of an inlined search helper: tested through a local, stored by symbol. */
        index = &gPakNoteIndex;
        s0 = 0;
        do {
            Pak_GetNoteGameName_2(s1, s0, &L.v);
            func_0045FBA8(L.v, buf, 0x10);
            if (Str_Compare_2(buf, gPakGameNoteName) == 0) {
                found = 1;
                if (index != NULL) {
                    gPakNoteIndex = s0;
                }
                goto after;
            }
            s0++;
        } while (s0 < 0x10);
        found = 0;
    after:
        if (found != 0 && gPakNoteIndex >= 0 && gPakNoteIndex < 0x10) {
            MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, gPakConfirmOverwriteDesc_2, o->player, o->controller, gPakSaveGameMenuDesc_2);
            return 1;
        }
        PakMenu_WriteSaveGame_2(a0, o, a2);
        return 1;
    }
    if (gPakMenuAnyPak != 0) {
        s32 r = gPakMenuPort;

        s0 = 0;
        if (r != -1) {
            s0 = r + 1;
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
        if (s0 == -1) {
            gPakCheckDone = gPakMenuLeaving = 1;
            return 1;
        }
        {
            s32 a1 = s0;

            if (gPakMenuAnyPak == 0) {
                a1 = o->controller->port;
            }
            L.pad = a1;
            if (PakMenu_CheckPak_2(o, a1, 0) != 0) {
            done:
                gPakMenuLeaving = 1;
                return 1;
            }
            PakMenu_UpdateNoteRows_2(o);
        }
    }
    return 0;
}

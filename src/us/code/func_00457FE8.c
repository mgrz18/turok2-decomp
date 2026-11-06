/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00407578.c, func_00407578, with Turok 2's layouts. */
/* Second VM module copy of PakMenu_WriteSaveGame (same C, second-module symbols). */
/* The result is tested through a copy (err = r): it stops jump threading before cse1, so the second r == 0 test survives as in the ROM. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"
#include "structs/save.h"

/* Controller pak save: when the pak has room and an existing note can be replaced, writes the save note;
 * stops the pak sounds and shows the result (or the "not enough space" message). */

extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort; /* pad */
extern s32 D_800F70A8;
extern s32 gPakMenuIsSave;
extern s32 gSaveBufferHandle;
extern s32 gLoadBufferHandle;
extern char gPakGameNoteName[];
extern char gPakGameCode[];
extern u8 gEngineApp_m_Game_m_MenuPool[];
extern u8 gPakConfirmNoSpaceDesc_2[];
extern u8 gPakMsgSaveDoneDesc_2[];
extern u8 gPakMsgSaveFailedDesc_2[];
extern u8 gSavePortalMenuDescB_2[];
u32 Pak_BytesToPages_2(u32 n);
s32 Pak_GetFreePages_2(s32 pad, s32 *out);
s32 Pak_CountEmptyNotes_2(s32 pad, s32 *out);
s32 Pak_FindFreeExtLetter_2(s32 pad, u8 *ext);
s32 Pak_SaveNoteVerified_2(s32 pad, s32 size, s32 buf, char *n1, u8 *ext, char *n2);
void CCache__Flush(s32 a);
void CCache__Deallocate(s32 a, s32 b);
void Cinema_StopAtEnd_2(void);
void Cinema_RequestById_2(s32 a);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, s32 d, s32 e);

static __inline__ void FreeAll(void) {
    if (gSaveBufferHandle != 0 || gLoadBufferHandle != 0) {
        CCache__Flush(0);
    }
    if (gSaveBufferHandle != 0) {
        CCache__Deallocate(0, gSaveBufferHandle);
    }
    if (gLoadBufferHandle != 0) {
        CCache__Deallocate(0, gLoadBufferHandle);
    }
    gSaveBufferHandle = 0;
    gLoadBufferHandle = 0;
    gSaveBuffer = 0;
    gLoadBuffer = 0;
}

s32 PakMenu_WriteSaveGame_2(s32 unused, Menu *own) {
    u8 ext[8];
    PakQuery pad;
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

    if (gPakMenuAnyPak != 0) {
        pv = gPakMenuPort;
        pp = NULL;
    } else {
        pv = own->controller->port;
        pp = own->player;
    }
    do {
        pad.channel = pv;
    } while (0);
    plr = pp;
    size = D_800F70A8 + 0x688;
    if (gPakMenuIsSave != 0) {
        need = Pak_BytesToPages_2(size);
        r = Pak_GetFreePages_2(pad.channel, &pad.freeBytes);
        if (r == 0) {
            if (pad.freeBytes >= need) {
                r = Pak_CountEmptyNotes_2(pad.channel, &pad.freeNotes);
                if (r == 0 && pad.freeNotes != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter_2(pad.channel, ext);
                    if (r == 0) {
                        written = 1;
                        padv = pad.channel;
                        r = Pak_SaveNoteVerified_2(padv, size, gSaveBuffer, gPakGameNoteName, ext, gPakGameCode);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, gPakConfirmNoSpaceDesc_2, plr, plr->m_ContState.m_pContState, own->desc);
                return 0;
            }
        }
    }
    FreeAll();
    if (!written) {
        return 1;
    }
    if (r == 0) {
        Cinema_StopAtEnd_2();
        Cinema_RequestById_2(0x191);
    }
    err = r;
    if (err == 0) {
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, gPakMsgSaveDoneDesc_2, plr, plr->m_ContState.m_pContState, (s32)gSavePortalMenuDescB_2);
    } else {
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, gPakMsgSaveFailedDesc_2, plr, plr->m_ContState.m_pContState, (s32)gSavePortalMenuDescB_2);
    }
    return 1;
}

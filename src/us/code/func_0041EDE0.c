/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407290.c, func_00407290.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/save.h"
#include "globals/pak.h"

typedef struct O01Pad { s32 v; s32 freeb; s32 cnt; } O01Pad;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuIsSave;
extern u8 gPakProfile[];
extern char gPakCharacterNoteName[];
extern char gPakGameCode[];
extern u8 gEngineApp_m_Game_m_MenuPool[];
extern u8 gPakMsgPakFullDesc[];
extern u8 gPakMsgSaveDoneDesc[];
extern u8 gPakMsgSaveFailedDesc[];
s32 Pak_BytesToPages(s32 n);
s32 Pak_GetFreePages(s32 pad, s32 *out);
s32 Pak_CountEmptyNotes(s32 pad, s32 *out);
s32 Pak_FindFreeExtLetter(s32 pad, u8 *ext);
s32 Pak_SaveNoteVerified(s32 pad, s32 size, u8 *buf, char *n1, u8 *ext, char *n2);
void MenuPool_OpenMenu(u8 *a, void *b, void *c, void *d, void *e);
extern u8 gStatsAfterPakMenuDesc1[];
extern u8 gStatsAfterPakMenuDesc2[];
extern u8 gStatsAfterPakMenuDesc3[];
extern u8 gStatsAfterPakMenuDesc4[];
void CCache__Flush(s32 a);
void CCache__Deallocate(s32 a, s32 b);
/* Static inlines in the Rage Wars source file (dropped by the importer). */
static inline u8 *O01Pick(O01Pad *sel) {
    u8 *p;
    switch (sel->v) {
    default:
    case 0:
        p = gStatsAfterPakMenuDesc1;
        break;
    case 1:
        p = gStatsAfterPakMenuDesc2;
        break;
    case 2:
        p = gStatsAfterPakMenuDesc3;
        break;
    case 3:
        p = gStatsAfterPakMenuDesc4;
        break;
    }
    return p;
}
static inline void O01FreeAll(void) {
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
s32 PakMenu_WriteProfile(s32 unused, Menu *own, s32 unused3) {
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
    if (gPakMenuAnyPak != 0) {
        pv = gPakMenuPort;
        pp = 0;
    } else {
        pv = own->controller->port;
        pp = own->player;
    }
    pad.v = pv;
    plr = pp;
    if (gPakMenuIsSave != 0) {
        need = Pak_BytesToPages(0x18);
        r = Pak_GetFreePages(pad.v, &pad.freeb);
        if (r == 0) {
            if (pad.freeb >= need) {
                r = Pak_CountEmptyNotes(pad.v, &pad.cnt);
                if (r == 0 && pad.cnt != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter(pad.v, ext);
                    if (r == 0) {
                        flag = 1;
                        nv = pad.v;
                        r = Pak_SaveNoteVerified(nv, 0x18, gPakProfile, gPakCharacterNoteName, ext, gPakGameCode);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gPakMsgPakFullDesc, plr, plr->m_ContState.m_pContState, own->desc);
                return 0;
            }
        }
    }
    O01FreeAll();
    if (flag != 0) {
        if (r == 0) {
            MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gPakMsgSaveDoneDesc, plr, plr->m_ContState.m_pContState, O01Pick(&pad));
        } else {
            MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gPakMsgSaveFailedDesc, plr, plr->m_ContState.m_pContState, O01Pick(&pad));
        }
    }
    return 1;
}

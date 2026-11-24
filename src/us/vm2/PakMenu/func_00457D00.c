/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407290.c, func_00407290.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/pak_2.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/save.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/pak.h"
#include "protos/pak.h"
#include "globals/pak.h"

extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */
extern void CCache__Deallocate(sptr, void *);
/* Static inlines in the Rage Wars source file (dropped by the importer). */
static inline u8 *O01Pick(PakQuery *sel) {
    u8 *p;
    switch (sel->channel) {
    default:
    case 0:
        p = (u8 *)&gStatsAfterPakMenuDesc1_2;
        break;
    case 1:
        p = (u8 *)&gStatsAfterPakMenuDesc2_2;
        break;
    case 2:
        p = (u8 *)&gStatsAfterPakMenuDesc3_2;
        break;
    case 3:
        p = (u8 *)&gStatsAfterPakMenuDesc4_2;
        break;
    }
    return p;
}
static inline void O01FreeAll(void) {
    if (gSaveBufferHandle != 0 || gLoadBufferHandle != 0) {
        CCache__Flush_Arg(0);
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
s32 PakMenu_WriteProfile_2(sptr unused, Menu *own, s32 unused3) {
    CPlayer *plr;
    CPlayer *pp;
    s32 pv;
    u8 ext[8];
    PakQuery pad;
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
    pad.channel = pv;
    plr = pp;
    if (gPakMenuIsSave != 0) {
        need = Pak_BytesToPages_2(0x18);
        r = Pak_GetFreePages_2(pad.channel, &pad.freeBytes);
        if (r == 0) {
            if (pad.freeBytes >= need) {
                r = Pak_CountEmptyNotes_2(pad.channel, &pad.freeNotes);
                if (r == 0 && pad.freeNotes != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter_2(pad.channel, ext);
                    if (r == 0) {
                        flag = 1;
                        nv = pad.channel;
                        r = Pak_SaveNoteVerified_2(nv, 0x18, &gPakProfile, gPakCharacterNoteName, ext, gPakGameCode);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgPakFullDesc_2, plr, plr->m_ContState.m_pContState, own->desc);
                return 0;
            }
        }
    }
    O01FreeAll();
    if (flag != 0) {
        if (r == 0) {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgSaveDoneDesc_2, plr, plr->m_ContState.m_pContState, O01Pick(&pad));
        } else {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgSaveFailedDesc_2, plr, plr->m_ContState.m_pContState, O01Pick(&pad));
        }
    }
    return 1;
}

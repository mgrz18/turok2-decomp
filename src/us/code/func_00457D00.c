/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407290.c, func_00407290.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/save.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

typedef struct O01Pad { s32 v; s32 freeb; s32 cnt; } O01Pad;
extern s32 gPakMenuAnyPak; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 gPakMenuPort; /* local: globals/pak.h is not included here (its gPakProfile and the save handles would clash with this file's local-type view) */
extern s32 gPakMenuIsSave; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern char gPakProfile[]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern char gPakCharacterNoteName[]; /* local: globals/pak.h is not included here (its gPakProfile and the save handles would clash with this file's local-type view) */
extern u8 gPakGameCode[]; /* local: globals/pak.h is not included here (its gPakProfile and the save handles would clash with this file's local-type view) */
extern void *gSaveBufferHandle; /* local: globals/pak.h is not included here (its gPakProfile and the save handles would clash with this file's local-type view) */
extern void *gLoadBufferHandle; /* local: globals/pak.h is not included here (its gPakProfile and the save handles would clash with this file's local-type view) */
s32 Pak_BytesToPages_2(s32 n);
s32 Pak_GetFreePages_2(s32 pad, s32 *out);
s32 Pak_CountEmptyNotes_2(s32 pad, s32 *out);
s32 Pak_FindFreeExtLetter_2(s32 pad, u8 *ext);
s32 Pak_SaveNoteVerified_2(s32 pad, s32 size, char *n1, char *n2, u8 *ext, u8 *buf);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, void *d, void *e);
void CCache__Flush(s32 a);
void CCache__Deallocate(s32 a, void *p);
/* Controller-pak name for the selected pad (inlined). */
static __inline__ char *O01Pick(O01Pad *sel) {
    switch (sel->v) {
    case 0:
    default:
        return (char *)&gStatsAfterPakMenuDesc1_2;
    case 1:
        return (char *)&gStatsAfterPakMenuDesc2_2;
    case 2:
        return (char *)&gStatsAfterPakMenuDesc3_2;
    case 3:
        return (char *)&gStatsAfterPakMenuDesc4_2;
    }
}
/* Release the menu buffers (inlined). */
static __inline__ void O01FreeAll(void) {
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
s32 PakMenu_WriteProfile_2(s32 unused, Menu *own, s32 unused3) {
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
        need = Pak_BytesToPages_2(0x18);
        r = Pak_GetFreePages_2(pad.v, &pad.freeb);
        if (r == 0) {
            if (pad.freeb >= need) {
                r = Pak_CountEmptyNotes_2(pad.v, &pad.cnt);
                if (r == 0 && pad.cnt != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter_2(pad.v, ext);
                    if (r == 0) {
                        flag = 1;
                        nv = pad.v;
                        r = Pak_SaveNoteVerified_2(nv, 0x18, gPakProfile, gPakCharacterNoteName, ext, gPakGameCode);
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

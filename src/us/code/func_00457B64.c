/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004070F4.c, func_004070F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"

extern u8 gSingleMenuAfterPakDesc_2[];
extern u8 gPakMsgInsertToLoadDesc_2[];
extern u8 gDamagedPakAtBootMenuDesc_2[];
extern u8 gPakLoadGameMenuDesc_2[];
extern u8 gPakMsgPakNotFoundDesc_2[];
void Pak_ProbeSlot_2(s32 ch);
s32 Pak_GetStatus_2(s32 ch);
s32 Pak_IsRumblePak_2(s32 ch);
s32 Pak_NeedsRepair_2(s32 ch);
void PakMenu_UpdateNoteRows_2(Menu *o);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, void *d, void *e);
void PakMenu_InitLoadGame_2(Menu *own) {
    s32 pad;
    register s32 o1 asm("$8");
    register s32 o2 asm("$18");
    gPakMenuMode = 2;
    gPakMenuIsSave = 0;
    gPakMenuWrites = 0;
    o1 = 1;
    gPakMenuIsLoad = o1;
    gPakMenuIsProfile = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    pad = own->controller->port;
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = o1;
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gSingleMenuAfterPakDesc_2, own->player, own->controller, 0);
    } else {
        own->items[20].flags &= 0xFEFFFFFF;
        Pak_ProbeSlot_2(own->controller->port);
        if (Pak_GetStatus_2(pad) == 0) {
            PakMenu_UpdateNoteRows_2(own);
        } else {
            o2 = 1;
            gPakMenuLeaving = o2;
            if (Pak_IsRumblePak_2(pad) != 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgInsertToLoadDesc_2, own->player, own->controller, (void *)o2);
            } else if (Pak_NeedsRepair_2(pad) != 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc_2, own->player, own->controller, gPakLoadGameMenuDesc_2);
            } else {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgPakNotFoundDesc_2, own->player, own->controller, gSingleMenuAfterPakDesc_2);
            }
        }
    }
}

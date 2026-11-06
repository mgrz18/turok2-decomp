/* FAKEMATCH: two register asm pins and a second player pointer stand for what natural C does not give: the ROM
 * loads the player into $v0 and copies it to $s0 (pp, then p as the copy the branches use), and the constant 1 of
 * the error branch gets its own register (natural C shares one $s register for every 1: cse2 follows the jumps
 * into that branch). Twin of PakMenu_InitSaveGame_2 (same C). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00406F7C.c, func_00406F7C, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/save.h"
#include "globals/menu.h"
#include "globals/pak.h"

/* Controller pak menu, "save" entry: when the pak is ready, builds the save note (with its checksum) and
 * opens the save menu; otherwise shows why the pak cannot be used. */

typedef struct {
    s32 v;
    s32 w;
} PadSel;

extern s32 gPakMenuMode;
extern s32 gDamagedPakContinued;
extern s32 gCameraPoolHideViews;
extern s32 D_800F70A8; /* save size */
extern u8 gSavePortalMenuDescB_2[];
extern u8 gPakMsgInsertToSaveDesc_2[];
extern u8 gDamagedPakAtBootMenuDesc_2[];
extern u8 gPakSaveGameMenuDesc_2[];
extern u8 gPakMsgPakNotFoundDesc_2[];
void Pak_ProbeSlot_2(s32 ch);
s32 Pak_GetStatus_2(s32 ch);
s32 Pak_IsRumblePak_2(s32 ch);
s32 Pak_NeedsRepair_2(s32 ch);
void SaveGame_Build_2(CPlayer *p);
void PakMenu_UpdateNoteRows_2(Menu *o);
u32 Crc32_Compute_2(u8 *data, s32 len, u32 crc);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, s32 d, void *e);

void PakMenu_InitSaveGame_2(Menu *own) {
    CPlayer *plr;
    CPlayer *p;
    PadSel sel;

    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 0;
    gPakMenuMode = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    sel.v = own->controller->port;
    {
        register CPlayer *pp asm("$2") = own->player;

        plr = pp;
    }
    if (gDamagedPakContinued != 0) {
        p = plr;
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu_2(((u8 *)p->m_pCamera) + 0x53C, gSavePortalMenuDescB_2, p, p->m_ContState.m_pContState, NULL);
        gCameraPoolHideViews = 0;
    } else {
        Pak_ProbeSlot_2(own->controller->port);
        p = plr;
        if (Pak_GetStatus_2(sel.v) == 0) {
            SaveGame_Build_2(plr);
            {
                u32 crc = Crc32_Compute_2(gSaveBuffer, D_800F70A8 + 0x684, 0xB);
                s32 buf = (s32)gSaveBuffer;
                s32 size = D_800F70A8;

                *(u32 *)(size + buf + 0x684) = crc;
            }
            PakMenu_UpdateNoteRows_2(own);
        } else {
            register s32 one asm("$17") = 1;

            gPakMenuLeaving = one;
            if (Pak_IsRumblePak_2(sel.v) != 0) {
                MenuPool_OpenMenu_2(((u8 *)p->m_pCamera) + 0x53C, gPakMsgInsertToSaveDesc_2, p, p->m_ContState.m_pContState, (void *)one);
            } else if (Pak_NeedsRepair_2(sel.v) != 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc_2, p, p->m_ContState.m_pContState, gPakSaveGameMenuDesc_2);
            } else {
                MenuPool_OpenMenu_2(((u8 *)p->m_pCamera) + 0x53C, gPakMsgPakNotFoundDesc_2, p, p->m_ContState.m_pContState, gSavePortalMenuDescB_2);
            }
        }
    }
}

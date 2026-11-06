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

/* Controller pak menu, "save" entry: when the pak is ready, builds the save note (with its checksum) and
 * opens the save menu; otherwise shows why the pak cannot be used. */

typedef struct {
    s32 v;
    s32 w;
} PadSel;

extern s32 gPakMenuIsSave;
extern s32 gPakMenuWrites;
extern s32 gPakMenuIsLoad;
extern s32 gPakMenuIsProfile;
extern s32 gPakMenuMode;
extern s32 gPakMenuLeaving;
extern s32 gPakMenuAnyPak;
extern s32 gDamagedPakContinued;
extern s32 gCameraPoolHideViews;
extern s32 D_800F70A8; /* save size */
extern u8 gSavePortalMenuDescB[];
extern u8 gPakMsgInsertToSaveDesc[];
extern u8 gDamagedPakAtBootMenuDesc[];
extern u8 gPakSaveGameMenuDesc[];
extern u8 gPakMsgPakNotFoundDesc[];
void Pak_ProbeSlot(s32 ch);
s32 Pak_GetStatus(s32 ch);
s32 Pak_IsRumblePak(s32 ch);
s32 Pak_NeedsRepair(s32 ch);
void SaveGame_Build(CPlayer *p);
void PakMenu_UpdateNoteRows(Menu *o);
u32 Crc32_Compute(u8 *data, s32 len, u32 crc);
void MenuPool_OpenMenu(u8 *a, void *b, void *c, s32 d, void *e);

void PakMenu_InitSaveGame(Menu *own) {
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
        MenuPool_OpenMenu(((u8 *)p->m_pCamera) + 0x53C, gSavePortalMenuDescB, p, p->m_ContState.m_pContState, NULL);
        gCameraPoolHideViews = 0;
    } else {
        Pak_ProbeSlot(own->controller->port);
        p = plr;
        if (Pak_GetStatus(sel.v) == 0) {
            SaveGame_Build(plr);
            {
                u32 crc = Crc32_Compute(gSaveBuffer, D_800F70A8 + 0x684, 0xB);
                s32 buf = (s32)gSaveBuffer;
                s32 size = D_800F70A8;

                *(u32 *)(size + buf + 0x684) = crc;
            }
            PakMenu_UpdateNoteRows(own);
        } else {
            register s32 one asm("$17") = 1;

            gPakMenuLeaving = one;
            if (Pak_IsRumblePak(sel.v) != 0) {
                MenuPool_OpenMenu(((u8 *)p->m_pCamera) + 0x53C, gPakMsgInsertToSaveDesc, p, p->m_ContState.m_pContState, (void *)one);
            } else if (Pak_NeedsRepair(sel.v) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc, p, p->m_ContState.m_pContState, gPakSaveGameMenuDesc);
            } else {
                MenuPool_OpenMenu(((u8 *)p->m_pCamera) + 0x53C, gPakMsgPakNotFoundDesc, p, p->m_ContState.m_pContState, gSavePortalMenuDescB);
            }
        }
    }
}

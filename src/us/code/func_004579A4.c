/* FAKEMATCH: two register asm pins and a second player pointer stand for what natural C does not give: the ROM
 * loads the player into $v0 and copies it to $s0 (pp, then p as the copy the branches use), and the constant 1 of
 * the error branch gets its own register (natural C shares one $s register for every 1: cse2 follows the jumps
 * into that branch). Twin of PakMenu_InitSaveGame_2 (same C). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00406F7C.c, func_00406F7C, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/save.h"

/* Controller pak menu, "save" entry: when the pak is ready, builds the save note (with its checksum) and
 * opens the save menu; otherwise shows why the pak cannot be used. */

typedef struct {
    s32 v;
    s32 w;
} PadSel;

extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2020;
extern s32 D_80130940;
extern s32 D_800F70A8; /* save size */
extern u8 D_80119E2C[];
extern u8 D_0043BE14[];
extern u8 D_0043BB68[];
extern u8 D_0043BF7C[];
extern u8 D_0043B634[];
extern u8 D_0043BB20[];
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

    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 0;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    sel.v = own->controller->port;
    {
        register CPlayer *pp asm("$2") = own->player;

        plr = pp;
    }
    if (D_800C2020 != 0) {
        p = plr;
        D_801309BC = 1;
        MenuPool_OpenMenu_2(((u8 *)p->m_pCamera) + 0x53C, D_0043BE14, p, p->m_ContState.m_pContState, NULL);
        D_80130940 = 0;
    } else {
        Pak_ProbeSlot_2(own->controller->port);
        p = plr;
        if (Pak_GetStatus_2(sel.v) == 0) {
            SaveGame_Build_2(plr);
            {
                u32 crc = Crc32_Compute_2(D_800C2018, D_800F70A8 + 0x684, 0xB);
                s32 buf = (s32)D_800C2018;
                s32 size = D_800F70A8;

                *(u32 *)(size + buf + 0x684) = crc;
            }
            PakMenu_UpdateNoteRows_2(own);
        } else {
            register s32 one asm("$17") = 1;

            D_801309BC = one;
            if (Pak_IsRumblePak_2(sel.v) != 0) {
                MenuPool_OpenMenu_2(((u8 *)p->m_pCamera) + 0x53C, D_0043BB68, p, p->m_ContState.m_pContState, (void *)one);
            } else if (Pak_NeedsRepair_2(sel.v) != 0) {
                MenuPool_OpenMenu_2(D_80119E2C, D_0043BF7C, p, p->m_ContState.m_pContState, D_0043B634);
            } else {
                MenuPool_OpenMenu_2(((u8 *)p->m_pCamera) + 0x53C, D_0043BB20, p, p->m_ContState.m_pContState, D_0043BE14);
            }
        }
    }
}

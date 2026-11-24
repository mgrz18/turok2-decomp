#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/save.h"
#include "globals/game.h"
#include "globals/pak.h"
#include "globals/scene.h"
#include "structs/menu.h"
#include "protos/pak.h"
/* WRONG CUT: real function 0x0041DC5C, size 0x134 (through 0x0041DD90): splat cut it in two
 * (PakMenu_GetReturnMenu, func_0041DCB0) at a load in a delay slot. MATCHES over the real extent
 * (checked with the symbol size overridden to 0x134). */
#include "common.h"
#include "globals/menu_desc.h"
#include "globals/menu.h"
#include "protos/menu.h"
#include "protos/pakmenu.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/camera.h"
#include "structs/pak.h"
/* WRONG CUT: real function 0x0041E1D8, size 0x19C (through 0x0041E374): splat cut it in two
 * (PakMenu_InitLoadProfile, func_0041E294) at a jal's delay slot. */
#include "common.h"
/* REAL PakMenu_InitSaveGameAnyPak size 0x584 (0x0041E500-0x0041EA84); splat cut it into PakMenu_InitSaveGameAnyPak + func_0041E7D0 + func_0041E824 + func_0041E97C. Matches whole (checked with the size overridden). Twin of src/us/code/PakMenu_InitSaveGameAnyPak_2.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004069F4.c, func_004069F4, with Turok 2's layouts. */
#include "common.h"
/* FAKEMATCH: two register asm pins and a second player pointer stand for what natural C does not give: the ROM
 * loads the player into $v0 and copies it to $s0 (pp, then p as the copy the branches use), and the constant 1 of
 * the error branch gets its own register (natural C shares one $s register for every 1: cse2 follows the jumps
 * into that branch). Already cse1 replaces the error branch's 1 with the first one's register: the
 * Pak_GetStatus jump is followed, its label having one use and a barrier before it. Tried in the L pass,
 * none splitting the 1 or loading the player through $v0: one `p` read once, `plr` copied into `p` in
 * each branch, a chained `gPakMenuIsSave = gPakMenuWrites = 1`, a `one` local for every 1, inline reset
 * and error helpers, early returns instead of the elses, both if/else orders, and an empty do-while
 * (its loop-end note stops cse1, but cse2 runs after loop and ignores it). Twin of PakMenu_InitSaveGame_2 (same C). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00406F7C.c, func_00406F7C, with Turok 2's layouts. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_7.c, func_804070F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407290.c, func_00407290.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00407578.c, func_00407578, with Turok 2's layouts. */
/* The result is tested through a copy (err = r): it stops jump threading before cse1, so the second r == 0 test survives as in the ROM. */
#include "common.h"
/* FAKEMATCH (round 21 audit): 2 empty `__asm__` barriers; without them: PakMenu_SelectNote 60 of 521 words.
 * */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407748.c, func_00407748.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "structs/cache.h"
/* First-module twin of src/us/code/PakMenu_FreePagesText_2.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408118.c, func_00408118, with Turok 2's layouts. */
#include "common.h"
#include "structs/twin_pakmenu.h"
/* First-module twin of src/us/code/PakMenu_SelectCancel_2.c. WRONG CUT: REAL PakMenu_SelectCancel size 0x2B8 (0x0042019C-0x00420454); splat cut its tail into func_004203B0. Matches whole (checked with the size overridden). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004085E0.c, func_004085E0, with Turok 2's layouts. */
#include "common.h"
/* WRONG CUT: real function 0x00420454, size 0x264 (through 0x004206B8): splat cut it into
 * GameNoteMenu_SelectYes, func_004205B8 and func_004205C4 inside its straight-line code. */
#include "common.h"
#include "structs/world.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00408C78.c, func_00408C78.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408E1C.c, func_00408E1C, with Turok 2's layouts. */
/* WRONG CUT: the real function is 0x0042085C-0x00420C30 (size 0x3D4, 245 words): splat split it into PakMessage_SelectOk (0x1C),
 * func_00420878 (0x68) and func_004208E0 (0x350). All 245 words match (checked over the real extent).
 * Twin of src/us/code/PakMessage_SelectOk_2.c (first-module callees and data names, +0xE0). */
#include "common.h"
#include "protos/scene.h"
#include "protos/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_13.c, func_80409144_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_14.c, func_80409744_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "m2c_macros.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409824.c, func_00409824.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "globals/pak.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_16.c, func_804098B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409A18.c, func_00409A18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* gScene_m_PreviewOptions is declared as an array so its store keeps its place after the save buffer reads (a scalar is assumed not to alias them and is hoisted). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_2.CPP, func_8040B700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409E10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409F2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409FA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_8040A1B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: one function 0x00421D20-0x00421DFC (size 0xDC); splat cut its second half into
 * func_00421D90 (it starts in the middle of an if chain, reading $v0 set here). */
#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040A500.c, func_0040A500.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A6DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A77C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_11.c, func_8040A9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: one function 0x00422484-0x00422608 (size 0x184); splat cut its tail (the last range
 * tests and the copy loop) into func_00422574. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_12.c, func_8040AA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_22.c, func_8040ABA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_30.c, func_8040AC54_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_31.c, func_8040ACA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040ACD4.c, func_0040ACD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
/* WRONG CUT: the function really starts at 0x00422ED8 (size 0x3C). splat left its prologue
 * (addiu sp,-0x18; sw ra) in stray_00422ED8: cuts.us.txt marks those two words as never
 * executed, misled by a data word 0x00422EE0 in virtual_rodata_1 that belongs to another
 * module mapped at the same addresses. SavePortalMenu_SelectSave inlines this same function. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_6.c, func_8040A4A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: one function 0x00423768-0x00423880 (size 0x118); splat cut its second half into
 * func_004237FC (which starts in the delay slot of a bnel). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A748_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_4.c, func_8040B868_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: one function 0x00423CBC-0x00423D38 (size 0x7C): a switch whose cases splat cut
 * into func_00423CF4, func_00423D04, func_00423D14, func_00423D24 (jump table jtbl_800C16D0),
 * and whose shared `return 0` tail is the already-accepted func_00423D30 (8 bytes). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_10.c, func_8040BBC0_us.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */
extern void CCache__Deallocate(sptr, void *);
extern void **CCache__Allocate(s32, s32, s32, char *);
extern void *memset(void *dst, s32 value, uptr size);
extern void func_0029E460(char *dst, char *src, s32 n);

static __inline__ s32 SaveFileSize(void) {
    return gScene.unk_30 + 0x688;
}

/* Snapshots the player and the multiplayer setup into a freshly allocated buffer. */
void SaveGame_Build(CPlayer *p) {
    s32 size = SaveFileSize();
    SaveGame *save;
    s32 allocSize;
    s32 *handle;
    SaveGame *buf;

    if (gSaveBufferHandle != NULL) {
        CCache__Flush_Arg(0);
        CCache__Deallocate(0, gSaveBufferHandle);
    }
    allocSize = SaveFileSize();
    handle = CCache__Allocate(0, allocSize, 0x33, "Single Save Block");
    buf = *(SaveGame **) handle; /* the block's data pointer (CMemEntry.data), pointer-wide on the host */
    gSaveBufferHandle = handle;
    gSaveBuffer = buf;
    memset(gSaveBuffer, 0, allocSize);
    gSaveBuffer->player = p->m_Data;
    gSaveBuffer->settings = *(GameSettingsBlock *)&gEngineApp_m_Settings;
    save = gSaveBuffer;
    save->frames = gEngineApp_m_Settings.match.frames;
    save->seconds = gEngineApp_m_Settings.match.seconds;
    save->minutes = gEngineApp_m_Settings.match.minutes;
    save->hours = gEngineApp_m_Settings.match.hours;
    /* gScene.unk_30 is a struct member, so it is reloaded after the store into the header. */
    save->size = size;
    save->levelFlagsSize = gScene.unk_30;
    func_0029E460((char *)save->levelFlags, gLoadSavePersistentData, gScene.unk_30);
}

/* Greys out the controller pak notes that cannot be picked: a used note is
 * selectable unless it is this game's save (gPakCharacterNoteName when loading, mode 1,
 * gPakGameNoteName in mode 2) with Turok 2's game and company codes. */


/* Game code / company code check on the controller-pak note (as in PakMenu_SelectNote). */
static __inline__ s32 k7_chk(char *gc, char *cc) {
    if (Str_Length(gc) == 4 && Str_Length(cc) == 2 &&
        Str_Compare(gc, gPakGameCode) == 0 && Str_Compare(cc, gPakCompanyCode) == 0) {
        return 1;
    }
    return 0;
}

static __inline__ void rowEnable(MenuItem *row) {
    row->flags &= ~0x01000000;
}

static __inline__ void rowDisable(MenuItem *row) {
    row->flags |= 0x01000000;
}

void PakMenu_UpdateNoteRows(Menu *menu) {
    char ext[8];
    char company[8];
    char game[8];
    char name[16];
    s32 used;
    s32 size;
    s32 slot;
    s32 i;
    s32 ok;
    s32 ours;
    s32 err;

    if (gPakMenuAnyPak) {
        slot = gPakMenuPort;
    } else {
        slot = menu->controller->port;
    }
    for (i = 0; i < 16; i++) {
        err = Pak_FindNote(slot, i, &used, name, ext, &size, company, game);
        ok = 1;
        if (err == 0 && used != 0) {
            switch (gPakMenuMode) {
            case 1:
                ours = k7_chk(game, company);
                if (Str_Compare(name, gPakCharacterNoteName) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 2:
                ours = k7_chk(game, company);
                if (Str_Compare(name, gPakGameNoteName) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 0:
            default:
                ok = 0;
                break;
            }
        }
        if (ok) {
            rowEnable(&menu->items[i + 3]);
        } else {
            rowDisable(&menu->items[i + 3]);
        }
    }
}

static __inline__ void *TmplForKind(s32 *kindp) {
    /* by address: kind stays in its stack slot, as in the ROM */
    void *t;

    switch (*kindp) {
    case 0:
    default:
        t = &gStatsAfterPakMenuDesc1;
        break;
    case 1:
        t = &gStatsAfterPakMenuDesc2;
        break;
    case 2:
        t = &gStatsAfterPakMenuDesc3;
        break;
    case 3:
        t = &gStatsAfterPakMenuDesc4;
        break;
    }
    return t;
}

/* Picks the menu template to open for the current game mode. */
void *PakMenu_GetReturnMenu(Menu *c) {
    void *tmpl = NULL;
    s32 kind;

    if (gPakMenuIsSave) {
        if (gPakMenuIsProfile) {
            if (gPakMenuAnyPak) {
                kind = gPakMenuPort;
            } else {
                kind = c->controller->port;
            }
            tmpl = TmplForKind(&kind);
        } else {
            tmpl = &gSavePortalMenuDescB;
        }
    } else if (gPakMenuIsLoad) {
        if (gPakMenuIsProfile) {
            tmpl = &gCharacterMenuAfterPakDesc;
        } else if (gSavePortalActive) {
            tmpl = &gSavePortalMenuDescB;
        } else {
            tmpl = &gSingleMenuAfterPakDesc;
        }
    } else if (gPakMenuAnyPak) {
        tmpl = &gPakSaveGameAnyPakMenuDesc;
    }
    return tmpl;
}

/* Checks the controller pak in the given slot before a save/load menu:
 * returns the pak status, or -1 after opening the "pak full" dialog; opens the
 * matching warning dialog (templates in module 1's data, 0x0043xxxx) when the pak is missing,
 * damaged or unformatted. */


s32 PakMenu_CheckPak(Menu *c, s32 slot, s32 quiet) {
    s32 status;
    void *parent;
    s32 full;
    void *tmpl;

    Pak_ProbeSlot(slot);
    status = Pak_GetStatus(slot);
    parent = PakMenu_GetReturnMenu(c);
    if (parent != NULL) {
        full = Controller_TakePakChanged(slot);
        if ((status == 0 || full != 0) && status != -2) {
            if (full != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgPakChangedDesc, c->player, c->controller, parent);
                return -1;
            }
        } else if (Pak_IsRumblePak(slot) != 0) {
            gPakMenuLeaving = 1;
            if (gPakMenuAnyPak != 0) {
                tmpl = &gPakMsgInsertToManageDesc;
            } else if (gPakMenuIsSave != 0) {
                tmpl = &gPakMsgInsertToSaveDesc;
            } else {
                tmpl = &gPakMsgInsertToLoadDesc;
            }
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, tmpl, c->player, c->controller, parent);
        } else if (quiet == 0) {
            gPakMenuLeaving = 1;
            if (Pak_NeedsRepair(slot) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gDamagedPakAtBootMenuDesc, c->player, c->controller, parent);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgPakNotFoundDesc, c->player, c->controller, parent);
            }
        } else if (status != -2) {
            status = 0;
        }
        return status;
    }
    return 0;
}

/* O01Pick and O01Capture are separate functions in Rage Wars; inlined here in Turok 2. */
static inline u8 *O01Pick(PakChannel *sel) {
    u8 *msg;
    switch (sel->channel) {
    case 0:
    default:
        msg = (u8 *)&gStatsAfterPakMenuDesc1;
        break;
    case 1:
        msg = (u8 *)&gStatsAfterPakMenuDesc2;
        break;
    case 2:
        msg = (u8 *)&gStatsAfterPakMenuDesc3;
        break;
    case 3:
        msg = (u8 *)&gStatsAfterPakMenuDesc4;
        break;
    }
    return msg;
}

static inline void O01Capture(CPlayer *a) {
    CPlayerOptions *o = a->m_pOptions;
    s32 i;
    gPakProfile.character = o->m_nCharacter;
    gPakProfile.suicides = o->m_Suicides;
    gPakProfile.deaths = o->m_Deaths;
    gPakProfile.kills = o->m_Kills;
    gPakProfile.tags = o->m_Tags;
    gPakProfile.reserved[0] = 0;
    gPakProfile.reserved[1] = 0;
    gPakProfile.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        gPakProfile.name[i] = o->m_Name[i];
    }
}

void PakMenu_InitSaveProfile(Menu *own) {
    CPlayer *plr;
    PakChannel sel;
    u8 *msg;
    s32 pad;
    CPlayer *pp;
    s32 pv;
    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 1;
    gPakMenuMode = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    pv = own->controller->port;
    pp = own->player;
    sel.channel = pv;
    plr = pp;
    msg = O01Pick(&sel);
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)own->player->m_pCamera->m_MenuPool, msg, plr, plr->m_ContState.m_pContState, 0);
    } else {
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        Pak_ProbeSlot(pad);
        if (Pak_GetStatus(sel.channel) == 0) {
            O01Capture(plr);
            gPakProfile.checksum = Crc32_Compute((u8 *)&gPakProfile, 0x14, 4);
            PakMenu_UpdateNoteRows(own);
        } else {
            gPakMenuLeaving = 1;
            if (Pak_IsRumblePak(sel.channel) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgInsertToSaveDesc, plr, plr->m_ContState.m_pContState, msg);
            } else if (Pak_NeedsRepair(sel.channel) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gDamagedPakAtBootMenuDesc, plr, plr->m_ContState.m_pContState, (u8 *)&gPakSaveProfileMenuDesc);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgPakNotFoundDesc, plr, plr->m_ContState.m_pContState, msg);
            }
        }
    }
}

void PakMenu_InitLoadProfile(Menu *m) {
    s32 kind;

    gPakMenuIsSave = 0;
    gPakMenuWrites = 0;
    gPakMenuIsLoad = 1;
    gPakMenuIsProfile = 1;
    gPakMenuMode = 1;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    m->items[20].flags &= ~0x1000000;
    kind = m->controller->port;
    if (gDamagedPakContinued) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)m->player->m_pCamera->m_MenuPool, &gCharacterMenuAfterPakDesc, m->player, m->controller, NULL);
        return;
    }
    Pak_ProbeSlot(kind);
    if (!Pak_GetStatus(kind)) {
        PakMenu_UpdateNoteRows(m);
        return;
    }
    gPakMenuLeaving = 1;
    if (Pak_IsRumblePak(kind)) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgInsertToLoadDesc, m->player, m->controller, &gCharacterMenuAfterPakDesc);
    } else if (Pak_NeedsRepair(kind)) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gDamagedPakAtBootMenuDesc, m->player, m->controller, &gPakLoadProfileMenuDesc);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgPakNotFoundDesc, m->player, m->controller, &gCharacterMenuAfterPakDesc);
    }
}

/* Pak slot scan step: a pak that already holds one of the game's two files
 * (names gPakCharacterNoteName, gPakGameNoteName) wins at once (*best = 123); otherwise the
 * slot is a candidate when the pak is usable and has room for a save, and
 * the slot with the most free pages is kept. Returns 1 to keep scanning. */



/* PakMenu_FindNoteByName without the index output */
static __inline__ s32 hasFile(char *file, s32 slot) {
    char name[16];
    char *entry;
    s32 i;

    for (i = 0; i < 16; i++) {
        Pak_GetNoteGameName(slot, i, &entry);
        Pak_DecodeText(entry, name, 16);
        if (Str_Compare(name, file) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 PakMenu_CheckSlotForSave(void *unused, s32 slot, s32 *bestSlot, s32 *best) {
    s32 free;
    s32 files;
    s32 found;
    s32 found2;
    s32 err;
    s32 need;

    found = hasFile(gPakCharacterNoteName, slot);
    found2 = hasFile(gPakGameNoteName, slot);
    if (found || found2) {
        *bestSlot = slot;
        *best = 123;
        return 1;
    }
    err = Pak_GetFreePages(gPakMenuPort, &free);
    if (err == 0) {
        err = Pak_CountEmptyNotes(gPakMenuPort, &files);
        if (err == 0) {
            need = Pak_BytesToPages((*(u8 * *) &gScene.unk_30) + 0x688);
            if (files == 0 || free < need) {
                if (*best < need) {
                    *best = need;
                    *bestSlot = slot;
                }
                return 0;
            }
        }
    }
    return err == 0;
}

/* Controller pak menu, load: finds a pad with a controller pak (trying each pad in turn), checks it for the
 * game's notes and free space and opens the load menu, or shows why the pak cannot be used. */

s32 Controller_AnyConnected(void);
void CEngineApp__UpdateControllers(void); /* the definition takes none; the calls pass the app */
#define PAK_ERROR()                                                        \
    gPakMenuLeaving = 1;                                                        \
    gPakBootPending = 0;                                                        \
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgCannotSaveDesc, arg0->player, arg0->controller, NULL); \
    gPakCheckHasPak = 0;                                                        \
    sPakMenuStartLatched = 1;

void PakMenu_InitSaveGameAnyPak(Menu *arg0) {
    u8 buf[16];
    s32 sp28, sp2C;
    sptr sp30, sp34;
    s32 sp38, sp3C;
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
        gPakBootPending = 0;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgNoControllerDesc, arg0->player, arg0->controller, NULL);
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
    ((void (*)(void *)) CEngineApp__UpdateControllers)(&gEngineApp);
    if ((arg0->controller->buttons & 0x1000) && sPakMenuStartLatched == 0) {
        gPakMenuStartPressed = 1;
        sPakMenuStartLatched = 1;
    }
    sPakMenuStartLatched = 1;
    if (gPakMenuConfirmed != 0 || gPakMenuStartPressed != 0) {
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
    m1 = Pak_BytesToPages(gScene.unk_30 + 0x688);
    r2 = Pak_BytesToPages(0x18);
    if (sp3C == 0 || sp38 < r2) {
        goto L56;
    }
    if (sp38 < m1) {
        gPakMenuLeaving = 1;
        gPakMenuStartPressed = 1;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakConfirmNoSpaceAllDesc, arg0->player, arg0->controller, (u8 *)&gPakSaveGameAnyPakMenuDesc);
        goto after;
    }
    if (sp38 < r2) {
        goto L56;
    }
    goto after;
L56:
    gPakMenuLeaving = 1;
    gPakMenuStartPressed = 1;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakConfirmNoSpaceDesc, arg0->player, arg0->controller, (u8 *)&gPakSaveGameAnyPakMenuDesc);
after:
    if (s0 == 0) {
        goto fin;
    }
blk59:
    gPakMenuLeaving = 1;
    gPakMenuStartPressed = 1;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gDamagedPakAtBootMenuDesc, arg0->player, arg0->controller, (u8 *)&gPakSaveGameAnyPakMenuDesc);
fin:
    if (gPakMenuLeaving == 0) {
        PakMenu_UpdateNoteRows(arg0);
    }
    gPakCheckHasPak = 0;
    sPakMenuStartLatched = 1;
}

/* Controller pak menu, "save" entry: when the pak is ready, builds the save note (with its checksum) and
 * opens the save menu; otherwise shows why the pak cannot be used. */


extern s32 gScene_unk_30; /* local: globals/scene.h; gScene.unk_30 (the member) moves the ROM's registers here. save size */

void PakMenu_InitSaveGame(Menu *own) {
    CPlayer *plr;
    CPlayer *p;
    PakChannel sel;

    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 0;
    gPakMenuMode = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    sel.channel = own->controller->port;
    {
        register CPlayer *pp asm("$2") = own->player;

        plr = pp;
    }
    if (gDamagedPakContinued != 0) {
        p = plr;
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)p->m_pCamera->m_MenuPool, &gSavePortalMenuDescB, p, p->m_ContState.m_pContState, NULL);
        gCameraPoolHideViews = 0;
    } else {
        Pak_ProbeSlot(own->controller->port);
        p = plr;
        if (Pak_GetStatus(sel.channel) == 0) {
            SaveGame_Build(plr);
            {
                u32 crc = Crc32_Compute(gSaveBuffer, gScene_unk_30 + 0x684, 0xB);
                sptr buf = (sptr)gSaveBuffer;
                sptr size = gScene_unk_30;

                *(u32 *)(size + buf + 0x684) = crc;
            }
            PakMenu_UpdateNoteRows(own);
        } else {
            register s32 one asm("$17") = 1;

            gPakMenuLeaving = one;
            if (Pak_IsRumblePak(sel.channel) != 0) {
                MenuPool_OpenMenu((u8 *)p->m_pCamera->m_MenuPool, &gPakMsgInsertToSaveDesc, p, p->m_ContState.m_pContState, (void *)one);
            } else if (Pak_NeedsRepair(sel.channel) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gDamagedPakAtBootMenuDesc, p, p->m_ContState.m_pContState, &gPakSaveGameMenuDesc);
            } else {
                MenuPool_OpenMenu((u8 *)p->m_pCamera->m_MenuPool, &gPakMsgPakNotFoundDesc, p, p->m_ContState.m_pContState, &gSavePortalMenuDescB);
            }
        }
    }
}

#ifdef TARGET_PORT
/* The port: the Rage Wars view (Shared_PakMenu, PakNotesMenuItem) puts the items, the player and the
 * controller at N64 offsets (0xC, 0x1C, 0x20; items 0x28 apart): on the host it read the channel from
 * the menu's param and cleared the disabled bit of the wrong item (the load button stayed disabled,
 * the cursor bounced between it and CANCEL). The same code on the typed Menu (structs/menu.h). */
void PakMenu_InitLoadGame(Menu *menu) {
    s32 ch;

    gPakMenuMode = 2;
    gPakMenuIsSave = 0;
    gPakMenuWrites = 0;
    gPakMenuIsLoad = 1;
    gPakMenuIsProfile = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    ch = menu->controller->port;
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *) &gSingleMenuAfterPakDesc, (sptr) menu->player,
                          (sptr) menu->controller, 0);
        return;
    }
    menu->items[20].flags &= ~0x01000000;
    Pak_ProbeSlot(menu->controller->port);
    if (Pak_GetStatus(ch) == 0) {
        PakMenu_UpdateNoteRows((struct Menu *) menu);
        return;
    }
    gPakMenuLeaving = 1;
    if (Pak_IsRumblePak(ch) != 0) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *) &gPakMsgInsertToLoadDesc, (sptr) menu->player,
                          (sptr) menu->controller, 1);
    } else if (Pak_NeedsRepair(ch) != 0) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *) &gDamagedPakAtBootMenuDesc, (sptr) menu->player,
                          (sptr) menu->controller, (sptr) &gPakLoadGameMenuDesc);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *) &gPakMsgPakNotFoundDesc, (sptr) menu->player,
                          (sptr) menu->controller, (sptr) &gSingleMenuAfterPakDesc);
    }
}
#else
/* The port body's signature; the N64 body reads the menu through the Rage Wars view (a macro: a local copy
 * would move a register). */
#define menu ((Shared_PakMenu *) menuArg)
void PakMenu_InitLoadGame(Menu *menuArg)
{
  Shared_PakMenu *new_var3;
  int new_var;
  char *new_var2;
  s32 ch;
  new_var = 1;
  gPakMenuMode = 2;
  gPakMenuIsSave = 0;
  gPakMenuWrites = 0;
  gPakMenuIsLoad = new_var;
  gPakMenuIsProfile = 0;
  gPakMenuLeaving = 0;
  gPakMenuAnyPak = 0;
  ch = menu->slot->unk4;
  if (gDamagedPakContinued != 0)
  {
    gPakMenuLeaving = 1;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gSingleMenuAfterPakDesc, menu->player, menu->slot, 0);
    return;
  }
  menu->items[20].flags &= ~0x01000000;
  Pak_ProbeSlot(menu->slot->unk4);
  new_var = 0;
  if (Pak_GetStatus(ch) == new_var)
  {
    PakMenu_UpdateNoteRows((struct Menu *) menu);
    return;
  }
  gPakMenuLeaving = 1;
  if (Pak_IsRumblePak(ch) != new_var)
  {
    new_var2 = (char *)&gPakMsgInsertToLoadDesc;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, new_var2, menu->player, menu->slot, (char *) 1);
  }
  else
  {
    new_var3 = menu;
    if (Pak_NeedsRepair(ch) != 0)
    {
      MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakAtBootMenuDesc, new_var3->player, menu->slot, (unsigned char *)&gPakLoadGameMenuDesc);
    }
    else
    {
      MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgPakNotFoundDesc, new_var3->player, new_var3->slot, (char *)&gSingleMenuAfterPakDesc);
    }
  }
}
#undef menu
#endif

/* Static inlines in the Rage Wars source file (dropped by the importer). */
static inline u8 *O01Pick_41EDE0(PakQuery *sel) {
    u8 *p;
    switch (sel->channel) {
    default:
    case 0:
        p = (u8 *)&gStatsAfterPakMenuDesc1;
        break;
    case 1:
        p = (u8 *)&gStatsAfterPakMenuDesc2;
        break;
    case 2:
        p = (u8 *)&gStatsAfterPakMenuDesc3;
        break;
    case 3:
        p = (u8 *)&gStatsAfterPakMenuDesc4;
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

s32 PakMenu_WriteProfile(sptr unused, Menu *own, s32 unused3) {
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
        need = Pak_BytesToPages(0x18);
        r = Pak_GetFreePages(pad.channel, &pad.freeBytes);
        if (r == 0) {
            if (pad.freeBytes >= need) {
                r = Pak_CountEmptyNotes(pad.channel, &pad.freeNotes);
                if (r == 0 && pad.freeNotes != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter(pad.channel, ext);
                    if (r == 0) {
                        flag = 1;
                        nv = pad.channel;
                        r = Pak_SaveNoteVerified(nv, 0x18, &gPakProfile, gPakCharacterNoteName, ext, gPakGameCode);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgPakFullDesc, plr, plr->m_ContState.m_pContState, own->desc);
                return 0;
            }
        }
    }
    O01FreeAll();
    if (flag != 0) {
        if (r == 0) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgSaveDoneDesc, plr, plr->m_ContState.m_pContState, O01Pick_41EDE0(&pad));
        } else {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgSaveFailedDesc, plr, plr->m_ContState.m_pContState, O01Pick_41EDE0(&pad));
        }
    }
    return 1;
}

void Cinema_StopAtEnd(void);
void Cinema_RequestById(s32 a);

static __inline__ void FreeAll(void) {
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

s32 PakMenu_WriteSaveGame(s32 unused, Menu *own) {
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
    size = gScene.unk_30 + 0x688;
    if (gPakMenuIsSave != 0) {
        need = Pak_BytesToPages(size);
        r = Pak_GetFreePages(pad.channel, &pad.freeBytes);
        if (r == 0) {
            if (pad.freeBytes >= need) {
                r = Pak_CountEmptyNotes(pad.channel, &pad.freeNotes);
                if (r == 0 && pad.freeNotes != 0) {
                    ok = 0;
                    r = Pak_FindFreeExtLetter(pad.channel, ext);
                    if (r == 0) {
                        written = 1;
                        padv = pad.channel;
                        r = Pak_SaveNoteVerified(padv, size, gSaveBuffer, gPakGameNoteName, ext, gPakGameCode);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakConfirmNoSpaceDesc, plr, plr->m_ContState.m_pContState, own->desc);
                return 0;
            }
        }
    }
    FreeAll();
    if (!written) {
        return 1;
    }
    if (r == 0) {
        Cinema_StopAtEnd();
        Cinema_RequestById(0x191);
    }
    err = r;
    if (err == 0) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgSaveDoneDesc, plr, plr->m_ContState.m_pContState, (sptr)&gSavePortalMenuDescB);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgSaveFailedDesc, plr, plr->m_ContState.m_pContState, (sptr)&gSavePortalMenuDescB);
    }
    return 1;
}

/* Game code / company code check on the controller-pak note (inlined). */
static __inline__ s32 k7_chk_41F32C(u8 *gc, u8 *cc) {
    if (Str_Length(gc) == 4 && Str_Length(cc) == 2 &&
        Str_Compare(gc, gPakGameCode) == 0 && Str_Compare(cc, gPakCompanyCode) == 0) {
        return 1;
    }
    return 0;
}

s32 PakMenu_SelectNote(s32 *arg0, Menu *arg1) {
    u8 sp20[8];
    u8 sp28[8];
    u8 sp30[8];
    u8 sp38[16];
    s32 sp48;
    s32 sp4C;
    s32 s0, s1, s2, s3, s4, s5, hs, s7, v1, v0;
    CMemEntry *m;
    u8 *d;
    s32 i, ch;
    u8 *a1p;
    s2 = 0;
    s5 = *arg0 - 3;
    s7 = gScene.unk_30 + 0x688;
    if (gPakMenuAnyPak != 0) {
        s4 = gPakMenuPort;
    } else {
        s4 = arg1->controller->port;
    }
    if (PakMenu_CheckPak(arg1, s4, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (gPakMenuWrites == 0) goto L00407BBC;
    s1 = Pak_FindNote(s4, s5, &sp48, sp38, sp20, &sp4C, sp28, sp30);
    __asm__("" : : "r"(s1));
    if (s1 == 0 && sp48 == 0) {
        arg1->cursor = 0x14;
        PakMenu_UpdateNoteRows(arg1);
        return 0;
    }
    v1 = k7_chk_41F32C(sp30, sp28);
    if (v1 != 0) {
        s3 = Str_Compare(sp38, gPakCharacterNoteName) == 0;
        s0 = Str_Compare(sp38, gPakGameNoteName) == 0;
    } else {
        s3 = 0;
        s0 = 0;
    }
    if (s1 != 0) goto L00407EE8;
    if (sp48 == 0) goto L00407EE8;
    __asm__("" : : "r"(s3));
    if (s3 == 0) goto L004079A0;
    if (Pak_ReadNote(s4, s5, &gPakProfile, 0x18) != 0) goto L0040799B;
    if (Crc32_Compute(&gPakProfile, 0x14, 4) != gPakProfile.checksum) goto L0040799B;
    s3 = 0;
    gPakProfileRead = gPakProfile;
    gPakNoteIndex = s5;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gCharNoteDeleteMenuDesc, (sptr)arg1->player, (sptr)arg1->controller, (sptr)arg1->desc);
    return 1;
L0040799B:
    s3 = 0;
L0040799C:
    s2 = 1;
L004079A0:
    if (s0 == 0) goto L00407AD0;
    if (s2 != 0) goto L00407AD0;
    s0 = sp4C;
    if (gLoadBufferHandle != 0) {
        CCache__Flush_Arg(0);
        CCache__Deallocate(0, gLoadBufferHandle);
    }
    if (s0 == 0) {
        s0 = gScene.unk_30 + 0x688;
    } else {
        s0 = s0 << 8;
    }
    m = CCache__Allocate(0, s0, 0x33, sPakMenuText_SingleSaveBlock);
    gLoadBufferHandle = (sptr)m;
    gLoadBuffer = (SaveGame *)m->data;
    memset((void *)gLoadBuffer, 0, s0);
    if (Pak_ReadNote(s4, s5, gLoadBuffer, sp4C << 8) != 0) goto L00407ACC;
    hs = *(s32 *)gLoadBuffer;
    if (hs != s7) goto L00407ACC;
    if (Crc32_Compute(gLoadBuffer, hs - 4, 0xB) != *(s32 *)(*(s32 *)gLoadBuffer + (sptr)gLoadBuffer - 4)) goto L00407ACC;
    s0 = 0;
    if (gScene.unk_30 != *(s32 *)((sptr)gLoadBuffer + 4)) { s2 = 1; goto L00407AD0; }
    gPakNoteIndex = s5;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gGameNoteDeleteMenuDesc, (sptr)arg1->player, (sptr)arg1->controller, (sptr)arg1->desc);
    return 1;
L00407ACC:
    s0 = 0;
    s2 = 1;
L00407AD0:
    if (s3 != 0) goto L00407EE8;
    if (s0 != 0) goto L00407EE8;
    ch = 0;
    gPakNoteIndex = s5;
    d = sCharNoteMenuText_Empty;
    if (Pak_TextHasInvalidChar(sp38, 0x10) != 0) {
        Str_Copy(sp38, sPakMenuText_OtherGame);
    }
    i = 1;
    v1 = 0;
    do {
        if (i != 0) ch = sp38[v1];
        if (ch == 0) { i = 0; ch = 0x20; }
        *d = ch;
        v1 += 1;
        d += 1;
    } while (v1 < 0x10);
    i = 1;
    d += i;
    v1 = 0;
    do {
        if (i != 0) ch = sp20[v1];
        if (ch == 0) { i = 0; ch = 0x20; }
        *d = ch;
        v1 += 1;
        d += 1;
    } while (v1 < 4);
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gCharNoteMenuDesc3, (sptr)arg1->player, (sptr)arg1->controller, (sptr)arg1->desc);
    return 1;
L00407BBC:
    if (gPakMenuIsLoad == 0) goto L00407EE8;
    if (gPakMenuIsProfile == 0) goto L00407CDC;
    if (s2 != 0) goto L00407CDC;
    if (Pak_FindNote(s4, s5, &sp48, sp38, sp20, &sp4C, sp28, sp30) != 0) goto L00407EE8;
    if (sp48 == 0) goto L00407EE8;
    if (Str_Compare(sp38, gPakCharacterNoteName) != 0) goto L00407EE8;
    if (Pak_ReadNote(s4, s5, &gPakProfile, 0x18) != 0) goto L00407CD0;
    if (Crc32_Compute(&gPakProfile, 0x14, 4) != gPakProfile.checksum) goto L00407CD0;
    gPakProfileRead = gPakProfile;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gCharNoteLoadMenuDesc, (sptr)arg1->player, (sptr)arg1->controller, 0);
    return 1;
L00407CD0:
    s2 = 1;
    goto L00407EE8;
L00407CDC:
    if (gPakMenuIsLoad == 0) goto L00407EE8;
    v0 = gPakMenuIsProfile;
    if (v0 != 0) goto L00407EE8;
    if (s2 != 0) goto L00407EF8;
    if (Pak_FindNote(s4, s5, &sp48, sp38, sp20, &sp4C, sp28, sp30) != 0) goto L00407EE8;
    if (sp48 == 0) goto L00407EE8;
    if (Str_Compare(sp38, gPakGameNoteName) != 0) goto L00407EE8;
    s0 = sp4C;
    if (gLoadBufferHandle != 0) {
        CCache__Flush_Arg(0);
        CCache__Deallocate(0, gLoadBufferHandle);
    }
    if (s0 == 0) {
        s0 = gScene.unk_30 + 0x688;
    } else {
        s0 = s0 << 8;
    }
    m = CCache__Allocate(0, s0, 0x33, sPakMenuText_SingleSaveBlock);
    gLoadBufferHandle = (sptr)m;
    gLoadBuffer = (SaveGame *)m->data;
    memset((void *)gLoadBuffer, 0, s0);
    if (Pak_ReadNote(s4, s5, gLoadBuffer, sp4C << 8) != 0) goto L00407E70;
    hs = *(s32 *)gLoadBuffer;
    if (hs != s7) goto L00407E70;
    if (Crc32_Compute(gLoadBuffer, hs - 4, 0xB) != *(s32 *)(*(s32 *)gLoadBuffer + (sptr)gLoadBuffer - 4)) goto L00407E70;
    if (gScene.unk_30 != *(s32 *)((sptr)gLoadBuffer + 4)) goto L00407E70;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gGameNoteLoadMenuDesc, (sptr)arg1->player, (sptr)arg1->controller, 0);
    return 1;
L00407E70:
    s2 = 1;
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
L00407EE8:
    if (s2 == 0) return 0;
    v0 = gPakMenuIsProfile;
L00407EF8:
    a1p = (u8 *)&gPakLoadGameMenuDesc;
    if (v0 != 0) a1p = (u8 *)&gPakLoadProfileMenuDesc;
    gPakOverwriteNote = s5;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakConfirmCorruptNoteDesc, (sptr)arg1->player, (sptr)arg1->controller, a1p);
    return 1;
}

/* Save/load menu: fills a pak file line ("NAME.EXT" and its size) for the
 * note of the chosen pak; an empty or unreadable note shows blanks. */


/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
s32 sprintf(char *dst, const char *fmt, ...);

s32 PakMenu_NoteText(MenuItem *item, Menu *c) {
    char name[16];
    char ext[8];
    u8 unk38[8];
    u8 unk40[8];
    s32 used;
    s32 size;
    char ch = 0x20;
    s32 note = item->index - 3;
    s32 slot;
    s32 more;
    s32 r;
    s32 i;
    char *dst;

    if (gPakMenuLeaving != 0) {
        return 0;
    }
    if (gPakMenuAnyPak != 0) {
        slot = gPakMenuPort;
    } else {
        slot = c->controller->port;
    }
    r = Pak_FindNote(slot, note, &used, name, ext, &size, unk38, unk40);
    if (used != 0 && r == 0) {
        if (Pak_TextHasInvalidChar(name, 16) != 0) {
            Str_Copy(name, sPakMenuText_OtherGame);
        }
    } else {
        Str_Copy(name, sPakMenuText_Empty);
        Str_Copy(ext, sPakMenuText_Empty);
        size = 0;
    }
    /* copy the name and the extension, padding with spaces after the terminator */
    dst = item->text;
    dst += 3;
    more = 1;
    for (i = 0; i < 16; i++) {
        if (more) {
            ch = name[i];
        }
        if (ch == 0) {
            more = 0;
            ch = 0x20;
        }
        *dst++ = ch;
    }
    more = 1;
    dst += more;
    for (i = 0; i < 4; i++) {
        if (more) {
            ch = ext[i];
        }
        if (ch == 0) {
            more = 0;
            ch = 0x20;
        }
        *dst++ = ch;
    }
    *dst += 2;
    sprintf(dst, "%3d", size);
    return 0;
}

/* Controller pak menu item: picks the item's text for the pak state and prints the pak's free blocks and
 * the blocks a save needs into it ("%3d"-style, three characters each). */


#ifndef TARGET_PORT /* the port keeps the generated copies */
/* The object's .rodata holds every string literal before the first jump table (PakConfirmMenu_SelectYes's):
 * after "Single Save Block" and PakMenu_NoteText's "%3d" come these, the literals of functions further on
 * (an earlier definition emitted them in the original: inline helpers, or the deferred functions' strings).
 * Here they are data in that place, and the functions read them by name. */
const char sPakMenuText_Spaces3_def[] __asm__("sPakMenuText_Spaces3") = "   ";
const char sPakMenuText_Space_def[] __asm__("sPakMenuText_Space") = " ";
const char sPakMenuFmt_1d_def[] __asm__("sPakMenuFmt_1d") = "%1d";
const char sGameNoteMenuFmt_2d_def[] __asm__("sGameNoteMenuFmt_2d") = "%2d";
const char sGameNoteMenuFmt_Time_def[] __asm__("sGameNoteMenuFmt_Time") = "%3d.%02d.%02d";
const char sCharNoteMenuFmt_04d_def[] __asm__("sCharNoteMenuFmt_04d") = "%04d";
const char sPakMenuFmt_1dDot[] = "%1d."; /* DamagedPakMenu_UpdateLine3's */
#define PAKMENU_FMT_1D sPakMenuFmt_1d_def
#define PAKMENU_FMT_2D sGameNoteMenuFmt_2d_def
#define PAKMENU_FMT_TIME sGameNoteMenuFmt_Time_def
#define PAKMENU_FMT_04D sCharNoteMenuFmt_04d_def
#define PAKMENU_FMT_1D_DOT sPakMenuFmt_1dDot
#else
#define PAKMENU_FMT_1D "%1d"
#define PAKMENU_FMT_2D "%2d"
#define PAKMENU_FMT_TIME "%3d.%02d.%02d"
#define PAKMENU_FMT_04D "%04d"
#define PAKMENU_FMT_1D_DOT "%1d."
#endif
extern Chars4 sPakMenuText_Spaces3View __asm__("sPakMenuText_Spaces3"); /* copied as a Chars4 (one lw/sw) */

s32 PakMenu_FreePagesText(MenuItem *item, Menu *own) {
    Chars4 buf;
    char *bp;
    s32 pad;
    s32 freeb;
    s32 need;
    char *dst;
    s32 r;
    char *t;
    u8 *fmt;

    if (gPakMenuAnyPak != 0) {
        pad = gPakMenuPort;
    } else {
        pad = own->controller->port;
    }
    buf = sPakMenuText_Spaces3View;
    bp = buf.c;
    if (gPakMenuIsSave != 0) {
        item->text = sPakMenuText_FreePagesXxxNeededXxx_B48B4;
        if (Pak_GetFreePages(pad, &freeb) == 0) {
            if (gPakMenuIsProfile != 0) {
                need = Pak_BytesToPages(0x18);
            } else {
                need = Pak_BytesToPages(gScene.unk_30 + 0x688);
            }
            dst = item->text;
            dst += 0xC;
            sprintf(buf.c, sPakMenuFmt_3d, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            sprintf(buf.c, sPakMenuFmt_3d, need);
            dst += 0xC;
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
        }
    } else {
        if (gPakMenuAnyPak != 0) {
            t = sPakMenuText_FreePagesXxxNeededXxx_B4914;
        } else if (gPakMenuIsLoad != 0) {
            t = sPakMenuText_FreePagesXxxNeededXxx_B48B4;
        } else {
            goto none;
        }
        item->text = t;
        r = Pak_GetFreePages(pad, &freeb);
        need = Pak_BytesToPages(gScene.unk_30 + 0x688);
        dst = item->text;
        if (r == 0) {
            dst += 0xC;
            fmt = sPakMenuFmt_3d;
            sprintf(bp, fmt, freeb);
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
            sprintf(bp, sPakMenuFmt_3d, need);
            dst += 0xC;
            *dst = buf.c[0];
            dst++;
            *dst = buf.c[1];
            dst++;
            *dst = buf.c[2];
        }
    }
    goto done;
none:
    item->text = sPakMenuText_Space;
done:
    return 0;
}

/* Save/load menu "select pak" handler: checks the pak in the chosen slot;
 * when saving over an existing game file asks for confirmation, when there
 * is no usable pak cycles to the next slot with one. */


/* PakMenu_FindNoteByName, inlined (the found path sets the result before storing the index) */
static __inline__ s32 findFile(void *unused, char *file, s32 slot, s32 *index) {
    char name[16];
    char *entry;
    s32 found;
    s32 i;

    for (i = 0; i < 16; i++) {
        Pak_GetNoteGameName(slot, i, &entry);
        Pak_DecodeText(entry, name, 16);
        if (Str_Compare(name, file) == 0) {
            found = 1;
            if (index != NULL) {
                *index = i;
            }
            goto end;
        }
    }
    found = 0;
end:
    return found;
}

static __inline__ void menuSlot(Menu *c, s32 *slot) {
    if (gPakMenuAnyPak) {
        *slot = gPakMenuPort;
    } else {
        *slot = c->controller->port;
    }
}

s32 PakMenu_SelectPak(void *menu, Menu *c, s32 arg2) {
    s32 slot;
    s32 pak;
    s32 n;
    s32 next;
    s32 cur;

    slot = 0;
    if (gPakMenuIsSave) {
        menuSlot(c, &slot);
    }
    if (gPakMenuAnyPak) {
        pak = gPakMenuPort;
    } else {
        pak = slot;
    }
    if (PakMenu_CheckPak(c, pak, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (gPakMenuIsProfile) {
        if (gPakMenuIsSave) {
            PakMenu_WriteProfile((sptr) menu, c, arg2);
            return 1;
        }
    } else if (gPakMenuIsSave) {
        gPakNoteIndex = -1;
        if (findFile(menu, gPakGameNoteName, pak, &gPakNoteIndex) && gPakNoteIndex >= 0 && gPakNoteIndex < 16) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakConfirmOverwriteDesc, c->player, c->controller, &gPakSaveGameMenuDesc);
            return 1;
        }
        ((s32 (*)(void *, Menu *, s32)) PakMenu_WriteSaveGame)(menu, c, arg2); /* passes a third word the definition does not take */
        return 1;
    }
    if (gPakMenuAnyPak) {
        if (gPakMenuPort == -1) {
            next = 0;
        } else {
            next = gPakMenuPort + 1;
            if (next >= 4) {
                next = 0;
            }
        }
        for (n = 0; n < 4; n++) {
            Pak_ProbeSlot(next);
            if (Pak_GetStatus(next) != -2) {
                break;
            }
            next++;
            if (next >= 4) {
                next = 0;
            }
        }
        if (n == 4) {
            next = -1;
        }
        gPakMenuPort = next;
        if (next == -1) {
            gPakCheckDone = gPakMenuLeaving = 1;
            return 1;
        }
        if (gPakMenuAnyPak) {
            cur = gPakMenuPort;
        } else {
            cur = c->controller->port;
        }
        slot = cur;
        if (PakMenu_CheckPak(c, cur, 0) != 0) {
            gPakMenuLeaving = 1;
            return 1;
        }
        PakMenu_UpdateNoteRows(c);
    }
    return 0;
}

void *CPlayerPool__FindByController(char *object, sptr arg1); /* code/CPlayer's definition */

s32 PakMenu_SelectCancel(s32 a0, Menu *arg) {
    struct {
        s32 m;
        s32 pad;
    } mm;
    u8 *tbl;
    CPlayer *plr;
    s32 sw;
    CPlayer *u;

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
    gCameraPoolHideViews = 0;
    if (gPakMenuAnyPak != 0) {
        mm.m = gPakMenuPort;
    } else {
        mm.m = arg->controller->port;
    }
    if (gPakMenuAnyPak != 0) {
        gPakCheckDone = 0;
        gPakMenuStartPressed = 0;
        gPakBootPending = 1;
        gPakMenuConfirmed = 0;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakSaveGameAnyPakMenuDesc, 0, (sptr)arg->controller, 0);
        return 1;
    }
    if (gPakMenuIsProfile != 0) {
        if (gPakMenuIsLoad != 0) {
            gPakMenuLeaving = 1;
            plr = CPlayerPool__FindByController((char *) &gEngineApp_m_Game.m_PlayerList, (sptr) arg->controller);
            MenuPool_OpenMenu((u8 *)plr->m_pCamera->m_MenuPool, (u8 *)&gCharacterMenuDesc, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
            return 1;
        }
        if (gPakMenuIsSave != 0) {
            gPakMenuLeaving = 1;
            u = (CPlayer *)arg->player;
            sw = arg->controller->port;
            mm.m = sw;
            plr = u;
            switch (sw) {
            case 0:
            default:
                tbl = (u8 *)&gStatsAfterPakMenuDesc1;
                break;
            case 1:
                tbl = (u8 *)&gStatsAfterPakMenuDesc2;
                break;
            case 2:
                tbl = (u8 *)&gStatsAfterPakMenuDesc3;
                break;
            case 3:
                tbl = (u8 *)&gStatsAfterPakMenuDesc4;
                break;
            }
            MenuPool_OpenMenu((u8 *)plr->m_pCamera->m_MenuPool, tbl, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
            return 1;
        }
        return 0;
    }
    plr = CPlayerPool__FindByController((char *) &gEngineApp_m_Game.m_PlayerList, (sptr) arg->controller);
    if (gPakMenuIsLoad != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, (u8 *)&gSingleMenuAfterPakDesc, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
        return 1;
    }
    if (gPakMenuIsSave == 0) {
        return 0;
    } else {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)plr->m_pCamera->m_MenuPool, (u8 *)&gSavePortalMenuDescB, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
        gCameraPoolHideViews = 0;
        return 1;
    }
}

/* local: globals/game.h declares these, but its gEngineApp_m_Settings_unk180 (u8) clashes with the array view below, which the ROM needs here */
extern GameSettings gEngineApp_m_Settings; /* local: globals/game.h is not included here (its field symbols clash with this file's views of unlockedCheats/unk180) */
extern GameMatch gEngineApp_m_Settings_match; /* local type: globals/game.h declares it; this view (its type here) is what keeps the ROM's code */
extern CGame gEngineApp_m_Game; /* local: as above */
/* Load game menu "select" handler: with the pak checked, either restores the
 * loaded save (player block, game progress, level times, the level's key
 * bytes) and opens the "game loaded" dialog, or opens the "no save"/"bad
 * save" dialog for the pak note. */

extern u32 gUnlockedCheatsView[] __asm__("gEngineApp_m_Settings_unlockedCheats"); /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u8 gSettingsUnk180View[] __asm__("gEngineApp_m_Settings_unk180"); /* local type: GameSettings.unk180 read as an array (other registers otherwise) */
extern f32 gEngineApp_m_Settings_match_seconds[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 gEngineApp_m_Settings_match_minutes[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 gEngineApp_m_Settings_match_hours[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s32 gEngineApp_m_Game_m_PlayerList_m_bCinema[2]; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 GameNoteMenu_SelectYes(void *menu, Menu *c) {
    s32 slot;
    CPlayer *player;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;
    CScene *world;

    if (gPakMenuAnyPak) {
        slot = gPakMenuPort;
    } else {
        slot = c->controller->port;
    }
    if (PakMenu_CheckPak(c, slot, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (sGameNoteLoad != 0) {
        player = CPlayerPool__FindByController((char *) &gEngineApp_m_Game.m_PlayerList, (sptr) c->controller);
        world = &gScene;
        keep = gSettingsUnk180View[0];
        player->m_Data = gLoadBuffer->player;
        *(GameSettingsBlock *)&gEngineApp_m_Settings = gLoadBuffer->settings;
        gEngineApp_m_Settings_match.frames = gLoadBuffer->frames;
        gEngineApp_m_Settings_match_seconds[0] = gLoadBuffer->seconds;
        gEngineApp_m_Settings_match_minutes[0] = gLoadBuffer->minutes;
        gScene_m_PreviewOptions = gUnlockedCheatsView[0];
        gEngineApp_m_Settings_match_hours[0] = gLoadBuffer->hours;
        src = gLoadBuffer->levelFlags;
        dst = gLoadSavePersistentData;
        gSettingsUnk180View[0] = keep;
        for (i = 0; i < world->unk_30; i++) {
            *dst++ = *src++;
        }
        gEngineApp_m_Game_m_PlayerList_m_bCinema[0] = 0;
        gEngineApp_m_Game_m_PlayerList_m_bCinema[1] = 0;
#ifdef TARGET_PORT
        /* the port: m_bCinema + 0x588 is gEngineApp_m_Game.m_MenuPool at the N64 offsets */
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakMsgRumblePakDesc, (sptr) player, (sptr) player->m_ContState.m_pContState, 0);
#else
        MenuPool_OpenMenu((u8 *) gEngineApp_m_Game_m_PlayerList_m_bCinema + 0x588, &gPakMsgRumblePakDesc, (sptr) player, player->m_ContState.m_pContState, NULL);
#endif
        return 1;
    }
    if (Pak_DeleteNote(slot, gPakNoteIndex) == 0) {
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakMsgDeleteDoneDesc, c->player, c->controller, c->context);
    } else {
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakMsgDeleteFailedDesc, c->player, c->controller, c->context);
    }
    return 1;
}

void CPlayer__SetCharacter(CPlayer *a, s32 b);

/* Turok 2 inlines this (Rage Wars calls it) */
static inline void O01Restore(CPlayer *a) {
    CPlayerOptions *s = a->m_pOptions;
    s32 i;
    s->m_nCharacter = gPakProfile.character;
    s->m_Suicides = gPakProfile.suicides;
    s->m_Deaths = gPakProfile.deaths;
    s->m_Kills = gPakProfile.kills;
    s->m_Tags = gPakProfile.tags;
    for (i = 0; i < 8; i++) {
        s->m_Name[i] = gPakProfile.name[i];
    }
}

s32 CharNoteMenu_SelectYes(s32 unused, Menu *own) {
    s32 pad;
    CPlayer *plr;
    if (gPakMenuAnyPak != 0) {
        pad = gPakMenuPort;
    } else {
        pad = own->controller->port;
    }
    if (PakMenu_CheckPak(own, pad, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (sCharNoteLoad != 0) {
        plr = CPlayerPool__FindByController((char *) &gEngineApp_m_Game.m_PlayerList, (sptr) own->controller);
        own->player = plr;
        O01Restore(plr);
        CPlayer__SetCharacter(plr, (s8) gPakProfileRead_character);
        MenuPool_OpenChildMenu((u8 *)plr->m_pCamera->m_MenuPool, own, (u8 *)&gPlayerSetupMenuDesc);
        return 1;
    }
    if (Pak_DeleteNote(pad, gPakNoteIndex) == 0) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgDeleteDoneDesc, own->player, own->controller, (void *)own->context);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgDeleteFailedDesc, own->player, own->controller, (void *)own->context);
    }
    return 1;
}

/* Controller pak menu: closes the current message box and shows the next one for the menu state. */

void Controller_ResetRumble(ControllerProfile *a);
void CEngineApp__StartCinemaGame(u8 *a);

s32 PakMessage_SelectOk(s32 a0, Menu *o) {
    s32 s0, s1, v;
    u8 *p;

    if (gPakMessageKind == 8) {
        return 0;
    }
    if (o->context == &gSavePortalMenuDescB || o->context == &gSavePortalMenuDesc) {
        gCameraPoolHideViews = 0;
    }
    if (gPakMessageKind == 9 || gPakMessageKind == 5) {
        Controller_ResetRumble(o->controller);
        Controller_SetRumbleOption(o->controller, 1);
    }
    if (gPakMessageKind == 9) {
        if (gPakMenuIsLoad != 0 && gPakMenuIsProfile == 0 && sPakMessageRumbleB == 0) {
            if (gPakMenuAnyPak != 0) {
                s1 = gPakMenuPort;
            } else {
                s1 = o->controller->port;
            }
            Controller_UnwatchPak(s1);
            CScene__RequestWarp(&gScene, o->player->m_Data.m_CheckPointWarpID, 2);
            return 1;
        }
        CEngineApp__StartCinemaGame(&gEngineApp);
        return 1;
    }
    if (gPakMenuAnyPak != 0) {
        s1 = gPakMenuPort;
    } else {
        s1 = o->controller->port;
    }
    if (gPakMessageKind == 5) {
        Controller_UnwatchPak(s1);
        CScene__RequestWarp(&gScene, gScene_m_PendingWarpID, 2);
    }
    if (gPakMenuAnyPak != 0) {
        if (gPakMessageKind == 6) {
            gPakCheckDone = 1;
        }
    }
    if (gPakMessageKind == 4 && gPakMenuIsLoad != 0 && gPakMenuIsProfile == 0) {
        Pak_ProbeSlot(s1);
        s0 = Pak_GetStatus(s1);
        v = Controller_TakePakChanged(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, o->player, o->controller, 0);
        } else {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gSingleMenuAfterPakDesc, o->player, o->controller, 0);
        }
    } else if (gPakMessageKind == 3 && gSavePortalActive != 0) {
        Pak_ProbeSlot(s1);
        s0 = Pak_GetStatus(s1);
        v = Controller_TakePakChanged(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakSaveGameMenuDesc, o->player, o->controller, 0);
        } else {
            if (o->player != NULL) {
                p = (u8 *)o->player->m_pCamera->m_MenuPool;
            } else {
                p = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu(p, &gSavePortalMenuDescB, o->player, o->controller, 0);
            gCameraPoolHideViews = 0;
        }
    } else {
        if (o->context == (void *)1) {
            o->context = NULL;
        }
        if (o->context != NULL) {
            if (o->context != &gPakSaveProfileMenuDesc && o->player != NULL) {
                p = (u8 *)o->player->m_pCamera->m_MenuPool;
            } else {
                p = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu(p, o->context, o->player, o->controller, s1);
        }
    }
    return 1;
}
s32 PakConfirmMenu_SelectYes(void *owner, MenuRecAt24 *menu, void *arg);

s32 PakConfirmMenu_SelectYes(void *owner, MenuRecAt24 *menu, void *arg) {
    s32 ch;
    switch (gPakConfirmKind) {
        case 4:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak((struct Menu *) menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, gPakOverwriteNote) == 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteDoneDesc, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteFailedDesc, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 3:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak((struct Menu *) menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, gPakNoteIndex) == 0) {
                ((s32 (*)(void *, MenuRecAt24 *, void *)) PakMenu_WriteSaveGame)(owner, menu, arg); /* passes a third word the definition does not take */
                return 1;
            }
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteFailedDesc, menu->player, menu->slot, menu->prompt);
            break;
        case 0:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak((struct Menu *) menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_RepairIfDamaged(ch) == 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgRepairedDesc, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakInGameMenuDesc, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 1:
        case 2:
            if (gPakMenuAnyPak != 0) {
                gPakMenuConfirmed = 1;
            }
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
            break;
    }
    return 1;
}

/* Menu item handler: depending on the menu state, checks the item (PakMenu_CheckPak) and prints its text, or (states 1
 * and 2) resets the menu flags and opens the item's menu on its owner's camera. */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s8 id;
} ItemData;
typedef struct {
    /* the host Menu's leading fields (structs/menu.h: cursor .. param), pointers 8 bytes wide */
    s16 cursor, offsetX, offsetY;
    u8 _pad6[2];
    void *block, *items;
    s32 count;
    void *desc, *param;
    /* 0x1C */ CPlayer *owner;
    /* 0x20 */ ItemData *data;
    /* 0x24 */ void *string;
} Item;

static __inline__ s32 ItemId(Item *item) {
    s32 id;

    if (gPakMenuAnyPak != 0) {
        id = gPakMenuPort;
    } else {
        id = item->data->id;
    }
    return id;
}

s32 PakConfirmMenu_SelectNo(s32 arg0, Item *item) {
    void *string;

    switch (gPakConfirmKind) {
        case 4:
            if (PakMenu_CheckPak((struct Menu *) item, ItemId(item), 1)) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            }
            break;
        case 3:
            if (PakMenu_CheckPak((struct Menu *) item, ItemId(item), 1)) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            }
            break;
        case 0:
            if (PakMenu_CheckPak((struct Menu *) item, ItemId(item), 1)) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            }
            break;
        case 1:
        case 2:
            gPakMenuLeaving = 1;
            gCameraPoolHideViews = 0;
            if (gPakMenuAnyPak) {
                gPakCheckDone = 1;
                return 1;
            }
            string = PakMenu_GetReturnMenu((struct Menu *) item);
            if (string == NULL) {
                string = item->string;
            }
            if (item->string == &gSavePortalMenuDescB || item->string == &gSavePortalMenuDesc) {
                gCameraPoolHideViews = 0;
            }
            MenuPool_OpenMenu(item->owner != NULL ? item->owner->m_pCamera->m_MenuPool : &gEngineApp_m_Game_m_MenuPool, string, item->owner,
                          item->data, 0);
            break;
    }
    return 1;
}
void PakMenu_PickDefaultPort(void);

void PakMenu_PickDefaultPort(void) {
    s32 var_s0;
    s32 var_s1;
    s32 unavailable;
    if (gPakMenuPort == -1) {
        var_s0 = 0;
        var_s1 = 0;
    unavailable = -2;
loop_2:
        Pak_ProbeSlot(var_s0);
        if (Pak_GetStatus(var_s0) == unavailable) {
            var_s0 += 1;
            if (var_s0 >= 4) {
                var_s0 = 0;
            }
            var_s1 += 1;
            if (var_s1 < 4) {
                goto loop_2;
            }
        }
        if (var_s1 == 4) {
            var_s0 = -1;
        }
        gPakMenuPort = var_s0;
    }
}

s32 PakMenu_HasPort(void) {
    return ~gPakMenuPort != 0;
}

void Pak_ClearDamagedContinued(void) {
    gDamagedPakContinued = 0;
}

s32 Pak_IsTurok2Note(char *a, char *b) {
    if (Str_Length(b) == 4 && Str_Length(a) == 2 && Str_Compare(b, gPakGameCode) == 0 && Str_Compare(a, gPakCompanyCode) == 0) {
        return 1;
    }
    return 0;
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80405DC0_16.CPP, func_80409884_de, with Turok 2's layouts. */


char *Pak_GetBufferSize(s32 block) {
    if (block == 0) {
        return (*(char * *) &gScene.unk_30) + 0x688;
    }
    return (char *) (block << 8);
}

typedef struct {
    /* 0x000 */ u8 pad0[0x684];
    /* 0x684 */ s32 unk684;
} Entry;

void Pak_SetBufferSize(s32 arg0, s32 arg1) {
    ((Entry *) (gScene.unk_30 + arg0))->unk684 = arg1;
}
s32 func_00421320(s32 *block);

s32 func_00421320(s32 *block) {
    return ((struct Shape_typemap_3 *) (block[0] + (sptr) block))[-1].field_0;
}

void Pak_AllocBuffer(void ***handle, void **data, s32 units) {
    void **block;
    void *first;
    if (*handle != 0) {
        CCache__Flush_Arg(0);
        CCache__Deallocate(0, *handle);
    }
    if (units != 0) {
        units <<= 8;
    } else {
        units = (s32)((*(char * *) &gScene.unk_30) + 0x688);
    }
    block = CCache__Allocate(0, units, 0x33, sPakMenuText_SingleSaveBlock);
    *handle = block;
    first = *block;
    *data = first;
    memset(first, 0, units);
}

void Pak_FreeSaveBuffers(void) {
    if ((gSaveBufferHandle != 0) || (gLoadBufferHandle != 0)) {
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

void PakMenu_SaveCharacter(CPlayer *a) {
    CPlayerOptions *p = a->m_pOptions;
    s32 i;
    gPakProfile.character = p->m_nCharacter;
    gPakProfile.suicides = p->m_Suicides;
    gPakProfile.deaths = p->m_Deaths;
    gPakProfile.kills = p->m_Kills;
    gPakProfile.tags = p->m_Tags;
    gPakProfile.reserved[0] = 0;
    gPakProfile.reserved[1] = 0;
    gPakProfile.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        gPakProfile.name[i] = p->m_Name[i];
    }
}

/* Copies the saved values into the options of the player. */
void PakMenu_RestoreCharacter(CPlayer *arg0)
{
  s32 var_a0;
  u8 temp_v0;
  CPlayerOptions *temp_a1;
  CPlayerOptions *var_v1;
  temp_a1 = arg0->m_pOptions;
  temp_a1->m_nCharacter = *(u8 *) &gPakProfile.character;
  temp_a1->m_Suicides = ((u16 *) &gPakProfile.character)[-4];
  temp_a1->m_Deaths = ((u16 *) &gPakProfile.character)[-3];
  temp_a1->m_Kills = ((u16 *) &gPakProfile.character)[-2];
  var_a0 = 0;
  temp_a1->m_Tags = ((u16 *) &gPakProfile.character)[-1];
  var_v1 = temp_a1;
  do
  {
    temp_v0 = gPakProfile.name[var_a0];
    var_v1 = (CPlayerOptions *) ((u8 *) temp_a1 + var_a0);
    var_a0 += 1;
    var_v1->m_Name[0] = temp_v0;
  }
  while (var_a0 < 8);
}

extern CScene gScene; /* local: globals/scene.h is not included here (its gScene_m_PreviewOptions would clash with this file's local-type view) */
extern s32 gScenePreviewOptionsView[] __asm__("gScene_m_PreviewOptions"); /* local type: globals/scene.h declares it; this view (its type here) is what keeps the ROM's code */
extern u8 gLoadSavePersistentData[]; /* local: globals/scene.h is not included here (its gScene_m_PreviewOptions would clash with this file's local-type view) */

void SaveGame_Restore(CPlayer *p) {
    CScene *level = &gScene;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;

    keep = gEngineApp_m_Settings.unk180;
    p->m_Data = gLoadBuffer->player;
    *(GameSettingsBlock *)&gEngineApp_m_Settings = gLoadBuffer->settings;
    gEngineApp_m_Settings.match.frames = gLoadBuffer->frames;
    gEngineApp_m_Settings.match.seconds = gLoadBuffer->seconds;
    gEngineApp_m_Settings.match.minutes = gLoadBuffer->minutes;
    gEngineApp_m_Settings.match.hours = gLoadBuffer->hours;
    gScenePreviewOptionsView[0] = gEngineApp_m_Settings.unlockedCheats;
    gEngineApp_m_Settings.unk180 = keep;
    src = gLoadBuffer->levelFlags;
    dst = gLoadSavePersistentData;
    for (i = 0; i < level->unk_30; i++) {
        *dst++ = *src++;
    }
}

s32 PakMenu_IsSavePortalMenu(char *arg0) {
    char *p = *(char **) (arg0 + 0x24);

    if (p == (char *)&gSavePortalMenuDescB || p == (char *)&gSavePortalMenuDesc) {
        gCameraPoolHideViews = 0;
        return 1;
    }
    return 0;
}

s32 PakMenu_GetPort(void *arg0) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return *(s8 *)(*(u8 **)((u8 *)arg0 + 0x20) + 4);
}

static __inline__ s32 func_80409BDC_de(struct Menu *record) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return record->controller->port;
}

s32 PakMenu_PollPak(struct Menu *record) {
    if (gPakMenuLeaving == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (Controller_PakChanged(func_80409BDC_de(record)) != 0) {
                if (PakMenu_CheckPak((struct Menu *) record, func_80409BDC_de(record), 0) != 0) {
                    gPakMenuLeaving = 1;
                }
            }
        }
    }
    return gPakMenuLeaving;
}

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 PakMenu_PollPakB(Menu *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (gPakMenuLeaving == 0) {
        if (gPakMenuAnyPak != 0) {
            var_v1 = gPakMenuPort;
        } else {
            var_v1 = (s32) arg0->controller->port;
        }
        if (var_v1 != -1) {
            if (gPakMenuAnyPak != 0) {
                var_a0 = gPakMenuPort;
            } else {
                var_a0 = (s32) arg0->controller->port;
            }
            if (Controller_PakChanged(var_a0) != 0) {
                if (gPakMenuAnyPak != 0) {
                    var_a1 = gPakMenuPort;
                } else {
                    var_a1 = (s32) arg0->controller->port;
                }
                if (PakMenu_CheckPak((struct Menu *) arg0, var_a1, 0) != 0) {
                    gPakMenuLeaving = 1;
                }
            }
        }
    }
    return gPakMenuLeaving;
}

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 PakMenu_PollStartPressed(Menu *arg0) {
    if ((arg0->controller->buttons & 0x1000) && (sPakMenuStartLatched == 0)) {
        gPakMenuStartPressed = 1;
        sPakMenuStartLatched = 1;
    }
    return gPakMenuStartPressed;
}

extern void Controller_UpdatePakInsertion();

s32 PakMenu_PollBootPak(s32 arg0) {
    if (gPakMenuStartPressed == 0) {
        ((void (*)(void *)) CEngineApp__UpdateControllers)(&gEngineApp);
        Controller_UpdatePakInsertion();
        if (gPakMenuStartPressed == 0) {
            if (gPakBootPending != 0) {
                gPakCheckDone = 1;
            }
            gPakCheckHasPak = 0;
            gPakMenuLeaving = 1;
        }
    }
    if (gPakMenuLeaving == 0) {
        if ((gPakMenuPort != -1) && (Controller_PakChanged(gPakMenuPort) != 0) && (PakMenu_CheckPak((struct Menu *) arg0, gPakMenuPort, 0) != 0)) {
            gPakCheckHasPak = 0;
            gPakMenuLeaving = 1;
        }
    }
    return gPakMenuLeaving;
}

sptr PakMenu_GetPlayerAndPort(Menu *arg0, s32 *arg1) {
    sptr var_a0;
    s32 var_v0;

    if (gPakMenuAnyPak != 0) {
        var_v0 = gPakMenuPort;
        var_a0 = 0;
    } else {
        var_a0 = arg0->player;
        var_v0 = arg0->controller->port;
    }
    *arg1 = var_v0;
    return var_a0;
}
void PakMenu_ProbeOwnerPort(struct Shared_PakMenu *record);

void PakMenu_ProbeOwnerPort(struct Shared_PakMenu *record) {
    s32 value;
    if (gPakMenuAnyPak != 0) {
        value = gPakMenuPort;
    } else {
        value = record->owner->unk4;
    }
    Pak_ProbeSlot(value);
}

void PakMenu_UnwatchAndWarp(s32 arg0, s32 arg1) {
    Controller_UnwatchPak(arg1);
    CScene__RequestWarp(&gScene, gScene_m_PendingWarpID, 2);
}

s32 PakMenu_FindNoteByName(void *unused, void *arg1, s32 arg2, s32 *arg3) {
    char sp10[16];
    sptr sp20;
    s32 var_s0;
    for(var_s0=0;var_s0<16;var_s0++) {
        Pak_GetNoteGameName(arg2,var_s0,&sp20);
        Pak_DecodeText(sp20,&sp10,16);
        if(Str_Compare(&sp10,arg1)==0) {
            if(arg3) *arg3=var_s0;
            return 1;
        }
    }
    return 0;
}

char *PakMenu_GetStatsReturnMenu(s32 unused, s32 which) {
    switch (which) {
        case 0:
        default:
            return (char *)&gStatsAfterPakMenuDesc1;
        case 1:
            return (char *)&gStatsAfterPakMenuDesc2;
        case 2:
            return (char *)&gStatsAfterPakMenuDesc3;
        case 3:
            return (char *)&gStatsAfterPakMenuDesc4;
    }
}
s32 PakMenu_FindNextPakPort(s32 arg0);

s32 PakMenu_FindNextPakPort(s32 arg0) {
    s32 var_s0;
    s32 var_s1;
    s32 unavailable;
    var_s0 = 0;
    if (arg0 != -1) {
        var_s0 = arg0 + 1;
        if (var_s0 >= 4) {
            var_s0 = 0;
        }
    }
    var_s1 = 0;
    unavailable = -2;
loop_4:
    Pak_ProbeSlot(var_s0);
    if (Pak_GetStatus(var_s0) == unavailable) {
        var_s0 += 1;
        if (var_s0 >= 4) {
            var_s0 = 0;
        }
        var_s1 += 1;
        if (var_s1 < 4) {
            goto loop_4;
        }
    }
    if (var_s1 == 4) {
        var_s0 = -1;
    }
    return var_s0;
}

s32 MenuItem_TextLength(void *item);

s32 PakMenu_UpdateModeText(MenuItem *item) {
    s32 player;

    if (gPakMenuIsProfile != 0 && gPakMenuIsSave != 0) {
        item->text = sPakMenuText_SaveCharacter;
    } else if (gPakMenuIsProfile == 0 && gPakMenuIsSave != 0) {
        item->text = sPakMenuText_SaveGame;
    } else if (gPakMenuAnyPak != 0) {
        player = gPakMenuPort;
        item->text = sPakMenuText_ControllerPakX_B48F8;
        sprintf(item->text + (MenuItem_TextLength(item) - 1), PAKMENU_FMT_1D, player + 1);
    }
    return 0;
}

s32 PakMenu_UpdateTitle(MenuItem *item, Menu *event) {
    s32 port;

    if (gPakMenuAnyPak != 0) {
        item->text = sPakMenuText_Space;
    } else {
        port = event->controller->port;
        item->text = sPakMenuText_ControllerPakX_B45E0;
        sprintf(item->text + (MenuItem_TextLength(item) - 1), sPakMenuFmt_1d, port + 1);
    }
    return 0;
}

s32 PakMenu_UpdateSubtitle(MenuItem *item, s16 *id) {
    s32 n = *id;
    u32 k = n - 3;

    if (gPakMenuIsLoad != 0) {
        if (gPakMenuIsProfile != 0) {
            item->text = sPakMenuText_LoadNote_B4870;
        } else {
            item->text = sPakMenuText_LoadNote_B4888;
        }
    } else if (gPakMenuIsSave != 0) {
        if (k < 16) {
            item->text = sPakMenuText_DeleteNote;
            return 0;
        }
        if (n == 21) {
            item->text = sPakMenuText_Space;
        } else if (gPakMenuIsProfile != 0) {
            item->text = sPakMenuText_SaveNote_B487C;
        } else {
            item->text = sPakMenuText_SaveNote_B4894;
        }
    } else if (gPakMenuAnyPak != 0) {
        item->text = sPakMenuText_ControllerPakMenu;
    }
    return 0;
}

#define T2_TEXT(s) ((s32 *) (s))
#ifdef TARGET_PORT
#else
#endif

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = T2_TEXT(sPakMenuText_Exit);
    }
    return 0;
}

void GameNoteMenu_InitLoad(void) {
    sGameNoteLoad = 1;
}

void GameNoteMenu_InitDelete(void) {
    sGameNoteLoad = 0;
}

s32 GameNoteMenu_UpdateTitle(MenuItem *arg0) {
    if (sGameNoteLoad != 0) {
        arg0->table = T2_TEXT(sGameNoteMenuText_NoteInfo);
    } else {
        arg0->table = T2_TEXT(sGameNoteMenuText_DeleteNote);
    }
    return 0;
}

s32 GameNoteMenu_UpdateDifficulty(MenuItem *item) {
    switch (gLoadBuffer->settings.difficulty) {
    default:
    case 0:
        item->text = sGameNoteMenuText_DifficultyEasy;
        break;
    case 16:
        item->text = sGameNoteMenuText_DifficultyNormal;
        break;
    case 32:
        item->text = sGameNoteMenuText_DifficultyHard;
        break;
    }
    return 0;
}

s32 GameNoteMenu_UpdateHealth(MenuItem *item) {
    s32 value = gLoadBuffer->player.m_Health >> 8;

    sprintf(item->text + (MenuItem_TextLength(item) - 3), sPakMenuFmt_3d, value);
    return 0;
}

s32 GameNoteMenu_UpdateLives(MenuItem *item) {
    s32 value = gLoadBuffer->player.m_Lives;

    sprintf(item->text + (MenuItem_TextLength(item) - 2), PAKMENU_FMT_2D, value);
    return 0;
}

s32 Level_GetHub(s32 id);

s32 GameNoteMenu_UpdateLevel(MenuItem *item) {
    switch (Level_GetHub(gLoadBuffer->player.m_CheckPointWarpID)) {
        case -1:
        default:
            item->text = sGameNoteMenuText_UnknownLevelId;
            break;
        case 0:
            item->text = sGameNoteMenuText_TheHub;
            break;
        case 1:
            item->text = sGameNoteMenuText_PortOfAdia;
            break;
        case 2:
            item->text = sGameNoteMenuText_RiverOfSouls;
            break;
        case 3:
            item->text = sGameNoteMenuText_DeathMarshes;
            break;
        case 4:
            item->text = sGameNoteMenuText_LairOfTheBlindOnes;
            break;
        case 5:
            item->text = sGameNoteMenuText_HiveOfTheMantids;
            break;
        case 6:
            item->text = sGameNoteMenuText_PrimagensLightship;
            break;
    }
    return 0;
}

s32 GameNoteMenu_UpdateTime(MenuItem *item) {
    s32 seconds = gLoadBuffer->seconds;
    s32 minutes = gLoadBuffer->minutes;
    s32 hours = gLoadBuffer->hours;

    sprintf(item->text + (MenuItem_TextLength(item) - 9), PAKMENU_FMT_TIME, hours, minutes, seconds);
    return 1;
}

s32 GameNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (sGameNoteLoad != 0) {
        arg0->table = T2_TEXT(sCharNoteMenuText_Load);
    } else {
        arg0->table = T2_TEXT(sCharNoteMenuText_Delete_B4B8C);
    }
    return 0;
}

#ifdef TARGET_PORT
/* The port: RecordRecAt24 reads the player, the controller and the context at N64 offsets 0x1C/0x20/0x24;
 * the host Menu (structs/menu.h) has them pointer-wide further on. */
s32 GameNoteMenu_SelectNo(void *unused, Menu *record) {
    if (sGameNoteLoad != 0) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, (sptr) record->player,
                          (sptr) record->controller, 0);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, record->context, (sptr) record->player,
                          (sptr) record->controller, 0);
    }
    return 1;
}
#else
/* The port body's signature; the N64 body reads the record through the Rage Wars view (a macro). */
#define record ((struct RecordRecAt24 *) recordArg)
s32 GameNoteMenu_SelectNo(void *unused, Menu *recordArg) {
    do {
        if (sGameNoteLoad != 0) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, record->first, record->second, 0);
        } else {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, record->resource, record->first, record->second, 0);
        }
    } while (0);
    return 1;
}
#undef record
#endif

void CharNoteMenu_InitLoad(void) {
    sCharNoteLoad = 1;
    gCharNoteMenuSaving = 0;
}

void CharNoteMenu_InitDelete(void) {
    sCharNoteLoad = 0;
    gCharNoteMenuSaving = 0;
}
void CharNoteMenu_InitSave(struct RecordRecAtCV1 *record);

void CharNoteMenu_InitSave(struct RecordRecAtCV1 *record) {
    record->target->first &= ~0x01800000;
    sCharNoteLoad = 0;
    gCharNoteMenuSaving = 1;
    record->target->second &= ~0x01800000;
}

s32 CharNoteMenu_UpdateTitle(struct FieldRecAt14 *field) {
    if (sCharNoteLoad != 0) {
        field->text = sCharNoteMenuText_NoteInfo;
    } else if (gCharNoteMenuSaving != 0) {
        field->text = sCharNoteMenuText_DeleteNote_B4AF8;
    } else {
        field->text = sCharNoteMenuText_DeleteNote_B4AEC;
    }
    return 0;
}

s32 CharNoteMenu_UpdateName(MenuItem *item) {
    s32 len = 8;
    u8 *src;
    char *field;
    char *p;
    s32 i;
    char c;

    if (gCharNoteMenuSaving != 0) {
        item->text = sCharNoteMenuText_NameExt;
    } else {
        item->text = sCharNoteMenuText_Name;
        src = &gPakProfileRead.name[7];
        while (*src-- == ' ' && len != 0) {
            len--;
        }
        field = item->text;
        field += 11;
        c = ' ';
        p = &field[7];
        for (i = 7; i >= 0; i--) {
            *p-- = c;
        }
        field = &field[8 - len];
        for (i = 0; i < len; i++) {
            *field++ = gPakProfileRead.name[i];
        }
    }
    return 0;
}

extern s8 gPakProfileReadCharacterView[] __asm__("gPakProfileRead_character"); /* the s8 view keeps the ROM code */

s32 CharNoteMenu_UpdateType(MenuItem *item) {
    char *src;
    char *dst;
    s32 i;

    if (gCharNoteMenuSaving != 0) {
        item->text = sCharNoteMenuText_Empty;
    } else {
        item->text = sCharNoteMenuText_Type;
        switch (gPakProfileReadCharacterView[0]) {
            case 0:
            default:
                src = sCharNoteMenuText_Turok;
                break;
            case 1:
                src = sCharNoteMenuText_Adon;
                break;
            case 2:
                src = sCharNoteMenuText_Gant;
                break;
            case 3:
                src = sCharNoteMenuText_FleshEater;
                break;
            case 4:
                src = sCharNoteMenuText_PurrLin;
                break;
            case 5:
                src = sCharNoteMenuText_EndTrail;
                break;
            case 6:
                src = sCharNoteMenuText_Fireborn;
                break;
            case 7:
                src = sCharNoteMenuText_Iggy;
                break;
            case 8:
                src = sCharNoteMenuText_Campaigner;
                break;
            case 9:
                src = sCharNoteMenuText_Telset;
                break;
        }
        dst = item->text;
        dst += 8;
        for (i = 0; i < 11; i++) {
            *dst++ = *src++;
        }
    }
    return 0;
}

s32 CharNoteMenu_UpdateFrags(MenuItem *item) {
    s32 value = gPakProfileRead.kills;
    s32 offset = MenuItem_TextLength(item) - 4;

    sprintf(item->text + offset, PAKMENU_FMT_04D, value);
    return 0;
}

s32 CharNoteMenu_UpdateRank(MenuItem *item) {
    s32 score = gPakProfileRead.kills;
    char *src = NULL;
    char *dst;
    s32 i;

    if (score >= 0 && score < 50) {
        src = sCharNoteMenuText_Pathetic;
    } else if (score >= 50 && score < 150) {
        src = sCharNoteMenuText_Poor;
    } else if (score >= 150 && score < 300) {
        src = sCharNoteMenuText_Harmless;
    } else if (score >= 300 && score < 500) {
        src = sCharNoteMenuText_Nuisance;
    } else if (score >= 500 && score < 800) {
        src = sCharNoteMenuText_Acceptable;
    } else if (score >= 800 && score < 1300) {
        src = sCharNoteMenuText_Average;
    } else if (score >= 1300 && score < 2000) {
        src = sCharNoteMenuText_Good;
    } else if (score >= 2000 && score < 3000) {
        src = sCharNoteMenuText_Excellent;
    } else if (score >= 3000 && score < 4500) {
        src = sCharNoteMenuText_Dangerous;
    } else if (score >= 4500 && score < 6500) {
        src = sCharNoteMenuText_Brutal;
    } else if (score >= 6500 && score < 9999) {
        src = sCharNoteMenuText_Savage;
    } else if (score >= 9999) {
        src = sCharNoteMenuText_Elite;
    }
    dst = item->text;
    dst += 8;
    if (src != NULL) {
        for (i = 0; i < 11; i++) {
            *dst++ = *src++;
        }
    }
    return 0;
}

s32 CharNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (sCharNoteLoad != 0) {
        arg0->table = T2_TEXT(sCharNoteMenuText_Load);
    } else if (gCharNoteMenuSaving != 0) {
        arg0->table = T2_TEXT(sCharNoteMenuText_Delete_B4B94);
    } else {
        arg0->table = T2_TEXT(sCharNoteMenuText_Delete_B4B8C);
    }
    return 0;
}
int CharNoteMenu_SelectNo(int unused, ItemRecAt24 *item);

int CharNoteMenu_SelectNo(int unused, ItemRecAt24 *item) {
    if (sCharNoteLoad != 0) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakLoadProfileMenuDesc, item->x, item->y, 0);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->label, item->x, item->y, 0);
    }
    return 1;
}

void PakMessage_InitPakFull(void) {
    gPakMessageKind = 0;
}

void PakMessage_InitPakNotFound(void) {
    gPakMessageKind = 1;
}

void PakMessage_InitInsertToManage(void) {
    gPakMessageKind = 2;
}

void PakMessage_InitInsertToSave(void) {
    gPakMessageKind = 3;
}

void PakMessage_InitInsertToLoad(void) {
    gPakMessageKind = 4;
}

void PakMessage_InitInsertRumblePak(void) {
    gPakMessageKind = 5;
}

void PakMessage_InitCannotSave(void) {
    gPakMessageKind = 6;
}

void PakMessage_InitPakChanged(void) {
    gPakMessageKind = 7;
}

extern s32 gPakMessageKindView[] __asm__("gPakMessageKind"); /* the array view keeps the ROM code */
s32 Controller_DisableMenuSounds(void *, void *);              /* extern */

void PakMessage_InitNoController(Menu *arg0)
{
  s32 new_var2;
  MenuItem *temp_a0;
  MenuItem *temp_a1;
  int new_var;
  temp_a1 = arg0->items;
  new_var = temp_a1[7].flags & 0xFBFFFFFF;
  temp_a1[7].flags = (s32) new_var;
  temp_a0 = arg0->items;
  new_var2 = temp_a0[7].flags;
  gPakMessageKindView[0] = 8;
  temp_a0[7].flags = (s32) (new_var2 | 8);
  Controller_DisableMenuSounds(temp_a0, temp_a1);
}
void PakMessage_InitRumblePak(void);

void PakMessage_InitRumblePak(void) {
    gPakMessageKind = 9;
    sPakMessageRumbleB = 0;
}

void PakMessage_InitRumblePakB(void) {
    gPakMessageKind = 9;
    sPakMessageRumbleB = 1;
}

void PakMessage_InitSaveDone(void) {
    gPakMessageKind = 0xA;
}

void PakMessage_InitNoteCorrupt(void) {
    gPakMessageKind = 0xB;
}

void PakMessage_InitDeleteDone(void) {
    gPakMessageKind = 0xC;
}

void PakMessage_InitSaveFailed(void) {
    gPakMessageKind = 0xD;
}

void PakMessage_InitDeleteFailed(void) {
    gPakMessageKind = 0xE;
}

void PakMessage_InitRepaired(void) {
    gPakMessageKind = 0xF;
}

s32 PakMessage_UpdateHeading(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_other;
        case 2: goto state_other;
        case 3: goto state_other;
        case 4: goto state_other;
        case 5: goto case_1;
        case 6: goto case_1;
        case 7: goto state_other;
        case 8: goto state_other;
        case 9: goto case_1;
        case 10: goto case_1;
        case 11: goto state_other;
        case 12: goto case_1;
        case 13: goto state_other;
        case 14: goto state_other;
        case 15: goto case_1;
        }
state_other:
    field->text = sPakMessageText_Error;
    goto done;
case_1:
    field->text = sPakMessageText_Important;
done:
    return 0;
}

s32 PakMessage_UpdateLine1(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = sPakMessageText_ControllerPakIsFull;
    goto done;
case_1:
    field->text = sPakMessageText_ControllerPakNot_B4BFC;
    goto done;
case_2:
    field->text = sPakMessageText_ThisGameIsController_B4C48;
    goto done;
case_3:
    field->text = sPakMessageText_ThisGameIsController_B4D1C;
    goto done;
case_4:
    field->text = sPakMessageText_ThisGameIsController_B4CB4;
    goto done;
case_5:
    field->text = sPakMessageText_ThisGameIsRumblePak_B4D84;
    goto done;
case_6:
    field->text = sPakMessageText_ControllerPakNot_B4DEC;
    goto done;
case_7:
    field->text = sPakMessageText_ControllerPak;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E70;
    goto done;
case_9:
    field->text = sPakMessageText_ThisGameIsRumblePak_B4E90;
    goto done;
case_10:
    field->text = sPakMessageText_SaveWas;
    goto done;
case_11:
    field->text = sPakMessageText_ThisNoteIsCorrupt;
    goto done;
case_12:
    field->text = sPakMessageText_NoteWas;
    goto done;
case_13:
    field->text = sPakMessageText_ProblemOccurred_B4F98;
    goto done;
case_14:
    field->text = sPakMessageText_ProblemOccurred_B4FD4;
    goto done;
case_15:
    field->text = sPakMessageText_ControllerPakWas;
done:
    return 0;
}

s32 PakMessage_UpdateLine2(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = sPakMessageText_DeleteANoteToFree;
    goto done;
case_1:
    field->text = sPakMessageText_Found;
    goto done;
case_2:
    field->text = sPakMessageText_PakCompatiblePlease_B4C60;
    goto done;
case_3:
    field->text = sPakMessageText_PakCompatiblePlease_B4D34;
    goto done;
case_4:
    field->text = sPakMessageText_PakCompatiblePlease_B4CCC;
    goto done;
case_5:
    field->text = sPakMessageText_CompatiblePlease_B4D9C;
    goto done;
case_6:
    field->text = sPakMessageText_FoundYouWillNot;
    goto done;
case_7:
    field->text = sPakMessageText_WasChanged;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E74;
    goto done;
case_9:
    field->text = sPakMessageText_CompatiblePlease_B4EA8;
    goto done;
case_10:
    field->text = sPakMessageText_Successful;
    goto done;
case_11:
    field->text = sPakMessageText_AndCannotBeUsed;
    goto done;
case_12:
    field->text = sPakMessageText_Successfully;
    goto done;
case_13:
    field->text = sPakMessageText_WhileTryingTo_B4FAC;
    goto done;
case_14:
    field->text = sPakMessageText_WhileTryingTo_B4FE8;
    goto done;
case_15:
    field->text = sPakMessageText_SuccessfullyRepaired;
done:
    return 0;
}

s32 PakMessage_UpdateLine3(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = sPakMessageText_UpSpace;
    goto done;
case_1:
    field->text = sPakMessageText_PleaseInsertA;
    goto done;
case_2:
    field->text = sPakMessageText_InsertYourController_B4C78;
    goto done;
case_3:
    field->text = sPakMessageText_InsertYourController_B4D4C;
    goto done;
case_4:
    field->text = sPakMessageText_InsertYourController_B4CE4;
    goto done;
case_5:
    field->text = sPakMessageText_InsertYourRumblePak_B4DB0;
    goto done;
case_6:
    field->text = sPakMessageText_BeAbleToSaveYour;
    goto done;
case_7:
    field->text = sPakMessageText_PleaseTry;
    goto done;
case_8:
    field->text = sPakMessageText_NoController;
    goto done;
case_9:
    field->text = sPakMessageText_InsertYourRumblePak_B4EBC;
    goto done;
case_10:
    field->text = sPakMessageText_Empty_B4F10;
    goto done;
case_11:
    field->text = sPakMessageText_YouMayDeleteThis;
    goto done;
case_12:
    field->text = sPakMessageText_Deleted;
    goto done;
case_13:
    field->text = sPakMessageText_SaveANote;
    goto done;
case_14:
    field->text = sPakMessageText_DeleteANote;
    goto done;
case_15:
    field->text = sPakMessageText_PleaseRememberThat;
done:
    return 0;
}

s32 PakMessage_UpdateLine4(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = sPakMessageText_Empty_B4BF4;
    goto done;
case_1:
    field->text = sPakMessageText_ControllerPakAnd;
    goto done;
case_2:
    field->text = sPakMessageText_PakNowIfYouWishTo_B4C90;
    goto done;
case_3:
    field->text = sPakMessageText_PakNowIfYouWishTo_B4D64;
    goto done;
case_4:
    field->text = sPakMessageText_PakNowIfYouWishTo_B4CFC;
    goto done;
case_5:
    field->text = sPakMessageText_NowIfYouWishToPlay_B4DC8;
    goto done;
case_6:
    field->text = sPakMessageText_Game;
    goto done;
case_7:
    field->text = sPakMessageText_Again;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E88;
    goto done;
case_9:
    field->text = sPakMessageText_NowIfYouWishToPlay_B4ED4;
    goto done;
case_10:
    field->text = sPakMessageText_Empty_B4F14;
    goto done;
case_11:
    field->text = sPakMessageText_Note;
    goto done;
case_12:
    field->text = sPakMessageText_Empty_B4F90;
    goto done;
case_13:
    field->text = sPakMessageText_Empty_B4FCC;
    goto done;
case_14:
    field->text = sPakMessageText_Empty_B5008;
    goto done;
case_15:
    field->text = sPakMessageText_SavedNotesMayHave;
done:
    return 0;
}

s32 PakMessage_UpdateLine5(struct FieldRecAt14 *field) {
    u32 state = gPakMessageKind;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = sPakMessageText_Empty_B4BF8;
    goto done;
case_1:
    field->text = sPakMessageText_TryAgain;
    goto done;
case_2:
    field->text = sPakMessageText_ManageIt;
    goto done;
case_3:
    field->text = sPakMessageText_Save;
    goto done;
case_4:
    field->text = sPakMessageText_Load;
    goto done;
case_5:
    field->text = sPakMessageText_WithIt_B4DE0;
    goto done;
case_6:
    field->text = sPakMessageText_Empty_B4E34;
    goto done;
case_7:
    field->text = sPakMessageText_Empty_B4E6C;
    goto done;
case_8:
    field->text = sPakMessageText_Empty_B4E8C;
    goto done;
case_9:
    field->text = sPakMessageText_WithIt_B4EEC;
    goto done;
case_10:
    field->text = sPakMessageText_Empty_B4F18;
    goto done;
case_11:
    field->text = sPakMessageText_Empty_B4F64;
    goto done;
case_12:
    field->text = sPakMessageText_Empty_B4F94;
    goto done;
case_13:
    field->text = sPakMessageText_Empty_B4FD0;
    goto done;
case_14:
    field->text = sPakMessageText_Empty_B500C;
    goto done;
case_15:
    field->text = sPakMessageText_BeenDeleted;
done:
    return 0;
}

s32 PakMessage_UpdateOk(MenuItem *item) {
    /* Rage Wars sends 5, 6, 9, 10, 12 and 15 to the second text; Turok 2's
     * jump table (jtbl_800C1630, 0-15) sends only 8 there. The other case
     * values are not in the table; 0 and 15 bound it. */
    switch (gPakMessageKind) {
    default:
    case 0:
    case 5:
    case 6:
    case 9:
    case 10:
    case 12:
    case 15:
        item->text = sPakMessageText_Ok;
        break;
    case 8:
        item->text = sPakMessageText_Empty_B4BB4;
        break;
    }
    return 0;
}

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);

s32 SavePortalMenu_IsCinemaPlaying(void) {
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0) {
        return 1;
    }
    return 0;
}

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

/* The pak selection (as in PakMenu_InitSaveGame): a two-word struct, so GCC keeps it in a stack slot; the store of
 * the port and the frame's extra 8 bytes come from it, while the call reads the register. */

void SavePortalMenu_Init(Menu *event) {
    ControllerProfile *controller = event->controller;
    PakChannel sel;

    gPakMenuAnyPak = 0;
    gDamagedPakContinued = 0;
    sSavePortalDone = 0;
    gSavePortalActive = 1;
    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 0;
    sSavePortalChosen = 0;
    sel.channel = controller->port;
    Controller_WatchPak(sel.channel);
    Pak_ProbeSlot(GetPort(event));
}

/* Takes the port by address: the ROM keeps it in a stack slot (as PakMenu_GetReturnMenu does with its kind). */
static __inline__ void SelectPort(s32 *port) {
    gDamagedPakContinued = 0;
    Controller_WatchPak(*port);
}

void SavePortalMenu_InitB(Menu *event) {
    s32 port;

    sSavePortalDone = 0;
    gSavePortalActive = 1;
    gPakMenuIsSave = 1;
    gPakMenuIsProfile = 0;
    sSavePortalChosen = 0;
    if (gPakMenuAnyPak != 0) {
        port = gPakMenuPort;
    } else {
        port = event->controller->port;
    }
    SelectPort(&port);
}

s32 SavePortalMenu_Poll(Menu *event) {
    if (sSavePortalChosen == 0 && sSavePortalDone == 0) {
        if (GetPort(event) != -1 && Controller_PakChanged(GetPort(event)) != 0) {
            if (PakMenu_CheckPak((struct Menu *) event, GetPort(event), 0) != 0) {
                sSavePortalDone = 1;
            }
        }
    }
    return sSavePortalDone;
}

/* Only the field read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 unk10;
} CallbackArg;

void SavePortalMenu_Close(s32 arg0, CallbackArg *arg1) {
    if ((sSavePortalDone == 0) && (gSavePortalActive != 0) && (arg1->unk10 < 2U)) {
        sSavePortalDone = 1;
        gCameraPoolHideViews = 1;
    }
}

static __inline__ s32 func_00422EE0_inline(void) {
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0) {
        return 1;
    }
    return 0;
}

s32 SavePortalMenu_SelectSave(s32 unused, Menu *event) {
    sptr player = event->player;

    if (func_00422EE0_inline()) {
        return 0;
    }
    if (player != 0) {
        sSavePortalChosen = 1;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakSaveGameMenuDesc, event->player, event->controller, 0);
        return 1;
    }
    return 0;
}

s32 CPlayer__CanGetRefill(struct CPlayer *player, s32 kind, s32 arg); /* code/CPlayer's definition */

s32 SavePortalMenu_UpdateHealth(s32 unused, Menu *event) {
    u8 *player = event->player;

    if (CPlayer__CanGetRefill((struct CPlayer *) player, 1, ((CPlayer *)player)->m_Data.m_CheckPointWarpID) != 0) {
        event->items[1].flags |= 0x01000000;
    } else {
        event->items[1].flags &= ~0x01000000;
    }
    return 0;
}

s32 CPlayer__RefillFrom(u8 *player, s32 arg1, s32 arg2);
void Cinema_RequestById(s32 sound);

s32 SavePortalMenu_SelectHealth(s32 unused, Menu *event) {
    u8 *player = event->player;
    s32 ok;

    if (func_00422EE0_inline()) {
        return 0;
    }
    ok = CPlayer__RefillFrom(player, 1, ((CPlayer *)player)->m_Data.m_CheckPointWarpID);
    event->cursor = 0;
    if (ok) {
        Cinema_StopAtEnd();
        Cinema_RequestById(0x192);
    }
    return 0;
}

s32 SavePortalMenu_UpdateAmmo(s32 arg0, Menu *arg1) {
    CPlayer *temp_a0;
    MenuItem *temp_a0_2;
    MenuItem *temp_v0;

    temp_a0 = arg1->player;
    if (CPlayer__CanGetRefill((struct CPlayer *) temp_a0, 0, temp_a0->m_Data.m_CheckPointWarpID) != 0) {
        temp_v0 = arg1->items;
        temp_v0[2].flags |= 0x01000000;
    } else {
        temp_a0_2 = arg1->items;
        temp_a0_2[2].flags &= 0xFEFFFFFF;
    }
    return 0;
}

s32 SavePortalMenu_SelectAmmo(s32 unused, Menu *event) {
    u8 *player = event->player;
    s32 ok;

    if (func_00422EE0_inline()) {
        return 0;
    }
    ok = CPlayer__RefillFrom(player, 0, ((CPlayer *)player)->m_Data.m_CheckPointWarpID);
    event->cursor = 0;
    if (ok) {
        Cinema_StopAtEnd();
        Cinema_RequestById(0x193);
    }
    return 0;
}

static __inline__ void ShowPlayerMessage(u8 *player, char *fmt) {
    MenuPool_OpenMenu((char *)((CPlayer *)player)->m_pCamera->m_MenuPool, fmt, player, (sptr)((CPlayer *)player)->m_ContState.m_pContState, 0);
}

s32 SavePortalMenu_SelectGoBack(s32 unused, Menu *event) {
    u8 *player;
    s32 port;
    s32 *p = &port; /* the ROM stores the port through its address and never reads it */

    sSavePortalChosen = 1;
    gEngineApp_m_Game_m_PlayerList_m_bCinema[0] = 0;
    gEngineApp_m_Game_m_PlayerList_m_bCinema[1] = 0;
    gCameraPoolHideViews = 0;
    player = NULL;
    if (gPakMenuAnyPak != 0) {
        *p = gPakMenuPort;
    } else {
        player = event->player;
        *p = event->controller->port;
    }
    ShowPlayerMessage(player, (char *)&gPakMsgInsertRumblePakDesc);
    gSavePortalActive = 0;
    return 1;
}

s32 DamagedPakMenu_Poll(struct Menu *record) {
    if (sDamagedPakDone == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (Controller_PakChanged(func_80409BDC_de(record)) != 0) {
                if (PakMenu_CheckPak((struct Menu *) record, func_80409BDC_de(record), 0) != 0) {
                    sDamagedPakDone = 1;
                }
            }
        }
    }
    return sDamagedPakDone;
}

void DamagedPakMenu_InitInGame(void) {
    sDamagedPakDone = 0;
    sDamagedPakInGame = 1;
}

void DamagedPakMenu_InitAtBoot(void) {
    sDamagedPakDone = 0;
    sDamagedPakInGame = 0;
}

s32 DamagedPakMenu_UpdateLine1(struct FieldRecAt14 *field) {
    if (sDamagedPakInGame != 0) {
        field->text = sDamagedPakMenuText_ControllerPakIs;
    } else {
        field->text = sDamagedPakMenuText_DamagedController;
    }
    return 0;
}

s32 DamagedPakMenu_UpdateLine2(struct FieldRecAt14 *field) {
    if (sDamagedPakInGame != 0) {
        field->text = sDamagedPakMenuText_DamagedGameCannot;
    } else {
        field->text = sDamagedPakMenuText_PakFoundIn;
    }
    return 0;
}

s32 DamagedPakMenu_UpdateLine3(MenuItem *item, Menu *event) {
    s32 port = GetPort(event);

    if (sDamagedPakInGame != 0) {
        item->text = sDamagedPakMenuText_BeSaved;
    } else {
        item->text = sDamagedPakMenuText_ControllerX;
        sprintf(item->text + (MenuItem_TextLength(item) - 2), PAKMENU_FMT_1D_DOT, port + 1);
    }
    return 0;
}

s32 DamagedPakMenu_SelectRepair(s32 unused, Menu *event) {
    if (PakMenu_CheckPak((struct Menu *) event, GetPort(event), 1) != 0) {
        gPakMenuLeaving = 1;
        if (event->context == (char *)&gSavePortalMenuDescB || event->context == (char *)&gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakConfirmRepairDesc, event->player, (sptr) event->controller, event->context);
        return 1;
    }
}

s32 DamagedPakMenu_SelectRetry(s32 unused, Menu *event) {
    s32 port = GetPort(event);
    char *dst;

    gPakMenuLeaving = 1;
    if (PakMenu_CheckPak((struct Menu *) event, port, 1) != 0) {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera->m_MenuPool;
        } else {
            dst = &gEngineApp_m_Game_m_MenuPool;
        }
        if (event->context == &gSavePortalMenuDescB || event->context == &gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(dst, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    } else {
        if (event->context == &gSavePortalMenuDescB || event->context == &gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    }
}

s32 DamagedPakMenu_SelectContinue(s32 unused, Menu *event) {
    char *dst;

    gDamagedPakContinued = 1;
    gCameraPoolHideViews = 0;
    if (gPakMenuAnyPak != 0) {
        gPakCheckDone = 1;
    } else {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera->m_MenuPool;
        } else {
            dst = &gEngineApp_m_Game_m_MenuPool;
        }
        if (event->text == (char *)&gSavePortalMenuDescB || event->text == (char *)&gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(dst, event->text, event->player, event->controller, 0);
    }
    return 1;
}

s32 DamagedPakMenu_UpdateContinue(struct FieldRecAt14 *field) {
    if (gPakMenuIsLoad != 0) {
        field->text = sDamagedPakMenuText_Continue;
    } else {
        field->text = sDamagedPakMenuText_ContinueWithoutSaving;
    }
    return 0;
}

s32 PakConfirmMenu_Poll(Menu *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (sPakConfirmDone == 0) {
        if (gPakMenuAnyPak != 0) {
            var_v1 = gPakMenuPort;
        } else {
            var_v1 = (s32) arg0->controller->port;
        }
        if (var_v1 != -1) {
            if (gPakMenuAnyPak != 0) {
                var_a0 = gPakMenuPort;
            } else {
                var_a0 = (s32) arg0->controller->port;
            }
            if (Controller_PakChanged(var_a0) != 0) {
                if (gPakMenuAnyPak != 0) {
                    var_a1 = gPakMenuPort;
                } else {
                    var_a1 = (s32) arg0->controller->port;
                }
                if (PakMenu_CheckPak((struct Menu *) arg0, var_a1, 0) != 0) {
                    sPakConfirmDone = 1;
                }
            }
        }
    }
    return sPakConfirmDone;
}

void PakConfirmMenu_InitRepair(void) {
    gPakConfirmKind = 0;
    sPakConfirmDone = 0;
}
void PakConfirmMenu_InitNoSpace(void);

void PakConfirmMenu_InitNoSpace(void) {
    gPakConfirmKind = 1;
    sPakConfirmDone = 0;
}

void PakConfirmMenu_InitNoSpaceAll(void) {
    gPakConfirmKind = 2;
    sPakConfirmDone = 0;
}

void PakConfirmMenu_InitOverwrite(void) {
    gPakConfirmKind = 3;
    sPakConfirmDone = 0;
}

void PakConfirmMenu_InitCorruptNote(void) {
    gPakConfirmKind = 4;
    sPakConfirmDone = 0;
}

s32 PakConfirmMenu_UpdateLine1(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_1;
        case 2: goto state_2;
        case 3: goto state_3;
        case 4: goto state_4;
        }
state_other:
    field->text = sPakConfirmMenuText_AttemptRepair;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_Error_B5194;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_Error_B51D0;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_Overwrite;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_Error_B527C;
done:
    return 0;
}

s32 PakConfirmMenu_UpdateLine2(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_1;
        case 2: goto state_2;
        case 3: goto state_3;
        case 4: goto state_4;
        }
state_other:
    field->text = sPakConfirmMenuText_PreviouslySaved;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_NotEnoughNotesOr_B519C;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_NotEnoughNotesOr_B51D8;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_YouWillBeDeleting;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_ThisNoteIsCorrupt;
done:
    return 0;
}

s32 PakConfirmMenu_UpdateLine3(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_1;
        case 2: goto state_2;
        case 3: goto state_3;
        case 4: goto state_4;
        }
state_other:
    field->text = sPakConfirmMenuText_DataMayBeLost;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_PagesAvailableTo_B51B0;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_PagesAvailableTo_B51EC;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_TheOldNote;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_AndCannotBeUsed;
done:
    return 0;
}

s32 PakConfirmMenu_UpdateLine4(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_1;
        case 2: goto state_2;
        case 3: goto state_3;
        case 4: goto state_4;
        }
state_other:
    field->text = sPakConfirmMenuText_WouldYouStillLike;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_Save;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_SaveAllNoteTypes;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_WouldYouLikeTo_B5250;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_WouldYouLikeTo_B52B0;
done:
    return 0;
}

s32 PakConfirmMenu_UpdateLine5(MenuItem *item) {
    switch (gPakConfirmKind) {
        case 0:
        default:
            item->text = sPakConfirmMenuText_ToAttemptRepair;
            break;
        case 1:
            item->text = sPakConfirmMenuText_Empty_B51CC;
            break;
        case 2:
            item->text = sPakConfirmMenuText_Empty_B5218;
            break;
        case 3:
            item->text = sPakConfirmMenuText_OverwriteThisNote;
            break;
        case 4:
            item->text = sPakConfirmMenuText_DeleteThisNote;
            break;
    }
    return 0;
}

s32 PakConfirmMenu_UpdateYes(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_3;
        case 2: goto case_3;
        case 3: goto case_1;
        case 4: goto case_2;
        }
state_other:
    field->text = sPakConfirmMenuText_Yes;
    goto done;
case_1:
    field->text = sPakConfirmMenuText_Yes;
    goto done;
case_2:
    field->text = sPakConfirmMenuText_Yes;
    goto done;
case_3:
    field->text = sPakConfirmMenuText_ManageTheControllerPak;
done:
    return 0;
}

s32 PakConfirmMenu_UpdateNo(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_3;
        case 2: goto case_3;
        case 3: goto case_1;
        case 4: goto case_2;
        }
state_other:
    field->text = sPakConfirmMenuText_No;
    goto done;
case_1:
    field->text = sPakConfirmMenuText_No;
    goto done;
case_2:
    field->text = sPakConfirmMenuText_No;
    goto done;
case_3:
    field->text = sPakConfirmMenuText_ContinueWithoutSaving;
done:
    return 0;
}

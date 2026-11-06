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

extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern s32 D_800F70A8;
extern s32 D_80130984;
extern s32 D_800C2010;
extern s32 D_800C2014;
extern char D_800B45CC[];
extern char D_800B45D8[];
extern u8 D_80119E2C[];
extern u8 D_0043C0E4[];
extern u8 D_0043BC88[];
extern u8 D_0043BD18[];
extern u8 D_0043BE14[];
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
    if (D_800C2010 != 0 || D_800C2014 != 0) {
        CCache__Flush(0);
    }
    if (D_800C2010 != 0) {
        CCache__Deallocate(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        CCache__Deallocate(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
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

    if (D_80130990 != 0) {
        pv = D_800C2028;
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
    if (D_80130984 != 0) {
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
                        r = Pak_SaveNoteVerified_2(padv, size, D_800C2018, D_800B45CC, ext, D_800B45D8);
                    }
                }
            }
            if (ok != 0) {
                MenuPool_OpenMenu_2(D_80119E2C, D_0043C0E4, plr, plr->m_ContState.m_pContState, own->desc);
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
        MenuPool_OpenMenu_2(D_80119E2C, D_0043BC88, plr, plr->m_ContState.m_pContState, (s32)D_0043BE14);
    } else {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043BD18, plr, plr->m_ContState.m_pContState, (s32)D_0043BE14);
    }
    return 1;
}

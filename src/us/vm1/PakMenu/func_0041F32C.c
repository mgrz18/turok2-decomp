/* FAKEMATCH (round 21 audit): 2 empty `__asm__` barriers; without them: PakMenu_SelectNote 60 of 521 words.
 * */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407748.c, func_00407748.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "structs/cache.h"
#include "structs/pak.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "structs/save.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "globals/scene.h"
#include "protos/pak.h"

extern void *memset(void *dst, s32 value, uptr size);
s32 PakMenu_CheckPak();
void PakMenu_UpdateNoteRows();
extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */
extern void CCache__Deallocate(sptr, void *);
extern void **CCache__Allocate(s32, s32, s32, char *);
extern void *memset(void *dst, s32 value, uptr size);
/* Game code / company code check on the controller-pak note (inlined). */
static __inline__ s32 k7_chk(u8 *gc, u8 *cc) {
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
    v1 = k7_chk(sp30, sp28);
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

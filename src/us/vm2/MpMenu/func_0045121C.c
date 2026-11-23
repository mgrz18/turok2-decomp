#include "globals/menu_desc.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E65C.c, func_0043E65C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 Controller_ConfirmPressed(void *p);
extern s32 Menu_StepValueLR_2(Menu *a, s32 cur, s32 step, s32 lo, s32 hi, s32 wrap);
extern void CPlayer__SetCharacter_2(CPlayer *player, s32 v);
extern void MenuItem_SetModel_2(void *a, s32 b, s32 c);
extern void Sound__Play2D(s32 a);
extern void MenuPool_OpenChildMenu_2(sptr a, sptr b, void *c);
extern s16 D_800C1E7A[][2];     /* 2 bytes into the module copy of gCharacterSelectSounds: [i][0] is the sound */
s32 CharacterMenu_SelectCharacter_2(void *x, Menu *m, void *y) {
    CPlayer *player = m->player;
    CPlayerOptions *options = player->m_pOptions;
    if (Controller_ConfirmPressed(m->controller)) {
        CPlayer__SetCharacter_2(player, options->m_nCharacter);
        Sound__Play2D(D_800C1E7A[player->m_Data.m_nDeathmatchCharacter][0]);
        MenuPool_OpenChildMenu_2((sptr)y, (sptr)m, &gPlayerSetupMenuDesc_2);
        return 1;
    } else {
        s8 v = Menu_StepValueLR_2(m, options->m_nCharacter, 1, 0, 0xA, 1);
        options->m_nCharacter = v;
        CPlayer__SetCharacter_2(player, v);
        MenuItem_SetModel_2(x, 0xB, options->m_nCharacter);
        return 0;
    }
}

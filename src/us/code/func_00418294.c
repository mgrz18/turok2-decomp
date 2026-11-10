/* WRONG CUT: real function 0x00418294, size 0xEC (through 0x00418380): splat cut it in two
 * (CharacterMenu_SelectCharacter, func_004182E8) in the middle of a call's argument setup. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu_desc.h"
#include "globals/menu.h"
#include "protos/menu.h"

extern s32 Controller_ConfirmPressed(void *controller);
extern s32 Menu_StepValueLR(Menu *c, s32 value, s32 step, s32 min, s32 max, s32 wrap);
extern void CPlayer__SetCharacter(CPlayer *player, s32 team);
extern void MenuItem_SetModel(sptr menu, s32 id, s32 arg);
extern void Sound__Play2D(s32 sound);

/* Team selection item: left/right cycles the team, A confirms. */
s32 CharacterMenu_SelectCharacter(s32 menu, Menu *c, s32 pool) {
    CPlayer *player = c->player;
    CPlayerOptions *options = player->m_pOptions;

    if (Controller_ConfirmPressed(c->controller)) {
        CPlayer__SetCharacter(player, options->m_nCharacter);
        Sound__Play2D(gCharacterSelectSounds[player->m_Data.m_nDeathmatchCharacter].sound);
        MenuPool_OpenChildMenu(pool, c, &gPlayerSetupMenuDesc);
        return 1;
    } else {
        s8 team = Menu_StepValueLR(c, options->m_nCharacter, 1, 0, 10, 1);

        options->m_nCharacter = team;
        CPlayer__SetCharacter(player, team);
        MenuItem_SetModel(menu, 0xB, options->m_nCharacter);
        return 0;
    }
}

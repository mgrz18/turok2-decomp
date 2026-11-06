/* WRONG CUT: real function 0x00420454, size 0x264 (through 0x004206B8): splat cut it into
 * GameNoteMenu_SelectYes, func_004205B8 and func_004205C4 inside its straight-line code. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "structs/save.h"

/* Load game menu "select" handler: with the pak checked, either restores the
 * loaded save (player block, game progress, level times, the level's key
 * bytes) and opens the "game loaded" dialog, or opens the "no save"/"bad
 * save" dialog for the pak note. */

extern s32 D_80130990;
extern s32 D_801309B8;
extern s32 D_801309BC;
extern s32 D_801309C4;
extern s32 D_800C2028;
extern u32 D_80110060;
extern u32 D_8011AADC[];
extern u8 D_8011AC58[];
extern f32 D_8011ACB4[];
extern f32 D_8011ACB8[];
extern f32 D_8011ACBC[];
extern s32 D_801198A4[2];
extern CScene gScene;
extern u8 D_800EBAC0[];
extern u8 D_0043BD20[];
extern u8 D_0043BDB0[];
extern u8 D_0043BE1C[];

extern s32 PakMenu_CheckPak(Menu *c, s32 slot, s32 quiet);
extern CPlayer *CPlayerPool__FindByController(void *players, ControllerProfile *ctl);
extern s32 Pak_DeleteNote(s32 slot, s32 note);
extern void MenuPool_OpenMenu(void *menus, void *tmpl, s32 a, void *b, void *parent);

s32 GameNoteMenu_SelectYes(void *menu, Menu *c) {
    s32 slot;
    CPlayer *player;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;
    CScene *world;

    if (D_80130990) {
        slot = D_800C2028;
    } else {
        slot = c->controller->port;
    }
    if (PakMenu_CheckPak(c, slot, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309B8 != 0) {
        player = CPlayerPool__FindByController(&D_80119870.m_PlayerList, c->controller);
        world = &gScene;
        keep = D_8011AC58[0];
        player->m_Data = D_800C201C->player;
        *(GameSettingsBlock *)&D_8011AAD8 = D_800C201C->settings;
        D_8011ACB0.frames = D_800C201C->frames;
        D_8011ACB4[0] = D_800C201C->seconds;
        D_8011ACB8[0] = D_800C201C->minutes;
        D_80110060 = D_8011AADC[0];
        D_8011ACBC[0] = D_800C201C->hours;
        src = D_800C201C->levelFlags;
        dst = D_800EBAC0;
        D_8011AC58[0] = keep;
        for (i = 0; i < world->unk_30; i++) {
            *dst++ = *src++;
        }
        D_801198A4[0] = 0;
        D_801198A4[1] = 0;
        MenuPool_OpenMenu((u8 *) D_801198A4 + 0x588, D_0043BD20, (s32) player, player->m_ContState.m_pContState, NULL);
        return 1;
    }
    if (Pak_DeleteNote(slot, D_801309C4) == 0) {
        MenuPool_OpenMenu(D_80119870.m_MenuPool, D_0043BDB0, c->player, c->controller, c->context);
    } else {
        MenuPool_OpenMenu(D_80119870.m_MenuPool, D_0043BE1C, c->player, c->controller, c->context);
    }
    return 1;
}

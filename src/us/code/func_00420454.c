/* WRONG CUT: real function 0x00420454, size 0x264 (through 0x004206B8): splat cut it into
 * func_00420454, func_004205B8 and func_004205C4 inside its straight-line code. */
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
extern CWorld D_800F7078;
extern u8 D_800EBAC0[];
extern u8 D_0043BD20[];
extern u8 D_0043BDB0[];
extern u8 D_0043BE1C[];

extern s32 func_0041DD90(Menu *c, s32 slot, s32 quiet);
extern CPlayer *func_0025342C(void *players, ControllerProfile *ctl);
extern s32 func_00426064(s32 slot, s32 note);
extern void func_00416644(void *menus, void *tmpl, s32 a, void *b, void *parent);

s32 func_00420454(void *menu, Menu *c) {
    s32 slot;
    CPlayer *player;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;
    CWorld *world;

    if (D_80130990) {
        slot = D_800C2028;
    } else {
        slot = c->controller->port;
    }
    if (func_0041DD90(c, slot, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309B8 != 0) {
        player = func_0025342C(&D_80119870.m_PlayerList, c->controller);
        world = &D_800F7078;
        keep = D_8011AC58[0];
        player->m_Data = D_800C201C->player;
        *(SaveSettings *)&D_8011AAD8 = D_800C201C->settings;
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
        func_00416644((u8 *) D_801198A4 + 0x588, D_0043BD20, (s32) player, player->m_ContState.m_pContState, NULL);
        return 1;
    }
    if (func_00426064(slot, D_801309C4) == 0) {
        func_00416644(D_80119870.m_MenuPool, D_0043BDB0, c->player, c->controller, c->context);
    } else {
        func_00416644(D_80119870.m_MenuPool, D_0043BE1C, c->player, c->controller, c->context);
    }
    return 1;
}

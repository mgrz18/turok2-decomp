/* WRONG CUT: real function 0x00420454, size 0x264 (through 0x004206B8): splat cut it into
 * func_00420454, func_004205B8 and func_004205C4 inside its straight-line code. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/menu.h"

/* Load game menu "select" handler: with the pak checked, either restores the
 * loaded save (player block, game progress, level times, the level's key
 * bytes) and opens the "game loaded" dialog, or opens the "no save"/"bad
 * save" dialog for the pak note. */

typedef struct {
    s32 words[0x4E8 / 4];
} PlayerSave;

typedef struct {
    s32 words[0x184 / 4];
} ProgressSave;

typedef struct {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ f32 time0;
    /* 0x00C */ f32 time1;
    /* 0x010 */ f32 time2;
    /* 0x014 */ f32 time3;
    /* 0x018 */ PlayerSave player;
    /* 0x500 */ ProgressSave progress;
    /* 0x684 */ u8 levelKeys[1];
} SaveGame;

typedef struct {
    /* 0x000 */ u8 pad0[0x520];
    /* 0x520 */ PlayerSave save;
    /* 0xA08 */ u8 padA08[0xA50 - 0xA08];
    /* 0xA50 */ void *controller;
} Player;


extern s32 D_80130990;
extern s32 D_801309B8;
extern s32 D_801309BC;
extern s32 D_801309C4;
extern s32 D_800C2028;
extern SaveGame *D_800C201C;
extern u32 D_80110060;
extern u8 D_80119870[];
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
extern Player *func_0025342C(void *players, ControllerProfile *ctl);
extern s32 func_00426064(s32 slot, s32 note);
extern void func_00416644(void *menus, void *tmpl, s32 a, void *b, void *parent);

s32 func_00420454(void *menu, Menu *c) {
    s32 slot;
    Player *player;
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
        player = func_0025342C(D_80119870, c->controller);
        world = &D_800F7078;
        keep = D_8011AC58[0];
        player->save = D_800C201C->player;
        *(ProgressSave *)&D_8011AAD8 = D_800C201C->progress;
        D_8011ACB0.frames = D_800C201C->time0;
        D_8011ACB4[0] = D_800C201C->time1;
        D_8011ACB8[0] = D_800C201C->time2;
        D_80110060 = D_8011AADC[0];
        D_8011ACBC[0] = D_800C201C->time3;
        src = D_800C201C->levelKeys;
        dst = D_800EBAC0;
        D_8011AC58[0] = keep;
        for (i = 0; i < world->unk_30; i++) {
            *dst++ = *src++;
        }
        D_801198A4[0] = 0;
        D_801198A4[1] = 0;
        func_00416644((u8 *) D_801198A4 + 0x588, D_0043BD20, (s32) player, player->controller, NULL);
        return 1;
    }
    if (func_00426064(slot, D_801309C4) == 0) {
        func_00416644(D_80119870 + 0x5BC, D_0043BDB0, c->player, c->controller, c->context);
    } else {
        func_00416644(D_80119870 + 0x5BC, D_0043BE1C, c->player, c->controller, c->context);
    }
    return 1;
}

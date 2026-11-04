/* Second VM module copy of func_0042DE74 (same source, linked 0x0042DD94). */
#include "common.h"
#include "structs/game.h"

typedef struct Crosshair {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x90 - 0xC];
    /* 0x90 */ f32 unk90;
    /* 0x94 */ f32 unk94;
    /* 0x98 */ f32 unk98;
    /* 0x9C */ s32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ f32 unkAC;
    /* 0xB0 */ f32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ s32 unkB8;
    /* 0xBC */ u8 padBC[0xC0 - 0xBC];
} Crosshair;

/* An animated HUD item (func_0027ACC4 places it, func_0027AD00 restarts it). */
typedef struct HudItem {
    u8 pad[0x3C];
} HudItem;

typedef struct Quad {
    s32 v[4];
} Quad;

typedef struct Camera {
    /* 0x000 */ u8 pad0[0x12C];
    /* 0x12C */ Quad unk12C;
} Camera;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x10];
    /* 0x0010 */ s32 unk10;
    /* 0x0014 */ u8 pad14[0x51C - 0x14];
    /* 0x051C */ Camera *camera;
    /* 0x0520 */ u8 pad520[0x92C - 0x520];
    /* 0x092C */ s32 warp;
    /* 0x0930 */ u8 pad930[0x934 - 0x930];
    /* 0x0934 */ s16 ammo[0x29];
    /* 0x0986 */ u8 pad986;
    /* 0x0987 */ s8 unk987;
    /* 0x0988 */ u8 pad988[0xA30 - 0x988];
    /* 0x0A30 */ s32 unkA30;
    /* 0x0A34 */ s32 unkA34;
    /* 0x0A38 */ s32 unkA38;
    /* 0x0A3C */ s32 unkA3C;
    /* 0x0A40 */ u8 padA40[0xA50 - 0xA40];
    /* 0x0A50 */ s32 controller;
    /* 0x0A54 */ u8 padA54[0xB04 - 0xA54];
    /* 0x0B04 */ Quad unkB04;
    /* 0x0B14 */ u8 padB14[0xB60 - 0xB14];
    /* 0x0B60 */ s32 unkB60;
    /* 0x0B64 */ u8 padB64[0xC10 - 0xB64];
    /* 0x0C10 */ Crosshair crosshair;
    /* 0x0CD0 */ u8 padCD0[0x1064 - 0xCD0];
    /* 0x1064 */ HudItem ammoItem;
    /* 0x10A0 */ HudItem healthItem;
    /* 0x10DC */ HudItem keysItem;
    /* 0x1118 */ HudItem livesItem;
    /* 0x1154 */ u8 pad1154[0x1190 - 0x1154];
    /* 0x1190 */ HudItem unk1190;
    /* 0x11CC */ HudItem unk11CC;
    /* 0x1208 */ HudItem scoreItem;
    /* 0x1244 */ HudItem fragsItem;
    /* 0x1280 */ s32 unk1280;
    /* 0x1284 */ u8 pad1284[0x1290 - 0x1284];
    /* 0x1290 */ s32 unk1290;
} Player;

extern s32 D_8011001C;
extern s32 D_80110048;
extern s32 D_800C2034;          /* screen height */
extern u8 D_800ADB60[];
extern u8 D_800ADB74[];
extern u8 D_800ADBB0[];
extern u8 D_800ADBF0[];
extern u8 D_800ADC30[];
extern u8 D_800ADC70[];
/* In the engine's .rodata, the pool shared with func_00248720. */
extern f32 D_800A6AA0;          /* 437.5f */
extern f32 D_800A6AA4;          /* -675.0f */
extern f32 D_800A6AA8;          /* 0.25f */
extern f32 D_800A6AAC;          /* 0.1875f */
extern f32 D_800A6AB0;          /* 1.5707964f */

extern void func_00254044(void);
extern void func_002541E4(Player *p);
extern void func_00285BD0(s32 controller, s32 state);
extern void func_0027ACC4(HudItem *item, s32 x, s32 y);
extern void func_0027AD00(HudItem *item, u8 *anim);
extern void func_00248340(Player *p);
extern s32 func_00284174(void);
extern s16 *func_0025398C(Player *p, s32);
extern void func_002842C8(void);
extern void func_002842B4(void);
extern void func_0026E19C(s32, s32, Player *p);

void func_00466D94(Player *p) {
    GameSettings *flags = &D_8011AAD8;
    Crosshair *c;
    s32 warp;

    if (flags->multiplayer) {
        func_00254044();
        func_002541E4(p);
    }
    c = &p->crosshair;
    c->unkB4 = 0;
    c->unk8 = 0;
    c->unkB8 = 0;
    c->unk4 = -1;
    c->unk0 = -1;
    c->unkA8 = 0;
    c->unk9C = 0;
    c->unkA4 = 0;
    c->unkAC = D_800A6AA0;
    c->unkB0 = D_800A6AA4;
    c->unk90 = D_800A6AA8;
    c->unk94 = D_800A6AAC;
    c->unk98 = D_800A6AA8;
    c->unkA0 = D_800A6AB0;
    p->unk1280 = 0;
    if (!flags->multiplayer) {
        func_00285BD0(p->controller, 1);
    }
    p->unkA30 = 0;
    p->unkA34 = 0;
    p->unkA38 = 0;
    p->unkA3C = 0;
    func_0027ACC4(&p->ammoItem, 120, 96);
    func_0027AD00(&p->ammoItem, D_800ADB74);
    func_0027ACC4(&p->healthItem, 48, 96);
    func_0027AD00(&p->healthItem, D_800ADB60);
    func_0027ACC4(&p->keysItem, -64, 16);
    func_0027AD00(&p->keysItem, D_800ADBB0);
    func_0027ACC4(&p->livesItem, 40, 16);
    func_0027AD00(&p->livesItem, D_800ADBF0);
    func_0027ACC4(&p->scoreItem, 40, 42);
    func_0027AD00(&p->scoreItem, D_800ADC30);
    func_0027ACC4(&p->fragsItem, 40, 68);
    func_0027AD00(&p->fragsItem, D_800ADC70);
    func_0027ACC4(&p->unk1190, 24, (s16) (D_800C2034 + 96));
    warp = 0;
    func_0027ACC4(&p->unk11CC, 24, (s16) (D_800C2034 + 96));
    p->unk1290 = 0;
    p->unkB60 = 0;
    p->unkB04 = p->camera->unk12C;
    func_00248340(p);
    if (!func_00284174()) {
        warp = D_80110048 != 999;
    }
    if (D_8011001C) {
        warp = 1;
    }
    if (warp) {
        p->warp = D_80110048;
    }
    if (!flags->multiplayer && p->unk987) {
        /* the ROM adds the scaled index first: written index + array */
        *(*func_0025398C(p, 21) + p->ammo) = 1;
    }
    func_002842C8();
    if (p->unk10) {
        func_0026E19C(p->unk10, 1, p);
    }
    func_002842B4();
}

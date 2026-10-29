#include "common.h"
#include "gfx.h"

/* An animated HUD item (func_00277328 ticks it, func_0027AD00 restarts it). */
typedef struct {
    /* 0x00 */ s32 anim;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 visible;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 x;
    /* 0x14 */ f32 y;
    /* 0x18 */ u8 pad18[0x38 - 0x18];
    /* 0x38 */ s32 value;           /* the value shown, to restart on change */
} HudItem;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 score;
    /* 0x06 */ u16 frags;
    /* 0x08 */ u8 pad8[0x3E - 0x8];
    /* 0x3E */ u8 tagMode;
} Options;

typedef struct {
    /* 0x000 */ u8 pad0[0x380];
    /* 0x380 */ s32 underwater;
} Strokes;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4F8];
    /* 0x04F8 */ f32 charge;
    /* 0x04FC */ u8 pad4FC[0x518 - 0x4FC];
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 pad51C[0x524 - 0x51C];
    /* 0x0524 */ s32 health;        /* 8.8 */
    /* 0x0528 */ s16 lives;
    /* 0x052A */ s16 keys;
    /* 0x052C */ u8 pad52C[0x996 - 0x52C];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xC10 - 0x998];
    /* 0x0C10 */ u8 crosshair[0xCD0 - 0xC10];
    /* 0x0CD0 */ Strokes strokes;
    /* 0x1054 */ u8 pad1054[0x1064 - 0x1054];
    /* 0x1064 */ HudItem ammo;
    /* 0x10A0 */ HudItem healthItem;
    /* 0x10DC */ HudItem keysItem;
    /* 0x1118 */ HudItem livesItem;
    /* 0x1154 */ u8 pad1154[0x1208 - 0x1154];
    /* 0x1208 */ HudItem scoreItem;
    /* 0x1244 */ HudItem fragsItem;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x24];
    /* 0x024 */ s32 noCrosshair;
    /* 0x028 */ u8 pad28[0x288 - 0x28];
    /* 0x288 */ f32 width;
    /* 0x28C */ f32 height;
    /* 0x290 */ f32 x;
    /* 0x294 */ f32 y;
} View;

typedef struct {
    u8 multiplayer;
} GameMode;

extern s32 D_8011AC8C;          /* game state */
extern u8 D_8011AAEA;           /* HUD alpha */
extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern GameMode D_8011AAF1;
extern s32 D_8011ACE4;
extern u8 D_800ADB84[];
extern u8 D_800ADBC0[];
extern u8 D_800ADC00[];
extern u8 D_800ADC40[];
extern u8 D_800ADC80[];

void func_00277328(HudItem *item);
void func_0027AD00(HudItem *item, u8 *anim);
void func_0024A86C(Player *player, View *view);
void func_00247438(void *crosshair, View *view);
void func_00279028(s32 alpha);
void func_00254D5C(Strokes *strokes, View *view, Player *player);
void func_0024B3E4(Strokes *strokes, View *view, Player *player);
s32 func_0024E700(Player *player, s32 weapon);
s32 func_0024E7D0(Player *player);
void func_0027B320(s32 sprite, s32 frame, s16 x, s16 y, f32 sx, f32 sy, s32);
void func_00277CF0(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32, s32 rightAlign, s32 minDigits);
void func_00254768(Player *player, View *view);

/* The next display list command. Through this helper, as in the ROM, the
 * head is loaded and stored by symbol; a post-increment written in place
 * keeps the address of the head in a register instead. */
static __inline__ Gfx *dl_next(void) {
    return D_800D8E24++;
}

/* Draw a player's HUD: tick the item animations, the crosshair and strokes,
 * then the ammo (with the charge gauge of weapon 0xDD), the health, the
 * lives, the keys in single player, and the score and frags in multiplayer. */
void func_0024C648(Player *player, View *view) {
    f32 sx;
    f32 sy;
    f32 shift;
    f32 x;
    f32 y;
    s32 ammo;
    s32 icon;
    s32 frame;
    s32 lives;
    s32 showAmmo;
    s32 value;
    Strokes *strokes;

    showAmmo = 1;
    if (D_8011AC8C == 0xB || D_8011AC8C == 8) {
        return;
    }
    func_00277328(&player->ammo);
    func_00277328(&player->healthItem);
    func_00277328(&player->keysItem);
    func_00277328(&player->livesItem);
    func_00277328(&player->scoreItem);
    func_00277328(&player->fragsItem);
    func_0024A86C(player, view);
    if (view->noCrosshair == 0) {
        func_00247438(player->crosshair, view);
    }
    strokes = &player->strokes;
    func_00279028(0xFF);
    {   /* gDPSetTextureFilter(G_TF_BILERP) */
        Gfx *g = dl_next();

        g->w0 = 0xE3001201;
        g->w1 = 0x2000;
    }
    if (strokes->underwater) {
        func_00254D5C(strokes, view, player);
    } else {
        func_0024B3E4(strokes, view, player);
    }
    func_00279028(D_8011AAEA);
    sx = view->width / (f32)D_800C2030;
    shift = sx * 32.0f;
    sy = view->height / (f32)D_800C2034;

    ammo = func_0024E700(player, player->weapon);
    if (player->ammo.value != ammo) {
        func_0027AD00(&player->ammo, D_800ADB84);
        player->ammo.value = ammo;
    }
    icon = func_0024E7D0(player);
    if (player->ammo.anim && player->ammo.visible && icon != -1) {
        x = player->ammo.x * sx + view->x;
        y = player->ammo.y * sy + view->y + view->height;
        func_0027B320(0x12C, 0, x - shift + sx * 18.0f, y + sy, sx, sy, 1);
        frame = 0;
        if (icon == 0xDD) {
            frame = 8;
            if (!(player->charge <= 0.0f)) {
                frame = (90.0f - player->charge) * 0.08888889f;
            }
            showAmmo = 0;
        } else if (player->weapon == 0x15) {
            showAmmo = 0;
        }
        func_0027B320(icon, frame, x - shift, y - sy * 8.0f, sx, sy, 1);
        if (showAmmo) {
            func_00277CF0(player->ammo.value, x - sx * 6.0f, y - sy * 4.0f, sx, sy, 1, 0, 0);
        }
    }

    x = player->healthItem.x * sx + view->x;
    y = player->healthItem.y * sy + view->y + view->height;
    func_0027B320(0x12C, 0, x - shift + sx * 21.0f, y + sy, sx, sy, 1);
    func_0027B320(0x1F7, 0, x - shift + sx, y - sy * 9.0f, sx, sy, 1);
    func_00277CF0((player->health + 0xFF) >> 8, x - sx * 3.0f, y - sy * 4.0f, sx, sy, 1, 0, 0);

    lives = player->lives;
    if (player->livesItem.value != lives) {
        func_0027AD00(&player->livesItem, D_800ADC00);
        player->livesItem.value = lives;
    }
    if (player->livesItem.anim && player->livesItem.visible) {
        x = player->livesItem.x * sx + view->x + view->width;
        y = player->livesItem.y * sy + view->y;
        func_0027B320(0x12C, 1, x + sx * -42.0f, y + sy * 24.0f, sx, sy, 1);
        func_0027B320(0x1F5, 0, x, y + sy * 8.0f, sx, sy, 1);
        func_00277CF0(player->lives, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
    }

    if (!D_8011AAF1.multiplayer) {
        value = player->keys;
        if (player->keysItem.value != value) {
            func_0027AD00(&player->keysItem, D_800ADBC0);
            player->keysItem.value = value;
        }
        if (player->keysItem.anim && player->keysItem.visible) {
            x = player->keysItem.x * sx + view->x;
            y = player->keysItem.y * sy + view->y;
            func_0027B320(0x12C, 0, x + sx * 18.0f, y + sy * 28.0f, sx, sy, 1);
            func_0027B320(0x1F6, 0, x, y, sx, sy, 1);
            func_00277CF0(player->keys, x + sx * 30.0f, y + sy * 24.0f, sx, sy, 1, 0, 0);
        }
    }
    if (D_8011AAF1.multiplayer) {
        value = player->options->score;
        if (player->scoreItem.value != value) {
            func_0027AD00(&player->scoreItem, D_800ADC40);
            player->scoreItem.value = value;
        }
        if (player->scoreItem.anim && player->scoreItem.visible) {
            x = player->scoreItem.x * sx + view->x + view->width;
            y = player->scoreItem.y * sy + view->y;
            func_0027B320(0x1FA, 0, x, y + sy * 8.0f, sx, sy, 1);
            func_00277CF0(player->options->score, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
        }
        if (D_8011ACE4) {
            value = player->options->frags;
            if (player->fragsItem.value != value) {
                func_0027AD00(&player->fragsItem, D_800ADC80);
                player->fragsItem.value = value;
            }
            if (player->fragsItem.anim && player->fragsItem.visible) {
                x = player->fragsItem.x * sx + view->x + view->width;
                y = player->fragsItem.y * sy + view->y;
                func_0027B320(0x1FD, 0, x, y + sy * 8.0f, sx, sy, 1);
                func_00277CF0(player->options->frags, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
            }
        }
    }
    if (player->options->tagMode == 1) {
        func_00254768(player, view);
    }
}

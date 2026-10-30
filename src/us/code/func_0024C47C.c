/* One source file from func_0024C47C to func_0024D344 (at least: the file
 * starts earlier, on a 16-byte boundary in .text, but this is the shortest
 * run whose .rodata starts 8-aligned). func_0024D344's pool
 * starts at 0x800A6CFC, 4 past an 8-byte boundary, and its 2^32 double is
 * 8-aligned only counted from 0x800A6C70, where func_0024C47C's pool opens;
 * func_0024C648's (0x800A6C74) and func_0024CF58's (0x800A6CDC) are 4 past a
 * boundary too, so neither can open the file.
 *
 * The parts were written apart: each keeps its own view of the player
 * (WheelPlayer, HudPlayer, SwitchPlayer, Player), and they share one
 * WeaponInfo. func_0024D344 is after Rage Wars (func_80220ED4_de). */
#include "common.h"

typedef struct WeaponMenu WeaponMenu;

/* A weapon's definition (D_800AF618[weapon]). */
typedef struct WeaponInfo {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 sound;
    /* 0x06 */ u8 pad6[0xC - 0x6];
    /* 0x0C */ s16 upgrade;         /* the weapon that replaces this one */
    /* 0x0E */ u8 padE[0x14 - 0xE];
    /* 0x14 */ u32 flags;           /* 1: in the wheel, 2: in the wheel underwater */
    /* 0x18 */ u8 pad18[0x20 - 0x18];
    /* 0x20 */ struct AmmoDef *ammo[3];
} WeaponInfo;

typedef struct WheelPlayer {
    /* 0x000 */ u8 pad0[0x3C8];
    /* 0x3C8 */ u32 aiFlags;
    /* 0x3CC */ u8 pad3CC[0x972 - 0x3CC];
    /* 0x972 */ s8 owned[0x23];
} WheelPlayer;

extern WeaponInfo *D_800AF618[];
extern s32 D_80110058;
extern s32 D_8011005C;

void func_0024BF1C(WeaponMenu *menu, s32 rows, s32 *owned, s32 perRow, WheelPlayer *player);

/* Opens the weapon wheel: picks the weapons to show (owned, allowed here,
 * not superseded by an owned upgrade) and how many times round to repeat
 * them so that the slots are at most 20 degrees apart. */
void func_0024C47C(WeaponMenu *menu, WheelPlayer *player) {
    s32 shown[0x23];
    s32 count = 0;
    s32 range = D_8011005C - D_80110058 + 1;
    s32 rows;
    s32 slots;
    s32 i;

    for (i = 0; i < 0x23; i++) {
        WeaponInfo *info = D_800AF618[i];
        s32 show;

        if (!player->owned[i]) {
            show = 0;
        } else if (!((player->aiFlags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            show = 0;
        } else {
            s16 upgrade = info->upgrade;

            show = 1;
            if (player->owned[upgrade]) {
                show = !(i < upgrade);
            }
        }
        shown[i] = show;
        if (show) {
            count++;
        }
    }
    rows = range / count;
    if (rows < 2) {
        rows = 1;
        if (count < range * 0.75f) {
            rows = 2;
        }
    }
    slots = rows * count;
    while (360 / slots > 20) {
        slots += count;
        rows++;
    }
    func_0024BF1C(menu, rows, shown, count, player);
}

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
} HudOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x380];
    /* 0x380 */ s32 underwater;
} Strokes;

typedef struct {
    /* 0x0000 */ u8 pad0[0x4F8];
    /* 0x04F8 */ f32 charge;
    /* 0x04FC */ u8 pad4FC[0x518 - 0x4FC];
    /* 0x0518 */ HudOptions *options;
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
} HudPlayer;

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
void func_0024A86C(HudPlayer *player, View *view);
void func_00247438(void *crosshair, View *view);
void func_00279028(s32 alpha);
void func_00254D5C(Strokes *strokes, View *view, HudPlayer *player);
void func_0024B3E4(Strokes *strokes, View *view, HudPlayer *player);
s32 func_0024E700(HudPlayer *player, s32 weapon);
s32 func_0024E7D0(HudPlayer *player);
void func_0027B320(s32 sprite, s32 frame, s16 x, s16 y, f32 sx, f32 sy, s32);
void func_00277CF0(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32, s32 rightAlign, s32 minDigits);
void func_00254768(HudPlayer *player, View *view);

/* The next display list command. Through this helper, as in the ROM, the
 * head is loaded and stored by symbol; a post-increment written in place
 * keeps the address of the head in a register instead. */
static __inline__ Gfx *dl_next(void) {
    return D_800D8E24++;
}

/* Draw a player's HUD: tick the item animations, the crosshair and strokes,
 * then the ammo (with the charge gauge of weapon 0xDD), the health, the
 * lives, the keys in single player, and the score and frags in multiplayer. */
void func_0024C648(HudPlayer *player, View *view) {
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


typedef struct Quad {
    s32 w[4];
} Quad;

typedef struct Target {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0x40 - 0x14];
    /* 0x40 */ Quad quad;
    /* 0x50 */ f32 angle;
} Target;

typedef struct ViewState {
    /* 0x000 */ u8 pad0[0x510];
    /* 0x510 */ s32 mode;
    /* 0x514 */ u8 pad514[8];
    /* 0x51C */ void *camera;
    /* 0x520 */ u8 pad520[0xA64 - 0x520];
    /* 0xA64 */ u32 buttons;
    /* 0xA68 */ u8 padA68[0xAB0 - 0xA68];
    /* 0xAB0 */ f32 unkAB0;
    /* 0xAB4 */ u8 padAB4[0xAC0 - 0xAB4];
    /* 0xAC0 */ f32 unkAC0;
    /* 0xAC4 */ u8 padAC4[4];
    /* 0xAC8 */ f32 eye[3];
    /* 0xAD4 */ f32 eyeOffset[3];
    /* 0xAE0 */ f32 lookOffset[3];
    /* 0xAEC */ u8 padAEC[0xB24 - 0xAEC];
    /* 0xB24 */ f32 unkB24;
    /* 0xB28 */ u8 padB28[0xB90 - 0xB28];
    /* 0xB90 */ f32 unkB90;
} ViewState;

typedef struct Orbit {
    /* 0x0 */ f32 angle;
    /* 0x4 */ f32 distance;
} Orbit;

#define MAX(a, b) ((a) < (b) ? (b) : (a))

extern f32 D_800AE308;
extern f32 D_800AE30C;

f32 func_002982F0(f32);             /* sin */
f32 func_002974C0(f32);             /* cos */
void func_00280C94(void *camera, s32 type, f32 a, f32 b, f32 x0, f32 y0, f32 x1, f32 y1, f32 z1,
                   f32 h, Quad quad, s32 region, f32 c, f32 d);

/* Points the camera at : in mode 1 from the view's offsets, in
 * mode 2 orbiting it, the buttons turning the orbit and moving it in or out. */
void func_0024CF58(ViewState *view, Target *target) {
    void *camera;
    f32 angle;
    f32 x;

    camera = view->camera;
    if (camera == NULL) {
        return;
    }
    if (view->mode == 1) {
        func_00280C94(camera, 0, target->angle, view->eye[0] + view->eyeOffset[0] - view->unkAB0,
                      view->eye[1] + view->eyeOffset[1], view->eye[2] + view->eyeOffset[2],
                      target->x + view->lookOffset[0], target->y, target->z + view->lookOffset[2],
                      MAX(view->lookOffset[1] + view->unkB24 - view->unkAC0, 40.96f), target->quad, target->region, view->unkB90, 3.0f);
    } else if (view->mode == 2) {
        if (view->buttons & 0x200) {
            D_800AE308 += 0.069813177f;
        } else if (view->buttons & 0x100) {
            D_800AE308 -= 0.069813177f;
        } else if (view->buttons & 0x800) {
            D_800AE30C -= 4.096f;
        } else if (view->buttons & 0x400) {
            D_800AE30C += 4.096f;
        }
        angle = target->angle + D_800AE308;
        x = target->x + D_800AE30C * func_002982F0(angle);
        func_00280C94(camera, 1, angle, 0.0f, 0.0f, 0.0f, x, target->y + 81.92f,
                      target->z + D_800AE30C * func_002974C0(angle), 0.0f, target->quad, target->region, 0.0f, 3.0f);
    }
}


typedef struct SwitchPlayer {
    /* 0x0000 */ u8 pad0[0x288];
    /* 0x0288 */ u8 unk288[0x3C8 - 0x288];
    /* 0x03C8 */ u8 unk3C8[0x996 - 0x3C8];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ s16 savedWeapon[3];
    /* 0x099E */ u8 pad99E[0xA08 - 0x99E];
    /* 0x0A08 */ u16 state;
    /* 0x0A0A */ u8 padA0A[0xA1C - 0xA0A];
    /* 0x0A1C */ u32 flags;
    /* 0x0A20 */ u8 padA20[0xB14 - 0xA20];
    /* 0x0B14 */ s16 nextWeapon;
    /* 0x0B16 */ u8 padB16[0x129C - 0xB16];
    /* 0x129C */ s32 lastWeaponSet;
    /* 0x12A0 */ s32 weaponSet;
} SwitchPlayer;

extern WeaponInfo *D_800AF618[];

void func_00258668(void *a, void *b);

/* Picks the player's weapon set (0 on foot, 1 in states 10-11, 2 with
 * flag 0x8000); when it changes, remembers the weapon held in the old set
 * if it belongs there and queues the new set's weapon. */
void func_0024D1F8(SwitchPlayer *player) {
    s32 keep;
    s16 weapon;

    player->lastWeaponSet = player->weaponSet;
    if ((u32) (player->state - 10) < 2) {
        player->weaponSet = 1;
    } else if (player->flags & 0x8000) {
        player->weaponSet = 2;
    } else {
        player->weaponSet = 0;
    }
    if (player->lastWeaponSet == player->weaponSet) {
        return;
    }
    weapon = player->weapon;
    switch (player->lastWeaponSet) {
    case 0:
        if (!(D_800AF618[weapon]->flags & 1)) {
            goto no;
        }
        keep = 1;
        break;
    case 1:
        if (!(D_800AF618[weapon]->flags & 2)) {
            goto no;
        }
        keep = 1;
        break;
    case 2:
        if (weapon != 19) {
            goto no;
        }
        keep = 1;
        break;
    default:
    no:
        keep = 0;
        break;
    }
    if (keep) {
        player->savedWeapon[player->lastWeaponSet] = player->weapon;
    }
    player->nextWeapon = player->savedWeapon[player->weaponSet];
    if (player->nextWeapon == 19) {
        func_00258668(player->unk288, player->unk3C8);
    }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_8.CPP, func_80220ED4_de, with Turok 2's layouts. */

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct Model {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;
} Model;

typedef struct ActorDef {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 kind;
    /* 0x0E */ u8 padE[0x18 - 0xE];
    /* 0x18 */ s32 maxHealth;
} ActorDef;

/* What the actor stands in (func_00268A2C). */
typedef struct Surface {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ u16 item;
    /* 0x4E */ u8 pad4E[0x54 - 0x4E];
    /* 0x54 */ u16 unk54;
} Surface;

typedef struct Options {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} Options;

typedef struct Controller {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ Vec3f pos;
} Controller;

typedef struct Input {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Controller *controller;
    /* 0x14 */ u8 pad14[0x24 - 0x14];
    /* 0x24 */ u32 held;
    /* 0x28 */ u32 pressed;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} Input;

typedef struct Voice {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 bank;
} Voice;

typedef struct Player {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ Model *model;
    /* 0x0014 */ ActorDef *def;
    /* 0x0018 */ u8 pad18[4];
    /* 0x001C */ f32 velY;
    /* 0x0020 */ u8 pad20[0x40 - 0x20];
    /* 0x0040 */ Quad unk40;
    /* 0x0050 */ f32 yaw;
    /* 0x0054 */ u8 pad54[0xD4 - 0x54];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0xDC - 0xD8];
    /* 0x00DC */ s16 anim;
    /* 0x00DE */ u8 padDE[0xE2 - 0xDE];
    /* 0x00E2 */ s8 animPending;
    /* 0x00E3 */ u8 padE3[0x140 - 0xE3];
    /* 0x0140 */ u32 aiFlags;          /* CAI */
    /* 0x0144 */ u8 pad144[0x1A8 - 0x144];
    /* 0x01A8 */ struct Player *owner;
    /* 0x01AC */ u8 pad1AC[0x288 - 0x1AC];
    /* 0x0288 */ u8 emitter[4];
    /* 0x028C */ Vec3f emitterPos;
    /* 0x0298 */ Model *emitterModel;
    /* 0x029C */ u8 pad29C[0x2C8 - 0x29C];
    /* 0x02C8 */ Quad emitter40;
    /* 0x02D8 */ f32 emitterYaw;
    /* 0x02DC */ u8 pad2DC[0x3C8 - 0x2DC];
    /* 0x03C8 */ u8 unk3C8[0x3F4 - 0x3C8];
    /* 0x03F4 */ Voice *voice;
    /* 0x03F8 */ u8 pad3F8[0x430 - 0x3F8];
    /* 0x0430 */ struct Player *unk430;
    /* 0x0434 */ u8 pad434[0x510 - 0x434];
    /* 0x0510 */ s32 control;
    /* 0x0514 */ u8 pad514[4];
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 pad51C[4];
    /* 0x0520 */ s32 character;
    /* 0x0524 */ s32 health;               /* 8.8 */
    /* 0x0528 */ u8 pad528[2];
    /* 0x052A */ s16 collide;
    /* 0x052C */ u8 pad52C[0x6A9 - 0x52C];
    /* 0x06A9 */ u8 unk6A9;
    /* 0x06AA */ u8 pad6AA;
    /* 0x06AB */ u8 unk6AB;
    /* 0x06AC */ u8 pad6AC[0x92C - 0x6AC];
    /* 0x092C */ s32 item;
    /* 0x0930 */ s32 lastItem;
    /* 0x0934 */ s16 ammo[31];
    /* 0x0972 */ s8 owned[36];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA08 - 0x998];
    /* 0x0A08 */ s16 state;
    /* 0x0A0A */ u8 padA0A[2];
    /* 0x0A0C */ s16 prevState;
    /* 0x0A0E */ u8 padA0E[2];
    /* 0x0A10 */ f32 stateTime;
    /* 0x0A14 */ u32 prevStateFlags;
    /* 0x0A18 */ u8 padA18[4];
    /* 0x0A1C */ u32 stateFlags;
    /* 0x0A20 */ u8 padA20[4];
    /* 0x0A24 */ f32 step;
    /* 0x0A28 */ f32 unkA28;
    /* 0x0A2C */ u8 padA2C[4];
    /* 0x0A30 */ f32 unkA30;
    /* 0x0A34 */ u8 padA34[0xA40 - 0xA34];
    /* 0x0A40 */ Input input;
    /* 0x0A78 */ f32 velX;
    /* 0x0A7C */ f32 velZ;
    /* 0x0A80 */ f32 speed;
    /* 0x0A84 */ f32 lastVelY;
    /* 0x0A88 */ s32 onGround;
    /* 0x0A8C */ u8 padA8C[0xA9C - 0xA8C];
    /* 0x0A9C */ f32 airTime;
    /* 0x0AA0 */ Vec3f goal;
    /* 0x0AAC */ f32 unkAAC;
    /* 0x0AB0 */ u8 padAB0[0xAC0 - 0xAB0];
    /* 0x0AC0 */ f32 crouch;
    /* 0x0AC4 */ s32 crouchToggle;
    /* 0x0AC8 */ f32 unkAC8;
    /* 0x0ACC */ u8 padACC[0xAFC - 0xACC];
    /* 0x0AFC */ f32 unkAFC;
    /* 0x0B00 */ f32 unkB00;
    /* 0x0B04 */ u8 padB04[0xB14 - 0xB04];
    /* 0x0B14 */ s16 nextWeapon;
    /* 0x0B16 */ u8 padB16[2];
    /* 0x0B18 */ Vec3f lastPos;
    /* 0x0B24 */ u8 padB24[4];
    /* 0x0B28 */ f32 unkB28;
    /* 0x0B2C */ u8 padB2C[0xC04 - 0xB2C];
    /* 0x0C04 */ s32 action;
    /* 0x0C08 */ s32 lastAction;
    /* 0x0C0C */ u8 padC0C[0xCD0 - 0xC0C];
    /* 0x0CD0 */ s32 unkCD0;
    /* 0x0CD4 */ u8 padCD4[0x1050 - 0xCD4];
    /* 0x1050 */ s32 unk1050;
    /* 0x1054 */ u8 pad1054[0x1298 - 0x1054];
    /* 0x1298 */ f32 shake;
    /* 0x129C */ u8 pad129C[0x12A4 - 0x129C];
    /* 0x12A4 */ s32 unk12A4;
    /* 0x12A8 */ u8 pad12A8[4];
    /* 0x12AC */ s32 unk12AC;
} Player;

typedef struct AmmoDef {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 max;
} AmmoDef;

typedef struct PlayerStateInfo {
    /* 0x00 */ void (*enter)(Player *, void *);
    /* 0x04 */ void (*update)(Player *, Player *);
    /* 0x08 */ u32 *flags;
    /* 0x0C */ u8 padC[0x18 - 0xC];
} PlayerStateInfo;

/* D_8011AAD8; the block before it (0xCAC bytes back) is D_80119E2C. */
typedef struct Settings {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x19 - 4];
    /* 0x19 */ u8 multiplayer;
    /* 0x1A */ u8 pad1A[0xCAC - 0x1A];
} Settings;

typedef struct DamageEvent {
    s32 w[6];
} DamageEvent;

extern f32 D_800B6D28;              /* frame time */
extern Settings D_8011AAD8;
extern s32 D_8011AAA0;
extern s32 D_8011ACE4;
extern s32 D_800B6D1C;
extern f32 D_800ADD88[];           /* health regeneration per character */
extern WeaponInfo *D_800AF618[];
extern PlayerStateInfo D_800ADF54[];
extern u32 D_800F1E30;
extern u32 D_800F1F10;
extern u8 *D_800F1CE0;
extern void *D_800F1D7C;
extern s32 D_800F1D90;
extern u8 D_800F7078;
extern s32 D_80110054;
extern s32 D_800ADB30[];
extern s32 D_800ADB5C;

s32 Cinema_PathPlaying(void);
void func_00206068(s32);
void func_00275B08(s32 enable);
void func_00242E34(Player *, u32 *ai);
void func_0021A344(Player *);
void func_00242C00(Player *, u32 *ai);
Surface *func_00268A2C(Player *);
void func_00254640(Player *, Player *, Surface *);
f32 func_00269C3C(Model *, f32 x, f32 z);
f32 func_0026BE60(Model *, f32 x, f32 z);
void func_00275B30(f32);
void func_002545F0(Player *, Player *);
f32 func_00298470(f32);
s32 func_002548FC(Player *);
f32 func_0021149C(f32);
void func_00245828(DamageEvent *, s32, s32, f32, s32, s32);
void func_00247C10(Player *, u32 *ai, DamageEvent *);
void func_00275544(s32 sound, Vec3f pos, s32, s32);
s32 func_002685F0(Player *);
void func_002699E4(Player *);
void func_0024E9D0(Player *, Player *);
void func_0024E88C(Player *, Player *);
void func_00255220(Player *, Player *);
void func_00255298(Player *, Player *);
void func_00247660(Player *);
void func_00269050(Player *, Surface *, Vec3f *, s32);
void func_00249358(Player *, Player *, s32, s32, s32);
void func_002543A4(Player *, Player *, Surface *);
void func_0024AB0C(Player *, Player *);
s32 func_00239F00(Player *, Vec3f, u32 *);
void func_0026E19C(Model *, s32, Player *);
void func_002233A8(void *, Player *, Vec3f *, Vec3f *);
void func_0026E110(u8 *, s32, Player *);
void func_0024ED54(Player *, Player *, u32 *);
s32 func_0024EAF8(Player *, Player *, u32 *);
void func_0023F05C(Player *, u8 *);
void func_002553BC(Player *, Player *);
void func_00248BA8(Player *, s32, s32, s32);
f32 func_0026872C(Player *, s32);
void func_002241D8(void *, void *, s32, s32);
s32 func_0021E9A4(Player *, s32, s32);
s32 func_00219F74(Player *, s32, s32);
void func_0021E9F8(Player *, s32, s32);
void func_0021A500(Player *);
s16 *func_0025398C(Player *, s32);
void func_0024BD18(s32 *, Player *, u8 *);
void func_0024A0DC(Player *);

static __inline__ s32 link_enabled(u8 *bits) {
    if (D_8011AAD8.flags & 0x10) {
        return 1;
    }
    return *bits & 0xF;
}

static __inline__ s32 not_flagged(u32 flags, u32 bit) {
    return (flags & bit) == 0;
}

/* Entering a surface that changes the player's state (flags 0x40000): through
 * a 0x200000 surface only from one without the flag, otherwise only from
 * above the floor. */
static __inline__ void enter_surface(Player *p, Player *actor, Surface *before, Surface *after, Model *model) {
    s32 ok = 0;     /* dead store, but it keeps the 2.5f of the shake out of the loop's hoisted invariants */

    if (p->stateFlags & 0x8000) {
        return;
    }
    if (not_flagged(after->flags, 0x40000)) {
        return;
    }
    if (after->flags & 0x200000) {
        ok = not_flagged(before->flags, 0x40000);
    } else if (func_0026BE60(model, actor->pos.x, actor->pos.z) < actor->pos.y) {
        ok = 0;
    } else {
        ok = 1;
    }
    if (ok) {
        func_00248BA8(p, after->unk48, (after->unk54 & 1) ? 1 : 2, 0);
    }
}

/* Moves the player one frame: input toggles, health regeneration in
 * multiplayer, then the state machine and collision in a loop over the frame
 * time, the landscape ceiling, the animation for the current action, the
 * cheats (all weapons / ammo) and the weapon sounds. */
void func_0024D344(Player *p) {
    Vec3f prev;
    Input saved;
    DamageEvent damage;
    s32 soundFlags;
    s32 human;
    s32 linkFlags;
    f32 scale;
    f32 remaining;
    f32 rate;
    f32 regen;
    Player *actor;
    Settings *game;
    s32 enable;
    Model *oldModel;
    Model *model;
    Surface *material;
    Surface *before;
    Surface *under;
    u32 *stateFlags;
    s32 hit;
    s32 landed;
    s32 flag;
    s32 first;
    s32 second;
    s32 low;
    s32 high;
    s32 sound;
    s32 i;
    s32 j;

    linkFlags = 0;
    scale = D_800B6D28;
    actor = p;
    soundFlags = 0;
    human = p->control == 2;
    if (Cinema_PathPlaying()) {
        human = 1;
    }
    game = &D_8011AAD8;
    if (game->multiplayer) {
        human = 1;
        func_00206068(8);
    }
    enable = 0;
    p->owner = p;
    p->unk430 = p;
    if ((p->aiFlags & 0x80) && game->multiplayer == 0) {
        enable = (game - 1)->flags == 0;
    }
    func_00275B08(enable);
    if (D_8011AAF1.multiplayer == 0) {
        if (!(D_8011AAA0 != 0 && (p->input.held & 0x20)) && (p->input.pressed & 0x800)) {
            if (p->unk12A4 != 0) {
                p->unk12A4 = 0;
            } else {
                p->unk12A4 = 1;
            }
        }
        if (D_8011AAF1.multiplayer == 0 && !(p->unk1050 != 0 && p->unkCD0 != 0)
            && !(D_8011AAA0 != 0 && (p->input.held & 0x20)) && (p->input.pressed & 0x200)) {
            WeaponInfo *info = D_800AF618[p->nextWeapon];
            s16 next = info->upgrade;

            if (next != -1 && p->owned[next] != 0) {
                p->nextWeapon = info->upgrade;
            }
        }
    }
    regen = 0.0f;   /* also serves the comparisons with 0 (f2 in the ROM) */
    if (D_8011AAF1.multiplayer != 0 && (rate = D_800ADD88[p->character]) != 0.0f) {
        f32 health = p->health;

        if (health != 0.0f && p->options->tagMode == 0) {
            f32 max;

            regen = health + (f32) (s32) (rate * 256.0f) * (D_800B6D28 * 0.06666667f);
            max = (u32) (p->def->maxHealth << 8);
            if (!(max <= regen)) {
                max = regen;
            }
            p->health = max;
        }
    }
    actor->flags &= ~0x20000;
    func_00242E34(actor, &actor->aiFlags);
    if (human) {
        func_0021A344(actor);
    }
    D_800B6D28 = scale;
    func_00242C00(actor, &actor->aiFlags);
    if (link_enabled(&p->unk6A9)) {
        linkFlags |= 0x800;
    }
    if (link_enabled(&p->unk6AB)) {
        linkFlags |= 0x1000;
    }
    if (p->options->tagMode == 1) {
        linkFlags |= 0x2000;
        soundFlags |= 0x400000;
    }
    Cinema_PathPlaying();
    if (p->unkAAC > 0.0f) {
        f32 left = p->unkAAC - D_800B6D28;

        if (left < 0.0f) {
            left = 0.0f;
        }
        p->unkAAC = left;
    }
    if (p->unkA28 > 0.0f) {
        f32 left = p->unkA28 - D_800B6D28;

        if (left < 0.0f) {
            left = 0.0f;
        }
        p->unkA28 = left;
    }
    /* Run the frame in steps (one, here): the input's edges are seen once. */
    saved = p->input;
    for (remaining = D_800B6D28; remaining > 0.0f;) {
        remaining = 0.0f;
        func_00242E34(actor, &actor->aiFlags);
        p->prevState = (u16) p->state;
        p->prevStateFlags = p->stateFlags;
        model = actor->model;
        material = func_00268A2C(actor);
        func_00254640(p, actor, material);
        if (D_8011AAF1.multiplayer == 0) {
            Vec3f *pos = &p->pos;

            if (model != NULL && pos != NULL && (model->flags & 0x40)) {
                f32 amount = (s32) (func_00269C3C(model, p->pos.x, pos->z) - func_0026BE60(model, p->pos.x, pos->z));

                if (amount < 1024.0f) {
                    func_00275B30(1.0f - amount * 0.0009765625f);
                } else {
                    func_00275B30(0.0f);
                }
            } else {
                func_00275B30(0.0f);
            }
        }
        if (material != NULL) {
            p->step = D_800B6D28 * p->unkB28;
        } else {
            p->step = D_800B6D28;
        }
        func_002545F0(p, actor);
        p->speed = func_00298470(p->velX * p->velX + p->velZ * p->velZ);
        p->unkB00 += p->speed * 0.021000002f;
        p->unkAFC += p->speed * 1.00000005e-4f;
        p->goal = actor->pos;
        if (p->shake > 0.0f && p->health != 0 && !func_002548FC(p)) {
            p->goal.x += (func_0021149C(5.0f) - 2.5f) * 10.24f;
            p->goal.z += (func_0021149C(5.0f) - 2.5f) * 10.24f;
            p->goal.y += func_0021149C(1.0f) * 10.24f;
            p->yaw += (func_0021149C(20.0f) - 10.0f) * 0.017453294f;
            p->unkAC8 += func_0021149C(0.5f) - 0.25f;
            if (D_800B6D1C != 0) {
                func_00245828(&damage, p->unk12AC, 0x19, 25.599998f, 0x100000, 0);
                func_00247C10(p, &p->aiFlags, &damage);
            }
            func_00275544(0x227, p->pos, 0, -1);
        }
        p->stateTime += D_800B6D28;
        {
            s32 ground = func_002685F0(actor);

            if (ground != 0 && p->onGround == 0 && p->lastVelY < -51.199997f) {
                func_002699E4(actor);
            }
            p->onGround = ground;
            p->lastVelY = actor->velY;
            if (ground == 0 && p->state != 14 && !(actor->aiFlags & 0x200)) {
                p->airTime += D_800B6D28;
            } else {
                p->airTime = 0.0f;
            }
        }
        if ((u16) p->state >= 2 && p->health != 0) {
            func_0024E9D0(p, actor);
            func_0024E88C(p, actor);
            func_00255220(p, actor);
            func_00255298(p, actor);
            func_00247660(p);
        }
        if (!Cinema_PathPlaying() || (u32) ((u16) p->state - 19) < 12) {
            void (*update)(Player *, Player *) = D_800ADF54[p->state].update;

            if (update != NULL) {
                update(p, actor);
            }
        }
        material = func_00268A2C(actor);
        if (material != NULL) {
            func_00269050(actor, material, &p->goal, 1);
            if ((material->flags & 0x4000) && actor->pos.y <= func_0026BE60(actor->model, actor->pos.x, actor->pos.z)) {
                func_00249358(p, actor, 0x3E700, 0, 0x1D);
            }
        }
        stateFlags = D_800ADF54[p->state].flags;
        if (p->state == 14) {
            stateFlags = &D_800F1E30;
            if (p->velX >= 0.0f) {
                stateFlags = &D_800F1F10;
            }
        }
        flag = 0;
        if (actor->aiFlags & 0x6000) {
            flag = 1;
        } else if ((u32) ((u16) p->state - 2) < 2) {
            if ((p->input.pressed & 0x400) && D_8011AAF1.multiplayer == 0 && !(D_8011AAA0 != 0 && (p->input.held & 0x20))) {
                p->crouchToggle ^= 1;
            }
        } else {
            p->crouchToggle = 0;
        }
        if (p->crouchToggle != 0) {
            flag = 1;
        }
        {
            f32 change;

            if (flag) {
                change = (40.96f - p->crouch) * 0.25f;
            } else {
                change = -p->crouch * 0.25f;
            }
            p->crouch += change * D_800B6D28;
            change = 40.96f - p->crouch;
            if (-10.24f <= change && change <= 10.24f) {
                *stateFlags |= 0x80;
            } else {
                *stateFlags &= ~0x80;
            }
        }
        func_002543A4(p, actor, material);
        if (p->unkA30 > 0.0f) {
            p->unkA30 -= D_800B6D28;
            if (p->unkA30 < 0.0f) {
                p->unkA30 = 0.0f;
            }
        }
        func_0024AB0C(p, actor);
        oldModel = actor->model;
        prev = actor->pos;
        before = func_00268A2C(actor);
        if (p->collide != 0) {
            actor->flags |= 0x10000;
            hit = func_00239F00(actor, p->goal, stateFlags);
            actor->flags &= ~0x10000;
        } else {
            hit = 0;
        }
        model = actor->model;
        material = func_00268A2C(actor);
        if (before != material) {
            func_0026E19C(oldModel, linkFlags | 2, actor);
            func_0026E19C(model, linkFlags | 1, actor);
        }
        func_002233A8(&D_800F7078, actor, &prev, &actor->pos);
        landed = 0;
        if (hit) {
            if (D_800F1CE0 != NULL && *D_800F1CE0 == 1) {
                func_0026E110(D_800F1CE0, soundFlags | 2, actor);
            }
            if (D_800F1D7C != NULL) {
                func_0026E19C(D_800F1D7C, linkFlags | 4, actor);
            }
            func_0024ED54(p, actor, stateFlags);
            landed = func_0024EAF8(p, actor, stateFlags);
            if (D_800F1D90 == 2 && (u32) ((u16) p->state - 4) < 2) {
                if (actor->velY > 0.0f) {
                    actor->velY = 0.0f;
                }
                p->state = 5;
                p->velZ *= 0.1f;
                p->velX *= 0.1f;
            }
        }
        if (D_800F1CE0 != NULL && *D_800F1CE0 == 3) {
            func_0023F05C(p, D_800F1CE0);
        }
        if (before != NULL && material != NULL) {
            if ((p->stateFlags & 0x8000) && !landed && !(before->unk54 & 8) && (material->unk54 & 8)) {
                func_002553BC(p, actor);
            }
            if (material->flags & 0x4000000) {
                if (material->item != p->item) {
                    p->item = material->item;
                }
            }
            if (material->flags & 0x8000000) {
                p->item = material->item;
                p->lastItem = material->item;
                D_80110054 = material->item;
            }
            enter_surface(p, actor, before, material, model);
        }
        p->input.pressed = 0;
        p->input.unk2C = 0;
        p->input.unk30 = 0;
        p->input.unk34 = 0;
    }
    p->input = saved;
    D_800B6D28 = scale;
    under = func_00268A2C(actor);
    if (under != NULL && (under->flags & 0x4000)) {
        f32 x2 = actor->pos.x * actor->pos.x;
        f32 z2 = actor->pos.z * actor->pos.z;

        if (x2 + actor->pos.y * actor->pos.y + z2 > 4e8f) {
            f32 y = 4e8f - x2 - z2;

            if (y < 0.0f) {
                y = 0.0f;
            }
            y = func_00298470(y);
            if (actor->pos.y < 0.0f) {
                y = -y;
            }
            actor->pos.y = y;
            if (under->flags & 0x4000) {
                actor->pos.y = func_0026872C(actor, 0) - 1.0f;
            }
        }
    }
    if (human) {
        sound = 102;
        if (D_8011AAF1.multiplayer != 0) {
            if (D_8011ACE4 != 0 && p->options->tagMode != 0) {
                sound = D_800ADB5C;
            } else {
                sound = D_800ADB30[p->def->kind];
            }
        }
        func_002241D8(&D_800F7078, p, sound, p->action);
        if (p->lastAction != p->action) {
            if (!func_0021E9A4(actor, p->action, 0) && Cinema_PathPlaying()) {
                func_0021E9A4(actor, 7000, 0);
            }
        }
        first = func_00219F74(actor, p->action, -1);
        second = func_00219F74(actor, p->action, first);
        low = second;
        if (second > first) {
            low = first;
        }
        high = second;
        if (high < first) {
            high = first;
        }
        if (actor->animPending) {
            if (actor->anim == low) {
                if (func_0021149C(100.0f) < 0.4f) {
                    func_0021E9F8(actor, high, 0);
                    actor->animPending = 0;
                }
            } else {
                func_0021E9F8(actor, low, 0);
                actor->animPending = 0;
            }
        }
    }
    if (!human) {
        func_0021A500(actor);
    }
    p->emitterPos = p->pos;
    p->emitterYaw = p->yaw;
    p->emitterModel = p->model;
    p->emitter40 = p->unk40;
    p->lastPos = p->pos;
    if (D_8011AAD8.flags & 8) {
        for (i = 0; i < 31; i++) {
            if (i != 21) {
                p->ammo[i] = 0;
            }
        }
        for (i = 0; i < 35; i++) {
            WeaponInfo *info;

            if (i == 21) {
                continue;
            }
            info = D_800AF618[i];
            for (j = 0; j < 3; j++) {
                AmmoDef *def = info->ammo[j];
                s16 *slot;

                if (def == NULL) {
                    break;
                }
                slot = &p->ammo[def->type];
                *slot = (def->max < *slot) ? *slot : def->max;
            }
        }
    }
    if (D_8011AAD8.flags & 4) {
        if (D_8011AAD8.multiplayer == 0 && p->owned[21] == 0) {
            *(p->ammo + *func_0025398C(p, 21)) = 1;
        }
        for (i = 0; i < 35; i++) {
            if ((i >= 24) ^ !D_8011AAD8.multiplayer) {
                p->owned[i] = 1;
            }
        }
    }
    func_0024D1F8((SwitchPlayer *) p);
    func_0024BD18(&p->unkCD0, p, p->unk3C8);
    if (p->shake > 0.0f) {
        p->shake -= D_800B6D28;
        if (p->shake > 0.0f) {
            p->flags |= 0x800000;
        } else {
            p->flags &= ~0x800000;
        }
    }
    func_0024A0DC(p);
    p->input.controller->pos = p->pos;
    func_002241D8(&D_800F7078, p->emitter, D_800AF618[p->weapon]->sound, p->voice->bank);
    p->lastAction = p->action;
}

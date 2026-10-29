#include "common.h"

/* Draws the weapon menu's icons: sorts the listed items, places each one in
 * the player's viewport (on the wheel, or in a vertical strip that fades out
 * away from the middle), tints it (pulsing under the cursor, dark when the
 * weapon is not owned, dimmer without ammo) and draws the sprite. */

#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define ABS(x) ((x) < 0.0f ? -(x) : (x))

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Gfx {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct MenuSlot {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 hasAmmo;
    /* 0x0C */ u8 padC[0x18 - 0xC];
} MenuSlot;

typedef struct WeaponMenu {
    /* 0x000 */ s32 active;
    /* 0x004 */ u8 pad4[4];
    /* 0x008 */ f32 open;
    /* 0x00C */ u8 padC[0x14 - 0xC];
    /* 0x014 */ f32 phase;
    /* 0x018 */ s32 count;
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
    /* 0x380 */ s32 wheel;
} WeaponMenu;

typedef struct MenuItem {
    /* 0x00 */ s32 slot;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ s32 icon;
    /* 0x14 */ f32 alpha;
    /* 0x18 */ f32 size;
} MenuItem;

typedef struct MenuView {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ f32 viewport[4];
} MenuView;

typedef struct MenuPlayer {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ MenuView *view;
    /* 0x520 */ u8 pad520[0x972 - 0x520];
    /* 0x972 */ s8 owned[1];
} MenuPlayer;

extern u8 D_8011AAEA;
extern Gfx *D_800D8E24;

void func_00226F90(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 func_002571D8();
s32 func_00257124();
f32 func_002974C0(f32);
void func_0027AEA8(s32 sprite, s32 frame, s32 *width, s32 *height);
void func_0027B320(s32 sprite, s32 frame, s16 x, s16 y, f32 sx, f32 sy, s32 arg6);

void func_0024AF68(WeaponMenu *menu, MenuItem *items, s32 count, MenuPlayer *player) {
    s32 width;
    s32 height;
    MenuItem *item;
    MenuView *view;
    f32 *viewport;
    f32 fade;
    f32 scaleX;
    f32 scaleY;
    f32 x;
    f32 y;
    f32 alpha;
    f32 scale;
    s32 level;
    s32 half;
    s32 red;
    s32 green;
    s32 blue;

    func_00226F90(items, count, sizeof(MenuItem), func_002571D8, func_00257124);
    level = D_8011AAEA + 0x40;
    fade = menu->open * (level < 0x100 ? level : 255.0f);
    item = items;
    while (--count != -1) {
        view = player->view;
        scaleX = view->viewport[0] * (1.0f / 320);
        scaleY = view->viewport[1] * (1.0f / 240);
        viewport = view->viewport;
        if (menu->wheel == 0) {
            half = viewport[1] * 0.5f;
            x = viewport[2] + viewport[0] * 0.2f * menu->open;
            y = viewport[3] + half;
            y += item->position.y * 1.2f * viewport[1];
            if (item->position.z < 0.0f) {
                alpha = 0.0f;
            } else {
                alpha = MAX(1.0f - ABS(y - (viewport[3] + half)) / half, 0.0f);
            }
            scale = alpha * 0.4f + 0.6f;
            scaleX *= scale;
            scaleY *= scale;
        } else {
            x = viewport[2] + viewport[0] * 0.5f + item->position.x * 0.5f * viewport[0];
            y = viewport[3] + viewport[1] * 0.5f + item->position.y * 0.5f * viewport[1];
            alpha = item->alpha;
        }
        func_0027AEA8(item->icon, 0, &width, &height);
        if (width != 0) {
            if (menu->cursor == item->slot) {
                red = func_002974C0(menu->phase) * 63.0f + 192.0f;
                alpha = 1.0f;
                green = blue = red;
            } else {
                if (player->owned[menu->slots[item->slot].weapon] == 0) {
                    blue = green = red = 8;
                    alpha *= 0.25f;
                } else if (menu->slots[item->slot].hasAmmo == 0) {
                    blue = green = red = 0x40;
                    alpha *= 0.9f;
                } else {
                    blue = green = red = 200;
                }
            }
            {
                Gfx *_g = D_800D8E24++;
                _g->w0 = 0xFB000000;
                _g->w1 = (red << 24) | ((green & 0xFF) << 16) | ((blue & 0xFF) << 8) |
                         ((u32)(alpha * fade) & 0xFF);
            }
            func_0027B320(item->icon, 0, x - width * item->size * scaleX * 0.5f,
                          y - height * item->size * scaleY * 0.5f, scaleX * item->size,
                          scaleY * item->size, 1);
        }
        item++;
    }
}

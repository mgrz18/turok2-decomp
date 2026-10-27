#include "common.h"
#include "gfx.h"

#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

/* Screen box of a weapon, in viewport-relative units. */
typedef struct WeaponDef {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 left;
    /* 0x3C */ f32 top;
    /* 0x40 */ f32 right;
    /* 0x44 */ f32 bottom;
} WeaponDef;

typedef struct Viewport {
    /* 0x0 */ f32 scaleX;
    /* 0x4 */ f32 scaleY;
    /* 0x8 */ f32 offsetX;
    /* 0xC */ f32 offsetY;
} Viewport;

typedef struct View {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ Viewport vp;
} View;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ u8 hud[0x52A - 0x288];
    /* 0x52A */ s16 hudActive;
    /* 0x52C */ u8 pad52C[0x996 - 0x52C];
    /* 0x996 */ s16 weapon;
} Player;

extern WeaponDef *D_800AF618[];
extern u8 D_800F6CB0[];
void func_0020B5B8(void);
void func_0020B5D4(void);
void func_0020B624(void);
void func_0021D39C(void *, View *);
void func_0028852C(void *, s32, s32, s32, s32);

/* Draws the weapon's HUD overlay, scissored to its screen box. */
void func_00249898(Player *player, View *view) {
    WeaponDef *def;
    Viewport *vp;
    f32 sx, sy;

    if (player->weapon == -1 || player->hudActive == 0) {
        return;
    }
    GFX_CMD(0xE3001201, 0x2000)
    GFX_CMD(0xE3000C00, 0x80000)
    func_0020B5B8();
    def = D_800AF618[player->weapon];
    vp = &view->vp;
    sx = vp->scaleX;
    sy = vp->scaleY;
    func_0028852C(D_800F6CB0,
                  def->left * sx + vp->offsetX,
                  def->right * sx + vp->offsetX,
                  def->top * sy + vp->offsetY,
                  def->bottom * sy + vp->offsetY);
    GFX_CMD(0xD9FFFFFF, 0x10000)
    GFX_CMD(0xDB040004, 1)
    GFX_CMD(0xDB04000C, 1)
    GFX_CMD(0xDB040014, 0xFFFF)
    GFX_CMD(0xDB04001C, 0xFFFF)
    func_0020B5D4();
    func_0021D39C(player->hud, view);
    func_0020B624();
}

#include "common.h"
#include "structs/player.h"
#include "gfx.h"
#include "structs/weapon.h"
#include "structs/camera.h"

#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

extern u8 D_800F6CB0[];
void func_0020B5B8(void);
void func_0020B5D4(void);
void func_0020B624(void);
void func_0021D39C(void *, CCamera *);
void func_0028852C(void *, s32, s32, s32, s32);

/* Draws the weapon's HUD overlay, scissored to its screen box. */
void func_00249898(CPlayer *player, CCamera *view) {
    WeaponInfo *def;
    CCameraViewport *vp;
    f32 sx, sy;

    if (player->m_Data.m_CurrentWeapon == -1 || player->m_Data.m_Lives == 0) {
        return;
    }
    GFX_CMD(0xE3001201, 0x2000)
    GFX_CMD(0xE3000C00, 0x80000)
    func_0020B5B8();
    def = D_800AF618[player->m_Data.m_CurrentWeapon];
    vp = &view->m_Viewport;
    sx = vp->m_Width;
    sy = vp->m_Height;
    func_0028852C(D_800F6CB0,
                  def->scopeLeft * sx + vp->m_ScreenPosX,
                  def->scopeRight * sx + vp->m_ScreenPosX,
                  def->scopeTop * sy + vp->m_ScreenPosY,
                  def->scopeBottom * sy + vp->m_ScreenPosY);
    GFX_CMD(0xD9FFFFFF, 0x10000)
    GFX_CMD(0xDB040004, 1)
    GFX_CMD(0xDB04000C, 1)
    GFX_CMD(0xDB040014, 0xFFFF)
    GFX_CMD(0xDB04001C, 0xFFFF)
    func_0020B5D4();
    func_0021D39C(&player->m_WeaponActor, view);
    func_0020B624();
}

#include "common.h"
#include "structs/vector.h"
#include "gfx.h"
#include "structs/weapon.h"
#include "globals/game.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Draws the weapon menu's icons: sorts the listed items, places each one in
 * the player's viewport (on the wheel, or in a vertical strip that fades out
 * away from the middle), tints it (pulsing under the cursor, dark when the
 * weapon is not owned, dimmer without ammo) and draws the sprite. */

#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define ABS(x) ((x) < 0.0f ? -(x) : (x))


void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 CBarrel__CompareIcons();
s32 CBarrel__SwapIcons();
f32 func_002974C0(f32);
void Hud_GetSpriteSize(s32 sprite, s32 frame, s32 *width, s32 *height);
void Hud_DrawSprite(s32 sprite, s32 frame, s16 x, s16 y, f32 sx, f32 sy, s32 arg6);

void CBarrel__DrawIcons(CBarrel *menu, CBarrelIcon *items, s32 count, CPlayer *player) {
    s32 width;
    s32 height;
    CBarrelIcon *item;
    CCamera *view;
    CCameraViewport *viewport;
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

    Sort_Quick(items, count, sizeof(CBarrelIcon), CBarrel__CompareIcons, CBarrel__SwapIcons);
    level = gEngineApp_m_Settings.hudAlpha + 0x40;
    fade = menu->m_Open * (level < 0x100 ? level : 255.0f);
    item = items;
    while (--count != -1) {
        view = player->m_pCamera;
        scaleX = view->m_Viewport.m_Width * (1.0f / 320);
        scaleY = view->m_Viewport.m_Height * (1.0f / 240);
        viewport = &view->m_Viewport;
        if (menu->m_bStickWheel == 0) {
            half = viewport->m_Height * 0.5f;
            x = viewport->m_ScreenPosX + viewport->m_Width * 0.2f * menu->m_Open;
            y = viewport->m_ScreenPosY + half;
            y += item->m_vPos.y * 1.2f * viewport->m_Height;
            if (item->m_vPos.z < 0.0f) {
                alpha = 0.0f;
            } else {
                alpha = MAX(1.0f - ABS(y - (viewport->m_ScreenPosY + half)) / half, 0.0f);
            }
            scale = alpha * 0.4f + 0.6f;
            scaleX *= scale;
            scaleY *= scale;
        } else {
            x = viewport->m_ScreenPosX + viewport->m_Width * 0.5f + item->m_vPos.x * 0.5f * viewport->m_Width;
            y = viewport->m_ScreenPosY + viewport->m_Height * 0.5f + item->m_vPos.y * 0.5f * viewport->m_Height;
            alpha = item->m_Alpha;
        }
        Hud_GetSpriteSize(item->m_Icon, 0, &width, &height);
        if (width != 0) {
            if (menu->m_nCursor == item->m_Item) {
                red = func_002974C0(menu->m_Glow) * 63.0f + 192.0f;
                alpha = 1.0f;
                green = blue = red;
            } else {
                if (player->m_Data.m_WeaponList[menu->m_Items[item->m_Item].m_Weapon] == 0) {
                    blue = green = red = 8;
                    alpha *= 0.25f;
                } else if (menu->m_Items[item->m_Item].m_bAvailable == 0) {
                    blue = green = red = 0x40;
                    alpha *= 0.9f;
                } else {
                    blue = green = red = 200;
                }
            }
            {
                Gfx *_g = gDList++;
                _g->w0 = 0xFB000000;
                _g->w1 = (red << 24) | ((green & 0xFF) << 16) | ((blue & 0xFF) << 8) |
                         ((u32)(alpha * fade) & 0xFF);
            }
            Hud_DrawSprite(item->m_Icon, 0, x - width * item->m_Size * scaleX * 0.5f,
                          y - height * item->m_Size * scaleY * 0.5f, scaleX * item->m_Size,
                          scaleY * item->m_Size, 1);
        }
        item++;
    }
}

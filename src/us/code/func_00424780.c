#include "common.h"
#include "gfx.h"
#include "structs/menu.h"

extern s32 D_800C1BD0;       /* current draw mode */

s32 CPlayer__GetPickupCountHigh(u8 *player, s32 type);
void Hud_SetupTextureMode(s32 alpha);
void Hud_DrawSprite(s32 image, s32 frame, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 arg6);

/* Draws an inventory item's icon, full colour when the player has it, greyed out otherwise. */
void InventoryMenu_DrawItem(MenuItem *item, MenuItemLayout *box, s32 unused, Menu *window) {
    s32 type = item->def->param;
    s32 index = item->def->index;
    u8 *player = window->player;
    s32 owned = 0;

    switch (type) {
        default:
            owned = index < CPlayer__GetPickupCountHigh(player, type);
            break;
        case 0x10FE:
            if (index >= 0) {
                if (index < 3) {
                    goto first_row;
                }
                if (index < 6) {
                    goto second_row;
                }
            }
            break;
        first_row:
            owned = index < player[0xA05];
            break;
        second_row:
            owned = index - 3 < player[0xA06];
            break;
        case 0x1194:
            owned = (player + index)[0x9FC] != 0;
            break;
    }
    if (D_800C1BD0 != 0x14) {
        D_800C1BD0 = 0x14;
        Hud_SetupTextureMode((s32) (window->fade * (window->alpha * 255.0f)));
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3000A01;
        g->w1 = 0x100000;
    }
    if (owned) {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xFB000000;
        g->w1 = ((s32) (window->fade * (window->alpha * 255.0f)) & 0xFF) | 0xFFFFFF00;
    } else {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xFB000000;
        g->w1 = ((s32) (window->fade * (window->alpha * 255.0f)) & 0xFF) | 0x32323200;
    }
    Hud_DrawSprite(item->image, 0, (s16) box->left, (s16) box->top, box->scaleX, box->scaleY, 1);
}

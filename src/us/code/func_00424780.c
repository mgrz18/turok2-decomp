#include "common.h"
#include "structs/menu.h"

typedef struct {
    u32 w0;
    u32 w1;
} GfxWords;

extern GfxWords *D_800D8E24; /* display list head */
extern s32 D_800C1BD0;       /* current draw mode */

s32 func_00253748(u8 *player, s32 type);
void func_00279028(s32 alpha);
void func_0027B320(s32 image, s32 frame, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 arg6);

/* Draws an inventory item's icon, full colour when the player has it, greyed out otherwise. */
void func_00424780(MenuItem *item, MenuItemLayout *box, s32 unused, Menu *window) {
    s32 type = item->def->param;
    s32 index = item->def->index;
    u8 *player = window->player;
    s32 owned = 0;

    switch (type) {
        default:
            owned = index < func_00253748(player, type);
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
        func_00279028((s32) (window->fade * (window->alpha * 255.0f)));
    }
    {
        GfxWords *g = D_800D8E24++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        GfxWords *g = D_800D8E24++;
        g->w0 = 0xE3000A01;
        g->w1 = 0x100000;
    }
    if (owned) {
        GfxWords *g = D_800D8E24++;
        g->w0 = 0xFB000000;
        g->w1 = ((s32) (window->fade * (window->alpha * 255.0f)) & 0xFF) | 0xFFFFFF00;
    } else {
        GfxWords *g = D_800D8E24++;
        g->w0 = 0xFB000000;
        g->w1 = ((s32) (window->fade * (window->alpha * 255.0f)) & 0xFF) | 0x32323200;
    }
    func_0027B320(item->image, 0, (s16) box->left, (s16) box->top, box->scaleX, box->scaleY, 1);
}

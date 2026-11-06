/* Item availability test (also inlined into InventoryMenu_DrawItem). The two row tests jump out to their bodies (gotos), which gives the ROM bnez/bnel + j layout. */
#include "common.h"
#include "structs/menu.h"

s32 CPlayer__GetPickupCountHigh(u8 *player, s32 type);

s32 InventoryMenu_HasItem(MenuItem *item, Menu *event) {
    s32 type = item->def->param;
    s32 index = item->def->index;
    u8 *player = event->player;
    s32 result = 0;

    switch (type) {
        default:
            result = index < CPlayer__GetPickupCountHigh(player, type);
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
            result = index < player[0xA05];
            break;
        second_row:
            result = index - 3 < player[0xA06];
            break;
        case 0x1194:
            result = (player + index)[0x9FC] != 0;
            break;
    }
    return result;
}

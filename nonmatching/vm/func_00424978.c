/* NON-MATCHING: 24 of 38 words differ: the 0x10FE branch tests come out inverted and type stays in a1 instead of a3 */
#include "common.h"

s32 func_00253748(u8 *player, s32 type);

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 type;
    /* 0x22 */ s16 index;
} ItemDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ ItemDef *def;
} MenuItem;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 *player;
} MenuEvent;

s32 func_00424978(MenuItem *item, MenuEvent *event) {
    u8 *player = event->player;
    s32 type = item->def->type;
    s32 index = item->def->index;
    s32 result = 0;

    switch (type) {
        default:
            result = index < func_00253748(player, type);
            break;
        case 0x10FE:
            switch (index) {
                case 0 ... 2:
                    result = index < player[0xA05];
                    break;
                case 3 ... 5:
                    result = index - 3 < player[0xA06];
                    break;
            }
            break;
        case 0x1194:
            result = player[index + 0x9FC] != 0;
            break;
    }
    return result;
}

typedef signed int s32;

typedef struct {
    /* 0x000 */ char pad0[0x2D8];
    /* 0x2D8 */ s32 flags;
} Player41A4D4;

typedef struct {
    /* 0x00 */ char pad0[0xC];
    /* 0x0C */ Player41A4D4 *player;
} Menu41A4D4;

extern s32 D_8011AADC; /* owned weapons */
extern s32 D_80130924; /* selected weapon */

void func_0041A4D0(Menu41A4D4 *menu) {
    s32 weapon = 0;
    s32 tries;
    s32 mask;

    for (tries = weapon; tries < 11; tries++) {
        switch (weapon) {
            default:
            case 0:
                mask = 0x10000;
                break;
            case 1:
                mask = 0x20000;
                break;
            case 2:
                mask = 0x40000;
                break;
            case 3:
                mask = 0x80000;
                break;
            case 4:
                mask = 0x100000;
                break;
            case 5:
                mask = 0x200000;
                break;
            case 6:
                mask = 0x800000;
                break;
            case 7:
                mask = 0x400000;
                break;
            case 8:
                mask = 0x1000000;
                break;
            case 9:
                mask = 0x2000000;
                break;
            case 10:
                mask = 0x4000000;
                break;
        }
        if (D_8011AADC & mask) {
            goto found;
        }
        weapon++;
        if (weapon >= 11) {
            weapon = 0;
        } else if (weapon < 0) {
            weapon = 10;
        }
    }
    weapon = -1;
found:
    D_80130924 = weapon;
    if (weapon != -1) {
        menu->player->flags |= 0x1800000;
    } else {
        menu->player->flags &= ~0x1800000;
    }
}

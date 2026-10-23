typedef signed int s32;

typedef struct {
    /* 0x00 */ char pad0[0x24];
    /* 0x24 */ s32 enabled;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 team0;
    /* 0x30 */ s32 team1;
} Score8011ACB0;

extern Score8011ACB0 D_8011ACB0;

#define TEAM(p) (*(s32 *) ((char *) (p) + 0xBF4))

void func_00256804(char *player, char *other, s32 event) {
    Score8011ACB0 *score = &D_8011ACB0;
    s32 *count;

    if (score->enabled == 0) {
        return;
    }
    switch (TEAM(player)) {
        case 0:
            count = &score->team0;
            break;
        case 1:
            count = &score->team1;
            break;
        default:
            return;
    }
    switch (event) {
        case 0:
            (*count)--;
            break;
        case 1:
            if (TEAM(other) != TEAM(player)) {
                (*count)++;
            }
            break;
        case 2:
            if (TEAM(other) != TEAM(player)) {
                (*count)++;
            }
            break;
    }
}

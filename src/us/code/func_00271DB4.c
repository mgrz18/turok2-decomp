typedef int s32;
typedef short s16;

typedef struct {
    s32 id;                     /* 0x00 */
    char pad4[0xCC - 0x4];
} Slot;                         /* size 0xCC */

typedef struct {
    char pad0[0x102];
    s16 unk_102;                /* 0x102 */
    char pad104[0x1E64 - 0x104];
    Slot slots[17];             /* 0x1E64 */
} Player;

s32 func_00271DB4(Player *player, s32 id) {
    s32 count;
    s32 i;

    if ((*(s32 *)((char *)player + 0x2BB8) != 0 && id == 0x44C) || player->unk_102 != -1) {
        return 1;
    }
    count = 0;
    if (id < 0x100) {
        for (i = 0; i < 17; i++) {
            if (player->slots[i].id == id) {
                count++;
            }
        }
        if (count >= 6) {
            return 1;
        }
    }
    return 0;
}

typedef int s32;

typedef struct {
    char pad0[0xC];
    s32 flags;                  /* 0x0C */
} Item;

s32 func_00443D28(void *unused, Item *item) {
    s32 flags = item->flags;
    s32 bit2 = flags & 2;
    s32 result;

    if (flags & 0x20) {
        if (!bit2) {
            result = 0x56C2;
        } else {
            return 0x5640;
        }
    } else if (bit2) {
        result = 0x56B8;
    } else {
        result = 0x56A4;
    }
    return result;
}

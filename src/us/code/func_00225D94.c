#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x84];
    /* 0x84 */ s32 *table;
} TableOwner;

/* Element i of a table whose first word is the element size, data at +8. */
void *func_00225D94(TableOwner *arg0, s32 arg1) {
    s32 *t = arg0->table;
    return (s8 *)t + (arg1 * *t + 8);
}

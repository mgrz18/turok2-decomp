#include "common.h"

typedef struct World {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ s32 *table;
} World;

/* Element i of a table whose first word is the element size, data at +8. */
void *func_00223F70(World *arg0, s32 arg1) {
    s32 *t = arg0->table;
    return (s8 *)t + (arg1 * *t + 8);
}

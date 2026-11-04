#include "common.h"

/* Only the field read here: a table whose first word is the element size,
 * data at +8. */
typedef struct {
    /* 0x00 */ u8 pad0[0x8C];
    /* 0x8C */ s32 *table;
} TableOwner;

/* Element i of the table. */
void *func_00226DCC(TableOwner *arg0, s32 arg1) {
    s32 *t = arg0->table;
    return (s8 *)t + (arg1 * *t + 8);
}

#include "common.h"

/* Only the field read here: a resource walked with func_002017D4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x5C];
    /* 0x5C */ s32 unk5C;
} TableOwner;

void func_002017D4(s32);                            /* extern */

void func_00223F50(TableOwner *arg0) {
    func_002017D4(arg0->unk5C);
}

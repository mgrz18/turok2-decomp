#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} LoopSound;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1288];
    /* 0x1288 */ LoopSound *weaponBackgroundSound;
} Player;

s32 func_0027588C(s32);                         /* extern */
s32 func_00275D34(s32, void *);                 /* extern */
s32 func_00275F7C();                                /* extern */

void func_00253E68(Player *arg0) {
    LoopSound *temp_v0;

    temp_v0 = arg0->weaponBackgroundSound;
    if (temp_v0 != NULL) {
        func_0027588C(temp_v0->id);
    }
    func_00275D34(func_00275F7C(), arg0->weaponBackgroundSound);
    arg0->weaponBackgroundSound = NULL;
}

#include "common.h"
#include "structs/sound.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SndSource *head;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 paused;
} SoundList;

s32 func_002757E4(s32);                             /* extern */

void func_00275E98(SoundList *arg0) {
    SndSource *var_s0;

    if (arg0->paused == 0) {
        var_s0 = arg0->head;
        if (var_s0 != NULL) {
            do {
                if (func_002757E4(var_s0->handle) == 0) {
                    var_s0->sound.w = -1;
                    var_s0->handle = -1;
                }
                var_s0 = var_s0->next;
            } while (var_s0 != NULL);
        }
    }
}

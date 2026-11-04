#include "common.h"

/* A node of the list at 0x14: a sound handle and a second word that are
 * both reset to -1 once the sound is done. */
typedef struct SoundNode {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ struct SoundNode *next;
    /* 0x08 */ s32 handle;
    /* 0x0C */ s32 unkC;
} SoundNode;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SoundNode *head;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 paused;
} SoundList;

s32 func_002757E4(s32);                             /* extern */

void func_00275E98(SoundList *arg0) {
    SoundNode *var_s0;

    if (arg0->paused == 0) {
        var_s0 = arg0->head;
        if (var_s0 != NULL) {
            do {
                if (func_002757E4(var_s0->handle) == 0) {
                    var_s0->unkC = -1;
                    var_s0->handle = -1;
                }
                var_s0 = var_s0->next;
            } while (var_s0 != NULL);
        }
    }
}

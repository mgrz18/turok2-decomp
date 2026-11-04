#include "common.h"
#include "structs/game_object.h"

typedef struct SoundNode {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct SoundNode *next;
    /* 0x08 */ s32 handle;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ s16 sound;
    /* 0x10 */ CVector3 pos;
    /* 0x1C */ s32 flags;
} SoundNode;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SoundNode *sounds;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 playing;
} SoundOwner;

s32 func_00275544(s16, CVector3, s32, s32);    /* extern */
extern s32 D_800B2A8C;

/* Restarts every looping sound of the owner. */
void func_00275E1C(SoundOwner *arg0) {
    s32 temp_v0;
    SoundNode *var_s0;

    var_s0 = arg0->sounds;
    if (var_s0 != NULL) {
        do {
            temp_v0 = func_00275544(var_s0->sound, var_s0->pos, var_s0->flags, -1);
            var_s0->handle = temp_v0;
            var_s0 = var_s0->next;
            D_800B2A8C = temp_v0;
        } while (var_s0 != NULL);
    }
    arg0->playing = 0;
}

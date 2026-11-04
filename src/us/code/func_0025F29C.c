#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad4[0x34 - 0x4];
    /* 0x034 */ u8 mode;
    /* 0x035 */ u8 pad35[0xC8 - 0x35];
    /* 0x0C8 */ u8 intensity; /* 0..255 */
    /* 0x0C9 */ u8 padC9[0x120 - 0xC9];
    /* 0x120 */ f32 unk120;
} AI;


/* AI callback: in modes 0x11-0x12 sets 0x120 from the intensity (0..255 ->
 * 60..15); clears AI flag 0x4000000. */
void func_0025F29C(CGameObjectInstance *self, AI *ai) {
    f32 level;

    if (ai->mode == 0x11 || ai->mode == 0x12) {
        level = ai->intensity;
        level *= (1.0f / 255);
        level = level * -45.0f + 60.0f;
        ai->unk120 = level;
    }
    ai->flags &= ~0x4000000;
}

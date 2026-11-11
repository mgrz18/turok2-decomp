#include "common.h"

typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 body[0x68 - 0x4];
} Slot00403494;

typedef struct {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ Slot00403494 slots[5];
    /* 0x210 */ u8 pad1[0x214 - 0x210];
    /* 0x214 */ s32 *info;
} State00403494;

void ScriptAnimator_SetScript(void *slot, void *arg);

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

/* Host body: the step's weak spot (step + 0x14) and the spot's animator (0x8 + 0x68 * i + 4)
 * at N64 offsets; BossWeakSpot holds pointers on the host. */
void MantidQueen_FlashWeakSpot(void *state, void *arg) {
    WaveBossState *s = (WaveBossState *) state;
    s32 index = s->step->weakSpot;

    if (index != -1) {
        ScriptAnimator_SetScript(&s->spots[index].anim, arg);
    }
}
#else
void MantidQueen_FlashWeakSpot(State00403494 *s, void *arg) {
    s32 index = s->info[5];

    if (index != -1) {
        /* &s->slots[index].body, computed as an address */
        ScriptAnimator_SetScript((void *)(index * sizeof(Slot00403494) + (sptr)s + 0xC), arg);
    }
}
#endif

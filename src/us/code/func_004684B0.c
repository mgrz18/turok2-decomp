#include "common.h"

/* Sequencer state at D_800F56B0 (minimal view). */
typedef struct SeqState {
    /* 0x000 */ s32 active;
    /* 0x004 */ u8 pad4[0x350 - 0x4];
    /* 0x350 */ s32 index;
} SeqState;

extern SeqState D_800F56B0;
extern void *D_00438408[]; /* NULL-terminated list */
extern void func_0042EE4C(void *entry);

void func_004684B0(void) {
    SeqState *s = &D_800F56B0;

    if (s->active) {
        func_0042EE4C(D_00438408[s->index]);
        s->index++;
        if (D_00438408[s->index] == NULL) {
            s->index = 0;
        }
    }
}

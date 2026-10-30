#include "common.h"

/* Picks the module object's idle anim for the current phase (0, 1; none in phase 2) from the AI's flags. */

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 flags;
} CAI;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

extern s32 D_801301CC; /* module state +0x65C: phase */
extern s16 func_0026D518(Pick *table);

s16 func_0040A170(void *obj, CAI *ai) {
    Pick picks[8];
    s32 n = 0;

    switch (D_801301CC) {
        case 0:
            if (ai->flags & 0x1C) {
                picks[n].id = 0x55FA;
                picks[n++].weight = 4;
            }
            if (ai->flags & 0x1C0) {
                picks[n].id = 0x5604;
                picks[n++].weight = 4;
            }
            break;
        case 1:
            if (ai->flags & 2) {
                picks[n].id = 0x5654;
                picks[n++].weight = 4;
            }
            if (ai->flags & 0x20) {
                picks[n].id = 0x565E;
                picks[n++].weight = 4;
            }
            break;
        case 2:
            break;
    }
    picks[n].id = -1;
    return func_0026D518(picks);
}

/* Second VM module copy of Demo_UpdateStreams (same source, linked 0xE0 lower); the per-timer calls are inline helpers, which keeps GCC from strength-reducing &s->buttons[i]. */
#include "common.h"
#include "structs/demo.h"


/* One per player, 0x224 bytes apart. */
typedef struct Slot {
    /* 0x00 */ s32 a;
    /* 0x04 */ u8 pad4[0x12];
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 c;
    /* 0x18 */ u8 pad18[0x224 - 0x18];
} Slot;

void Demo_Update_2(void);
extern Slot gControllers_buttons[];
static __inline__ void fire(DemoStream *t) { t->decode(t); }
static __inline__ void tick(DemoStream *t) { if (--t->frames <= 0) t->decode(t); }
void Demo_UpdateStreams_2(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    if (s->fastForward != 0) {
        for (i = 0; i < s->playerCount; i++) {
            fire(&s->buttons[i]);
            fire(&s->stickX[i]);
            fire(&s->stickY[i]);
        }
    } else {
        for (i = 0; i < s->playerCount; i++) {
            tick(&s->buttons[i]);
            tick(&s->stickX[i]);
            tick(&s->stickY[i]);
        }
    }
    for (i = 0; i < s->playerCount; i++) {
        if (s->done != 0) {
            gControllers_buttons[i].a = 0;
            gControllers_buttons[i].b = 0;
            gControllers_buttons[i].c = 0;
        } else {
            gControllers_buttons[i].a = s->buttons[i].value;
            gControllers_buttons[i].b = s->stickX[i].value;
            gControllers_buttons[i].c = s->stickY[i].value;
        }
    }
    Demo_Update_2();
}

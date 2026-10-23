#include "common.h"

typedef struct LevelActor {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 *def;
    /* 0x018 */ u8 pad18[0x144 - 0x18];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x28C - 0x148];
    /* 0x28C */ struct LevelActor *next;
} LevelActor;

extern LevelActor *D_8010FEB4;

/* Is any actor whose definition kind is 1 still alive? */
s32 func_00408E64(void) {
    LevelActor *actor;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        if (*actor->def == 1 && actor->health != 0) {
            return 1;
        }
    }
    return 0;
}

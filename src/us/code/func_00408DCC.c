#include "common.h"
#include "structs/game_object.h"

/* A level actor (0x290 bytes), only the fields used here. */
typedef struct LevelActor {
    /* 0x000 */ u8 pad000[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad010[0x4];
    /* 0x014 */ s32 *def;                     /* def[0] is the actor kind; 1 = player */
    /* 0x018 */ u8 pad018[0x128];
    /* 0x140 */ u32 aiFlags;                  /* CAI at 0x140 */
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x70];
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ u8 pad1BC[0x4];
    /* 0x1C0 */ void *target;
    /* 0x1C4 */ u8 pad1C4[0x4];
    /* 0x1C8 */ void *targetOverride;
    /* 0x1CC */ u8 pad1CC[0xC0];
    /* 0x28C */ struct LevelActor *next;
} LevelActor;

extern LevelActor *D_8010FEB4;  /* active actors */

extern u8 *D_80119890;          /* current player */

void func_00408DCC(void) {
    LevelActor *actor;

    *(f32 *) (D_80119890 + 0xAAC) = 450.0f;
    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        if (*actor->def == 1 && actor->health != 0) {
            actor->targetOverride = NULL;
            actor->target = NULL;
            actor->aiFlags |= 0x20000000;
            actor->aiFlags |= 0x40000000;
        }
    }
}

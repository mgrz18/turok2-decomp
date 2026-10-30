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

typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ s16 limit;
} TargetDef;

typedef struct {
    /* 0x0 */ void *target;
    /* 0x4 */ TargetDef *def;
    /* 0x8 */ s32 mode;
} TargetInfo;

extern void *D_80119890;        /* current player */

/* Point the player actors at a target (mode 0), or back at the player. */
void func_00408C10(TargetInfo *info, s32 force) {
    LevelActor *actor;
    void *player;
    s32 count;

    if (info->mode == 1) {
        return;
    }
    actor = D_8010FEB4;
    player = D_80119890;
    count = 0;
    for (; actor != NULL; actor = actor->next) {
        if (*actor->def != 1 || actor->unk1B8 != 0) {
            continue;
        }
        if (info->mode == 0) {
            if (count < info->def->limit || force) {
                count++;
                actor->targetOverride = player;
                actor->target = player;
                actor->aiFlags |= 0x20000000;
                actor->aiFlags |= 0x40000000;
            } else {
                actor->target = info->target;
                actor->targetOverride = info->target;
                actor->aiFlags |= 0x20000000;
                actor->aiFlags |= 0x40000000;
            }
        } else if (actor->target == info->target) {
            actor->targetOverride = player;
            actor->target = player;
            actor->aiFlags |= 0x20000000;
            actor->aiFlags |= 0x40000000;
        }
    }
}

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
    u8 data[0x18];
} DamageEvent;

extern f32 func_0021149C(f32 scale);                                      /* random 0..scale */
extern void func_00246690(void *actor, s32 event, CVector3 pos, s32 arg);
extern void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void func_0025B2D4(LevelActor *actor, void *ai, DamageEvent *event);

/* One time in four, hurt the first live player actor. */
void func_00408CF4(void) {
    DamageEvent event;
    LevelActor *actor;

    if (func_0021149C(100.0f) > 25.0f) {
        return;
    }
    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        if (*actor->def == 1 && actor->health != 0) {
            func_00246690(actor, 0x17C, actor->pos, 0);
            func_00245828(&event, NULL, 0xFF00, 51.199997f, 0x100000, NULL);
            func_0025B2D4(actor, &actor->aiFlags, &event);
            return;
        }
    }
}

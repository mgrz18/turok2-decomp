typedef int s32;

typedef struct Actor {
    char pad0[0x14];
    s32 *def;                   /* 0x14: first word is the kind */
    char pad18[0x144 - 0x18];
    s32 health;                 /* 0x144 */
    char pad148[0x28C - 0x148];
    struct Actor *next;         /* 0x28C */
} Actor;

extern Actor *D_8010FEB4;

s32 func_00441E64(void) {
    Actor *actor;

    for (actor = D_8010FEB4; actor != 0; actor = actor->next) {
        if (*actor->def == 1 && actor->health != 0) {
            return 1;
        }
    }
    return 0;
}

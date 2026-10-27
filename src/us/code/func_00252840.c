#include "common.h"

/* Spawn point record from func_00225EBC: a position and a starting health. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 health;
    /* 0x1C */ u8 pad1C[0x38 - 0x1C];
    /* 0x38 */ f32 pos[3];
} SpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x14];
    /* 0x0014 */ SpawnPoint *spawn;
    /* 0x0018 */ u8 pad18[0x34 - 0x18];
    /* 0x0034 */ f32 pos[3];
    /* 0x0040 */ u8 pad40[0x144 - 0x40];
    /* 0x0144 */ s32 actorHealth;
    /* 0x0148 */ u8 pad148[0x151 - 0x148];
    /* 0x0151 */ s8 skin;
    /* 0x0152 */ u8 pad152[0x518 - 0x152];
    /* 0x0518 */ PlayerOptions *options;
    /* 0x051C */ u8 pad51C[4];
    /* 0x0520 */ s32 character;
    /* 0x0524 */ s32 health;
    /* 0x0528 */ u8 pad528[0x1320 - 0x528];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

extern u8 D_800F7078[];
extern SpawnPoint *func_00225EBC(void *spawns, s32 kind, s32 character);
extern s32 func_002113A4(void);

/* Same as func_00254044. */
static __inline__ void place_at_spawn(Player *p) {
    SpawnPoint *sp;
    s32 character = p->character;

    if (p->options->tagMode == 1) {
        character = 11;
    }
    sp = func_00225EBC(D_800F7078, 11, character);
    if (sp == NULL) {
        sp = func_00225EBC(D_800F7078, 11, -1);
        if (sp == NULL) {
            sp = func_00225EBC(D_800F7078, -1, -1);
        }
    }
    p->spawn = sp;
    p->pos[0] = sp->pos[0];
    p->pos[1] = sp->pos[1];
    p->pos[2] = sp->pos[2];
}

/* Respawn every player at a spawn point, with full health, out of tag mode.
 * A goto loop: as a for/do loop GCC hoists the constant 1 out of it. */
void func_00252840(PlayerList *list) {
    Player *p;

    p = list->head;
    if (p == NULL) return;
loop:
    {
        p->options->tagMode = 0;
        place_at_spawn(p);
        p->health = p->spawn->health << 8;
        p->actorHealth = p->spawn->health << 8;
        p->skin = (p->character != 7) ? -1 : func_002113A4() % 18;
        p = p->next;
        if (p != NULL) goto loop;
    }
}

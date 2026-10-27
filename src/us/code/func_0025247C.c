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

/* CBarrel, the weapon wheel (opaque in structs/player.h). */
typedef struct {
    /* 0x000 */ s32 field0;
    /* 0x004 */ s32 field4;
    /* 0x008 */ s32 field8;
    /* 0x00C */ u8 padC[0x37C - 0xC];
    /* 0x37C */ s32 field37C;
    /* 0x380 */ u8 pad380[0x394 - 0x380];
} Barrel;

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
    /* 0x0528 */ u8 pad528[0xCD0 - 0x528];
    /* 0x0CD0 */ Barrel barrel;
    /* 0x1064 */ u8 pad1064[0x1284 - 0x1064];
    /* 0x1284 */ s32 headMovementDisabled;
    /* 0x1288 */ u8 pad1288[0x1320 - 0x1288];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

extern u8 D_800F7078[];
extern SpawnPoint *func_00225EBC(void *spawns, s32 kind, s32 character);
extern s32 func_002113A4(void);
extern void func_00275624(s32 sound);
extern void func_00243414(void *weapon, void *weaponAI, s32 arg2);

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

/* Put the player into tag mode ("it"): a sound, the weapon wheel and the
 * weapon reset, then respawn with full health. */
void func_0025247C(Player *p) {
    Barrel *barrel;

    p->options->tagMode = 1;
    func_00275624(0x27D8);
    barrel = &p->barrel;
    barrel->field0 = 0;
    barrel->field4 = 0;
    barrel->field37C = -1;
    barrel->field8 = 0;
    p->headMovementDisabled = 0;
    func_00243414((u8 *)p + 0x288, (u8 *)p + 0x3C8, 1);
    place_at_spawn(p);
    p->health = p->spawn->health << 8;
    p->actorHealth = p->spawn->health << 8;
    p->skin = (p->character != 7) ? -1 : func_002113A4() % 18;
}

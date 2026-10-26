/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/CODE_80289790.CPP, Scene_UpdateSelectionTransition, with Turok 2's layouts. */

#include "common.h"

typedef struct ColorEntry {
    /* 0x00 */ f32 value;               /* the fade's length */
    /* 0x04 */ u8 pad4[4];
} ColorEntry;

typedef struct Spawn {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 kind;
    /* 0x12 */ u8 pad12[2];
} Spawn;

/* The world: what this reads of it. */
typedef struct World {
    /* 0x00000 */ u8 pad0[0x18FAC];
    /* 0x18FAC */ s32 selectionState;
    /* 0x18FB0 */ s32 frozen;
    /* 0x18FB4 */ f32 selectionTimer;
    /* 0x18FB8 */ s32 selectionFlags;
    /* 0x18FBC */ Spawn selectionSpawn;
    /* 0x18FD0 */ s32 selectionValues[2];
    /* 0x18FD8 */ u8 pad18FD8[0x18FE8 - 0x18FD8];
    /* 0x18FE8 */ s32 previewStart;
} World;

typedef struct MenuSettings {
    /* 0x00 */ s32 pad0;
    /* 0x04 */ s32 optionFlags;
} MenuSettings;

/* The global at D_801198B0: what this reads of it. */
typedef struct Game {
    /* 0x0000 */ u8 pad0[0x40];
    /* 0x0040 */ u8 fallbackCamera[0x1228 - 0x40];
    /* 0x1228 */ MenuSettings settings;    /* D_8011AAD8 */
} Game;

extern Game D_801198B0;
extern ColorEntry D_800ACDE8[];
extern f32 D_800B6D28;                  /* the frame's time step */
extern u8 D_800B54E0[];

void func_0028441C(ColorEntry *entry);
void func_00412B14(void);
void func_00412438(s32 arg0);
s32 func_0042C194(World *world, s32 arg1, s32 id, Spawn *spawn, s32 arg4);
void func_0042AC0C(World *world, s32 id, s32 flags);
s32 func_0027BC34(void);
void func_00281610(Game *game, void *camera, u8 *text, f32 time);

static __inline__ s32 transitionId(World *scene) {
    if (scene->selectionFlags != 0) {
        return scene->selectionSpawn.kind;
    }
    return scene->selectionValues[1];
}

/* Runs the fade of a level transition: state 1 fades in to the entry length and then warps
 * (func_0042C194, or straight to ~id for a negative id), state 2 fades back out. */
void func_002237C4(World *scene) {
    ColorEntry *entry = &D_800ACDE8[scene->selectionState];
    s32 id;
    MenuSettings *settings;

    switch (scene->frozen) {
        case 0:
            break;
        case 1:
            if (scene->selectionTimer == entry->value) {
                func_0028441C(entry);
                if (scene->selectionValues[1] == 999) {
                    func_00412B14();
                    func_00412438(400);
                }
                if (scene->selectionState == 0) {
                    scene->frozen = 0;
                } else {
                    scene->frozen = 2;
                }
                if (scene->selectionValues[1] < 0) {
                    scene->selectionValues[1] = ~scene->selectionValues[1];
                    scene->selectionFlags = 0;
                } else {
                    scene->selectionFlags = func_0042C194(scene, -1, scene->selectionValues[1], &scene->selectionSpawn, 1);
                }
                scene->selectionValues[0] = scene->selectionValues[1];
                id = transitionId(scene);
                func_0042AC0C(scene, id, scene->selectionFlags);
                if (scene->selectionFlags != 0) {
                    settings = &D_801198B0.settings;
                    if (scene->previewStart != settings->optionFlags && !func_0027BC34()) {
                        func_00281610(&D_801198B0, D_801198B0.fallbackCamera, D_800B54E0, 4.0f);
                        scene->previewStart = settings->optionFlags;
                    }
                }
            } else {
                scene->selectionTimer += D_800B6D28;
                if (entry->value < scene->selectionTimer) {
                    scene->selectionTimer = entry->value;
                }
            }
            break;
        case 2:
            if (scene->selectionTimer == 0.0f) {
                scene->frozen = 0;
            } else {
                scene->selectionTimer -= D_800B6D28;
                if (scene->selectionTimer < 0.0f) {
                    scene->selectionTimer = 0.0f;
                }
            }
            break;
    }
}

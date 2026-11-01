/* Second-module copy of func_0040D5C4 (nonmatching/vm/func_0040D5C4.c), with its data addresses 0xE0 lower. */
#include "common.h"

/* AI init of the second boss: sets the level's warp id, the anim table and callbacks, starts the phase script
 * and shows its health bar ("EyeBall"). */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x24C - 0xD8];
    /* 0x24C */ s32 unk24C;
} LevelActor;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} ScriptStep;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
} Mover;

typedef struct {
    /* 0x00 */ u8 data[0x64];
} Slot2;

struct State2;

typedef struct {
    /* 0x00 */ void (*init)(struct State2 *state);
    /* 0x04 */ void (*update)(struct State2 *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

typedef struct State2 {
    /* 0x000 */ Obj2 *obj;
    /* 0x004 */ s32 count1;
    /* 0x008 */ LevelActor *actors1[4];
    /* 0x018 */ s32 count2;
    /* 0x01C */ LevelActor *actors2[4];
    /* 0x02C */ s32 count3;
    /* 0x030 */ LevelActor *actors3[3];
    /* 0x03C */ Slot2 slots[12];
    /* 0x4EC */ s32 slotCount;
    /* 0x4F0 */ u8 pad4F0[0x4F8 - 0x4F0];
    /* 0x4F8 */ ScriptStep *script;
    /* 0x4FC */ s32 phase;
    /* 0x500 */ u8 pad500[0x518 - 0x500];
    /* 0x518 */ Mover movers[3];
} State2;

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ void *anims;
    /* 0x030 */ u8 pad30[0xF0 - 0x30];
    /* 0x0F0 */ void *callbackF0;
    /* 0x0F4 */ void *callbackF4;
    /* 0x0F8 */ void *callbackF8;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ u8 pad110[0x118 - 0x110];
    /* 0x118 */ f32 hitTimer;
} CAI;

extern State2 D_80130200;
extern s32 D_80110048[]; /* warp id */
extern u8 D_800C1910[];
extern ScriptStep D_00432260[];
extern u8 D_801100F0[];
extern void func_0040DA90(void);
extern void func_0040D9B8(void);
extern void func_0040EDB0(void);
extern void func_0040EC14(State2 *state, s32 id);
extern void func_00243414(Actor *obj, CAI *ai, s32 state);
extern void func_0027AD24(void *hud, Actor *obj, char *name);

/* Starts the boss script: its first step runs at once. */
static __inline__ void StartScript(State2 *state, ScriptStep *script, Actor *obj) {

    state->script = script;
    D_80130200.obj = (Obj2 *)obj;
    func_0040EC14(state, D_80130200.script->id);
}

void func_004465C4(Actor *obj, CAI *ai) {
    D_80110048[0] = 0x1CE9;
    ai->anims = D_800C1910;
    ai->callbackF0 = func_0040DA90;
    ai->callbackF8 = func_0040D9B8;
    ai->callbackF4 = func_0040EDB0;
    StartScript(&D_80130200, D_00432260, obj);
    obj->flags &= ~0x10000000;
    ai->unk10C = 0;
    ai->hitTimer = 0.0f;
    func_00243414(obj, ai, 1);
    func_0027AD24(D_801100F0, obj, "EyeBall");
}

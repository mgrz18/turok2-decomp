#include "common.h"

typedef struct DefInfo {
    char pad0[4];
    u16 kind;          /* 0x04 */
} DefInfo;

typedef struct Def {
    char pad0[0x14];
    DefInfo info;      /* 0x14 */
} Def;

typedef struct Actor {
    char pad0[0x14];
    Def *def;          /* 0x14 */
} Actor;

typedef struct Target {
    char pad0[4];
    s8 active;         /* 0x04 */
} Target;

typedef struct Controller {
    char pad0[4];
    Target *target;    /* 0x04 */
    char pad8[0x34 - 0x8];
    void *a;           /* 0x34 */
    void *b;           /* 0x38 */
    s32 unk3C;         /* 0x3C */
    s32 unk40;         /* 0x40 */
    void *c;           /* 0x44 */
    Actor *actor;      /* 0x48 */
} Controller;

extern Target D_800F1E10;
extern char *D_800F1ADC;

s32 func_00241E30(Controller *ctrl, Actor *actor);
s32 func_00242284(Controller *ctrl, Actor *actor);
s32 func_00241FC8(Controller *ctrl, Actor *actor);

void func_00242604(Controller *ctrl, Actor *actor) {
    DefInfo *info;
    Target *target;
    s32 ok;

    info = &actor->def->info;
    target = ctrl->target;
    if (target != &D_800F1E10 && target->active != 0) {
        switch (info->kind) {
            case 0:
                ok = func_00241E30(ctrl, actor);
                break;
            case 1:
                ok = func_00242284(ctrl, actor);
                break;
            case 2:
                ok = func_00241FC8(ctrl, actor);
                break;
            default:
                ok = 0;
                break;
        }
        if (ok != 0) {
            ctrl->unk3C = 0;
            ctrl->unk40 = 0;
            ctrl->actor = actor;
            ctrl->a = D_800F1ADC + 0x90;
            ctrl->b = D_800F1ADC + 0x8C;
            ctrl->c = D_800F1ADC + 0x88;
        }
    }
}

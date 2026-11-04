#include "common.h"
#include "structs/game_object.h"


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
    CGameObjectInstance *actor;      /* 0x48 */
} Controller;

extern Target D_800F1E10;
extern char *D_800F1ADC;

s32 func_00241E30(Controller *ctrl, CGameObjectInstance *actor);
s32 func_00242284(Controller *ctrl, CGameObjectInstance *actor);
s32 func_00241FC8(Controller *ctrl, CGameObjectInstance *actor);

void func_00242604(Controller *ctrl, CGameObjectInstance *actor) {
    CPlatformIntelligence *info;
    Target *target;
    s32 ok;

    info = &actor->header.m_pIntelligence->m_Platform;
    target = ctrl->target;
    if (target != &D_800F1E10 && target->active != 0) {
        switch (info->m_CollisionType) {
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

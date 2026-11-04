#include "common.h"
#include "structs/game_object.h"

/* Per-part callback of the module's actors: in part 0, fades the actor's two blend values in with the module
 * object's anim state (9: first value up to 1; 0x13: after a delay, second value up to 3 then back down to 1;
 * from 30 on: second value 1); the other parts apply one of the values to the part. */

typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 animState;
} ModuleObj;

typedef struct {
    /* 0x000 */ ModuleObj *obj;
} State;


typedef struct {
    /* 0x00 */ CGameObjectInstance *actor;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ u32 part;
} PartInfo;

extern State D_8012FB70;
extern f32 D_800B6D28; /* frame time */
extern f32 D_800C1900; /* fade timer */
extern s32 D_800C1904; /* second value reached its peak */
extern void func_0021034C(void *arg0, f32 r, f32 g, f32 b);

void func_0040A25C(void *arg0, PartInfo *info) {
    CGameObjectInstance *actor = info->actor;
    ModuleObj *obj;
    f32 v;
    f32 t;
    f32 dt;

    switch (info->part) {
        case 0:
            obj = D_8012FB70.obj;
            if (obj->animState == 9) {
                v = (*(f32 *)&actor->m_AI.m_VarBuffer[2]) + D_800B6D28 * 0.01f;
                if (v > 1.0f) {
                    v = 1.0f;
                }
                (*(f32 *)&actor->m_AI.m_VarBuffer[2]) = v;
            }
            if (obj->animState == 0x13) {
                t = D_800C1900;
                if (t < 8.8f) {
                    D_800C1900 = t + D_800B6D28 * (1.0f / 15);
                } else if (D_800C1904 == 0) {
                    dt = D_800B6D28;
                    (*(f32 *)&actor->m_AI.m_VarBuffer[1]) += dt * 0.4f;
                    if ((*(f32 *)&actor->m_AI.m_VarBuffer[1]) > 3.0f) {
                        D_800C1904 = 1;
                    }
                    D_800C1900 = t + dt * (1.0f / 15);
                } else {
                    v = (*(f32 *)&actor->m_AI.m_VarBuffer[1]) - D_800B6D28 * 0.12f;
                    if (v < 1.0f) {
                        v = 1.0f;
                    }
                    (*(f32 *)&actor->m_AI.m_VarBuffer[1]) = v;
                }
            }
            if (obj->animState >= 30) {
                (*(f32 *)&actor->m_AI.m_VarBuffer[1]) = 1.0f;
            }
            break;
        case 21:
        case 26:
        case 31:
        case 36:
        case 46:
        case 51:
        case 56:
        case 61:
            func_0021034C(arg0, (*(f32 *)&actor->m_AI.m_VarBuffer[2]), (*(f32 *)&actor->m_AI.m_VarBuffer[2]), (*(f32 *)&actor->m_AI.m_VarBuffer[2]));
            break;
        case 89:
        case 93:
        case 97:
        case 101:
        case 105:
        case 109:
            func_0021034C(arg0, (*(f32 *)&actor->m_AI.m_VarBuffer[1]), (*(f32 *)&actor->m_AI.m_VarBuffer[1]), (*(f32 *)&actor->m_AI.m_VarBuffer[1]));
            break;
    }
}

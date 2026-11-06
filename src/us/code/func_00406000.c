/* First-module copy of Primagen_UpdateMode19_2 (src/us/code/Primagen_UpdateMode19_2.c): a local pointer to the 0x8012F9D4 block. */
#include "common.h"
#include "structs/game_object.h"
#define BOSS_STAGE_AS_WORD
#include "structs/boss.h"
#include "globals/boss.h"

/* LevelScriptState (structs/boss.h) seen from 0x8012F604, one word before it
 * (offsets below are from 0x8012F608): the ROM addresses it through D_8012F604. */
typedef struct {
    /* -0x04 */ s32 padF604;
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad004[0x8];
    /* 0x00C */ f32 speed;                  /* 0x8012F614 */
    /* 0x010 */ u8 pad010[0x35C];
    /* 0x36C */ CGameObjectInstance *partA; /* 0x8012F974 */
    /* 0x370 */ CGameObjectInstance *partB; /* 0x8012F978 */
    /* 0x374 */ u8 pad374[0x58];
    /* 0x3CC */ s32 unk3CC;                 /* 0x8012F9D4 */
    /* 0x3D0 */ s32 state;                  /* 0x8012F9D8 */
    /* 0x3D4 */ s32 mode;
    /* 0x3D8 */ u8 pad3D8;
    /* 0x3D9 */ u8 scriptOn;                /* 0x8012F9E1 */
    /* 0x3DA */ u8 pad3DA[0x1A];
    /* 0x3F4 */ BossStep *script;           /* 0x8012F9FC */
    /* 0x3F8 */ s32 cursor;                 /* 0x8012FA00 */
    /* 0x3FC */ s32 index;                  /* 0x8012FA04 */
    /* 0x400 */ u8 pad400[0x28];
    /* 0x428 */ CVector3 dest;              /* 0x8012FA30 */
} LevelScriptF604;

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);                                    /* heading */
extern f32 Math_StepClamped(f32 value, f32 target, f32 rate);
extern f32 func_00298470(f32 x);                                          /* sqrt */
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */
extern f32 Random_Blend(f32 lo, f32 hi);
extern s32 Primagen_RunAttackStep(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
extern CVector3 *gPrimagenPaths[];  /* boss paths, ended by a point with y < 0 */
extern s32 gLevelScript_path;          /* path */
extern LevelScriptF604 D_8012F604;
typedef struct {
    /* 0x00 */ s32 unk0;   /* 0x8012F9D4 */
    /* 0x04 */ s32 state;  /* 0x8012F9D8 */
    /* 0x08 */ u8 pad8[0x5C - 0x8];
    /* 0x5C */ CVector3 dest; /* 0x8012FA30 */
} BossCtl;
extern BossCtl gLevelScript_unk3CC;
extern s32 sPrimagenPathPoint;          /* point on the path */
extern f32 gLevelScript_script;          /* cooldown */
extern BossStep *gLevelScript_script_steps;    /* script */
extern s32 gLevelScript_script_cursor;          /* cursor */
extern s32 gLevelScript;          /* stage, as the script selection reads it */

void Primagen_UpdateMode19(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 d;
    CVector3 dest;
    CGameObjectInstance *target;
    f32 angle;
    f32 speed;
    f32 heading;
    BossCtl *ctl = &gLevelScript_unk3CC;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    ctl->unk0 = 3;
    D_800F1F50[0] &= ~0x8000;
    do {
        target = t.m_Sight.m_pTarget;
        angle = 0.0f;
    } while (0);
    if (target != NULL) {
        if (ctl->state == 4) {
            dest = target->header.m_vPos;
            angle = AI_AngleToPoint(obj, dest);
        }
        dest = ctl->dest;
        if (ctl->state == 3 || ctl->state == 11) {
            angle = AI_AngleToPoint(obj, dest);
        }
        CVector3__Subtract(&d, &obj->header.m_vPos, &dest);
        if (func_00298470(d.x * d.x + d.z * d.z) <= 50.0f) {
            if (D_8012F604.state == 3) {
                if (gLevelScript == 1) {
                    D_8012F604.script = sPrimagenAttackScriptStage1;
                }
                if (gLevelScript == 2) {
                    D_8012F604.script = sPrimagenAttackScriptStage2;
                }
                if (gLevelScript == 3) {
                    D_8012F604.script = sPrimagenAttackScriptStage3;
                }
                D_8012F604.cursor = -1;
                D_8012F604.index = -1;
                CAI__SetMode(D_8012F604.partB, &D_8012F604.partB->m_AI, 0);
                CAI__SetMode(D_8012F604.partA, &D_8012F604.partA->m_AI, 0);
                CAI__SetMode(obj, ai, 0x1A);
                return;
            }
            if (D_8012F604.state == 11) {
                if (gLevelScript == 1) {
                    D_8012F604.script = sPrimagenAttackScriptStage1;
                }
                if (gLevelScript == 2) {
                    D_8012F604.script = sPrimagenAttackScriptStage2;
                }
                if (gLevelScript == 3) {
                    D_8012F604.script = sPrimagenAttackScriptStage3;
                }
                D_8012F604.cursor = -1;
                D_8012F604.index = -1;
                CAI__SetMode(obj, ai, 0x17);
                return;
            }
            if (D_8012F604.state == 4) {
                sPrimagenPathPoint++;
                if (gPrimagenPaths[gLevelScript_path][sPrimagenPathPoint].y < 0.0f) {
                    sPrimagenPathPoint = 0;
                    if (D_8012F604.stage != 1) {
                        gLevelScript_path++;
                        if (gLevelScript_path >= 3) {
                            gLevelScript_path = 0;
                        }
                    }
                }
                D_8012F604.dest = gPrimagenPaths[gLevelScript_path][sPrimagenPathPoint];
            }
        }
    }
    speed = 0.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, angle);
    dest = D_8012F604.dest;
    CVector3__Subtract(&d, &obj->header.m_vPos, &dest);
    heading = CVector3__GetYaw(&d);
    if (D_8012F604.state == 3 || D_8012F604.state == 11) {
        if (obj->header.m_vPos.y < 921.6f) {
            obj->m_vVelocity.y = 460.8f;
        } else {
            obj->m_vVelocity.y = 0.001f;
        }
        speed = 500.0f;
        obj->m_vVelocity.x = func_002982F0(heading) * speed;
        obj->m_vVelocity.z = func_002974C0(heading) * speed;
    } else {
        speed = Math_StepClamped(speed, D_8012F604.speed, 5120.0f);
        obj->m_vVelocity.x += speed * func_002982F0(heading);
        obj->m_vVelocity.z += speed * func_002974C0(heading);
        obj->m_vVelocity.y = 0.001f;
    }
    if (D_8012F604.scriptOn && D_8012F604.state != 3 && D_8012F604.state != 11) {
        if (D_8012F604.index == -1) {
            if (gLevelScript_script_cursor != -1) {
                gLevelScript_script = gLevelScript_script_steps[gLevelScript_script_cursor].delay;
            } else {
                gLevelScript_script = 0.0f;
            }
            do {
                gLevelScript_script_cursor++;
                if (gLevelScript_script_steps[gLevelScript_script_cursor].type == 5) {
                    gLevelScript_script_cursor = 0;
                }
                D_8012F604.index = gLevelScript_script_cursor;
            } while ((f32) gLevelScript_script_steps[gLevelScript_script_cursor].chance <= Random_Blend(0.0f, 100.0f));
        }
        Primagen_RunAttackStep(obj, ai, t.m_Sight.m_Dist);
    }
}

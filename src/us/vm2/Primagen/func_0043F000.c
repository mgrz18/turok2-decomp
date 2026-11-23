/* Second-module twin of Primagen_UpdateMode19 (nonmatching/vm; the fix here, a local pointer to the 0x8012F9D4 block, should
 * work there too). Same source, its own strings and pool. */
#include "common.h"
#include "structs/game_object.h"
#define BOSS_STAGE_AS_WORD
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);                                    /* heading */
extern f32 Math_StepClamped(f32 value, f32 target, f32 rate);
extern f32 func_00298470(f32 x);                                          /* sqrt */
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */
extern f32 Random_Blend(f32 lo, f32 hi);
extern s32 Primagen_RunAttackStep_2(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
typedef struct {
    /* 0x00 */ s32 unk0;   /* 0x8012F9D4 */
    /* 0x04 */ s32 state;  /* 0x8012F9D8 */
    /* 0x08 */ u8 pad8[0x5C - 0x8];
    /* 0x5C */ CVector3 dest; /* 0x8012FA30 */
} BossCtl;
extern s32 gLevelScript; /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */          /* stage, as the script selection reads it */

void Primagen_UpdateMode19_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 d;
    CVector3 dest;
    CGameObjectInstance *target;
    f32 angle;
    f32 speed;
    f32 heading;
    BossCtl *ctl = (BossCtl *) &gLevelScript_unk3CC;  /* unk3CC on */

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
            if (gLevelScriptBlock.m_Script.bossState == 3) {
                if (gLevelScript == 1) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage1_2;
                }
                if (gLevelScript == 2) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage2_2;
                }
                if (gLevelScript == 3) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage3_2;
                }
                gLevelScriptBlock.m_Script.script.cursor = -1;
                gLevelScriptBlock.m_Script.script.current = -1;
                CAI__SetMode(gLevelScriptBlock.m_Script.found[1], &gLevelScriptBlock.m_Script.found[1]->m_AI, 0);
                CAI__SetMode(gLevelScriptBlock.m_Script.found[0], &gLevelScriptBlock.m_Script.found[0]->m_AI, 0);
                CAI__SetMode(obj, ai, 0x1A);
                return;
            }
            if (gLevelScriptBlock.m_Script.bossState == 11) {
                if (gLevelScript == 1) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage1_2;
                }
                if (gLevelScript == 2) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage2_2;
                }
                if (gLevelScript == 3) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage3_2;
                }
                gLevelScriptBlock.m_Script.script.cursor = -1;
                gLevelScriptBlock.m_Script.script.current = -1;
                CAI__SetMode(obj, ai, 0x17);
                return;
            }
            if (gLevelScriptBlock.m_Script.bossState == 4) {
                sPrimagenPathPoint++;
                if (gPrimagenPaths[gLevelScript_path][sPrimagenPathPoint].y < 0.0f) {
                    sPrimagenPathPoint = 0;
                    if (gLevelScriptBlock.m_Script.stage != 1) {
                        gLevelScript_path++;
                        if (gLevelScript_path >= 3) {
                            gLevelScript_path = 0;
                        }
                    }
                }
                gLevelScriptBlock.m_Script.dest = gPrimagenPaths[gLevelScript_path][sPrimagenPathPoint];
            }
        }
    }
    speed = 0.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, angle);
    dest = gLevelScriptBlock.m_Script.dest;
    CVector3__Subtract(&d, &obj->header.m_vPos, &dest);
    heading = CVector3__GetYaw(&d);
    if (gLevelScriptBlock.m_Script.bossState == 3 || gLevelScriptBlock.m_Script.bossState == 11) {
        if (obj->header.m_vPos.y < 921.6f) {
            obj->m_vVelocity.y = 460.8f;
        } else {
            obj->m_vVelocity.y = 0.001f;
        }
        speed = 500.0f;
        obj->m_vVelocity.x = func_002982F0(heading) * speed;
        obj->m_vVelocity.z = func_002974C0(heading) * speed;
    } else {
        speed = Math_StepClamped(speed, gLevelScriptBlock.m_Script.speed, 5120.0f);
        obj->m_vVelocity.x += speed * func_002982F0(heading);
        obj->m_vVelocity.z += speed * func_002974C0(heading);
        obj->m_vVelocity.y = 0.001f;
    }
    if (gLevelScriptBlock.m_Script.scriptOn && gLevelScriptBlock.m_Script.bossState != 3 && gLevelScriptBlock.m_Script.bossState != 11) {
        if (gLevelScriptBlock.m_Script.script.current == -1) {
            if (gLevelScript_script_cursor != -1) {
                gLevelScript_script.timer = gLevelScript_script_steps[gLevelScript_script_cursor].delay;
            } else {
                gLevelScript_script.timer = 0.0f;
            }
            do {
                gLevelScript_script_cursor++;
                if (gLevelScript_script_steps[gLevelScript_script_cursor].type == 5) {
                    gLevelScript_script_cursor = 0;
                }
                gLevelScriptBlock.m_Script.script.current = gLevelScript_script_cursor;
            } while ((f32) gLevelScript_script_steps[gLevelScript_script_cursor].chance <= Random_Blend(0.0f, 100.0f));
        }
        Primagen_RunAttackStep_2(obj, ai, t.m_Sight.m_Dist);
    }
}

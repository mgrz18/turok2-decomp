/* NON-MATCHING: 5 of 104 words differ (scheduling of the first block: ROM puts li a2,0x13 in the jal delay slot). WRONG CUT: the real function is 0x00405AC8-0x00405C68 (size 0x1A0); splat's func_00405BFC
 * (0x00405BFC, 0x6C bytes) is its tail, reached by `j func_00405BFC` from inside this function. */
#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ CVector3 a;
    /* 0x18 */ CVector3 b;
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x88 - 0x28];
} Targets;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_00246310(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 func_0020EAA0(CVector3 *v);                                    /* heading */
extern f32 func_0021160C(f32 value, f32 target, f32 rate);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1F50[];
extern s32 D_8012FA04;          /* boss script index */

void func_00405AC8(CGameObjectInstance *obj, CAI *ai) {
    Targets t;
    CVector3 pos;
    CVector3 dir;
    f32 speed;
    f32 heading;
    f32 target;

    if (ai->m_AnimRepeatedCount != 0) {
        D_8012FA04 = -1;
        obj->m_AI.m_AnimSpeed = 1.0f;
        func_00243414(obj, ai, 0x13);
    }
    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.obj == NULL) {
        return;
    }
    pos = t.obj->header.m_vPos;
    speed = 0.0f;
    func_00245BAC(obj, ai, 0.261799425f, func_00246310(obj, pos));
    func_0020EF2C(&dir, &obj->header.m_vPos, &pos);
    heading = func_0020EAA0(&dir);
    if (obj->m_CurrentAnimation.m_AnimationFrame <= 9.0f) {
        target = t.dist * 0.229568392f;
    } else {
        target = speed;
    }
    speed = func_0021160C(speed, target, 512.0f);
    obj->m_vVelocity.x += speed * func_002982F0(heading);
    obj->m_vVelocity.z += speed * func_002974C0(heading);
    obj->m_vVelocity.y = 0.001f;
}

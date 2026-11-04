#include "common.h"
#include "structs/game_object.h"

extern void func_00246108(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_0025AD90(CGameObjectInstance *actor, CAI *ai, f32 angle);
extern void func_00210EF0(f32 *angle); /* wraps an angle */
extern void func_00245BAC(CGameObjectInstance *actor, CAI *ai, f32 turnRate, f32 angle);

void func_0040E108(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    f32 delta;
    f32 rate;
    f32 range;
    f32 turn;
    f32 half;
    f32 sum;

    func_00246108(actor, ai, &t, 0, 1);
    ai->m_BlindOne.m_SightDist = t.m_Sight.m_FlatDist;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            rate = 0.06981318f;
            range = 6.283186f;
            func_0025AD90(actor, ai, -1.5707965f);
            ai->m_HeadTurn = 0;
            break;
        case 0x455:
            rate = 0.06981318f;
            range = 0.78539824f;
            break;
        case 0x456:
            rate = 0.06981318f;
            range = 0.52359885f;
            break;
        default:
            range = rate = 0.0f;
            break;
    }
    delta = actor->m_RotY - ai->m_InitialRotY;
    func_00210EF0(&delta);
    turn = t.m_Sight.m_Angle;
    if (range < 6.283186f) {
        half = range * 0.5f;
        sum = delta + turn;
        if (sum > half) {
            turn = half - delta;
        } else if (sum < -half) {
            turn = -half - delta;
        }
    }
    func_00245BAC(actor, ai, rate, turn);
}

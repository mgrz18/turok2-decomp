#include "common.h"
#include "structs/game_object.h"

/* AI state: turn toward the target; after 100 steps, or when the target is within 460.8 and less than 20
 * degrees off, go to anim state 0x16. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ CGameObjectInstance *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 unk664;
    /* 0x668 */ u8 pad668[0x674 - 0x668];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

extern State D_8012FB70;
extern void func_00246108(void *obj, void *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00245BAC(void *obj, void *ai, f32 turnRate, f32 angle);
extern void func_00243414(void *obj, void *ai, s32 state);

void func_0040B80C(void *obj, void *ai) {
    CAITargets t;
    State *state;

    func_00246108(obj, ai, &t, 1, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.m_Path.m_Angle);
    state = &D_8012FB70;
    if (++state->counter > 100 ||
        (t.m_Path.m_pTarget != NULL && t.m_Sight.m_pTarget != NULL && t.m_Path.m_FlatDist < 460.8f &&
         (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle <= 0.34906590f : t.m_Sight.m_Angle <= 0.34906590f))) {
        func_00243414(obj, ai, 0x16);
    }
}

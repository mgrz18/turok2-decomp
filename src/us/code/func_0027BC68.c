#include "common.h"

typedef struct Timer800F5844 Timer800F5844;

/* At D_800F56B0 + 0x194. */
struct Timer800F5844 {
    /* 0x00 */ s32 m_Value;
    /* 0x04 */ u8 _pad4[0x8];
    /* 0x0C */ s32 m_Count;
    /* 0x10 */ s32 _unk10;
    /* 0x14 */ void (*m_pfnExpire)(Timer800F5844 *);
};

/* What this reads of the state at D_800F56B0. */
typedef struct {
    /* 0x000 */ s32 _unk0;
    /* 0x004 */ s32 m_bA;
    /* 0x008 */ s32 m_bB;
    /* 0x00C */ s32 m_bC;
    /* 0x010 */ u8 _pad[0x184];
    /* 0x194 */ Timer800F5844 m_Timer;
    /* 0x1AC */ u8 _pad1AC[0x8];
    /* 0x1B4 */ s32 m_Value;
} State800F56B0;

extern State800F56B0 D_800F56B0;
/* The ROM reaches these two through their own symbols, not D_800F56B0. */
extern s32 D_800F56BC;                              /* D_800F56B0.m_bC */
extern void (*D_800F5858)(Timer800F5844 *);         /* D_800F56B0.m_Timer.m_pfnExpire */
extern s32 D_8011AD10;

void func_00206068(s32);

static __inline__ s32 func_0027BC34(void) {
    State800F56B0 *state = &D_800F56B0;

    if (state->m_bA != 0 || state->m_bB != 0) {
        return state->m_Value;
    }
    return 0;
}

void func_0027BC68(void) {
    State800F56B0 *state;
    Timer800F5844 *timer;
    s32 flag;

    if (func_0027BC34()) {
        func_00206068(8);
        flag = D_800F56BC;
        state = &D_800F56B0;
        timer = &state->m_Timer;
        if (flag != 0) {
            D_800F5858(timer);
        } else if (--timer->m_Count <= 0) {
            timer->m_pfnExpire(timer);
        }
        D_8011AD10 = state->m_Timer.m_Value;
    }
}

#include "common.h"

/* What this reads of the state D_800C1BB0 points to. */
typedef struct {
    /* 0x00 */ u8 _pad0[0x1C];
    /* 0x1C */ f32 m_Time;
    /* 0x20 */ f32 m_PrevTime;
    /* 0x24 */ u8 _pad24[0x8];
    /* 0x2C */ f32 m_EndTime;
    /* 0x30 */ f32 m_EventTime;
    /* 0x34 */ u8 _pad34[0x8];
    /* 0x3C */ s32 m_bFinished;
    /* 0x40 */ u8 _pad40[0x74];
    /* 0xB4 */ s32 m_bStopAtEnd;
} State800C1BB0;

extern State800C1BB0 *D_800C1BB0;
extern s32 D_8011ACA4;
extern f32 D_800B6D28;          /* frame time scale */

s32 func_00284174(void);
s32 func_00284240(void);
void func_002838A4(void);

/* Advances the clock at D_800C1BB0 by one frame. */
void func_00284568(void) {
    if (func_00284174()) {
        if (D_8011ACA4 != 0 || func_00284240()) {
            D_800C1BB0->m_PrevTime = D_800C1BB0->m_Time;
            return;
        }
        D_800C1BB0->m_PrevTime = D_800C1BB0->m_Time;
        D_800C1BB0->m_Time += D_800B6D28 * (1.0f / 15);
        if (D_800C1BB0->m_Time >= D_800C1BB0->m_EventTime) {
            func_002838A4();
        }
        if (D_800C1BB0->m_Time >= D_800C1BB0->m_EndTime) {
            D_800C1BB0->m_Time = D_800C1BB0->m_EndTime;
            if (D_800C1BB0->m_bStopAtEnd) {
                D_800C1BB0->m_bFinished = 1;
                D_800C1BB0->m_bStopAtEnd = 0;
            }
        }
    }
}

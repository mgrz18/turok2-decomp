#include "common.h"
#include "structs/player.h"

/* The weapon AI's own variables, in its m_VarBuffer. */
#define ANGLE(ai) (*(f32 *) &(ai)->m_VarBuffer[0])
#define SPEED(ai) (*(f32 *) &(ai)->m_VarBuffer[1])
#define TIME(ai) (*(f32 *) &(ai)->m_VarBuffer[5])

/* Weapon spin: turns the barrel and makes the controller rumble with the spin speed. */

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 rumble;
} Controller;

extern f32 D_800B6D28; /* frame time scale */

extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern void func_002117DC(f32 *value, f32 target, f32 rate);
extern f32 func_002982F0(f32 x); /* sin */

void func_00259944(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    f32 t;

    ANGLE(state) += SPEED(state) * D_800B6D28;
    if (state->m_Mode == 4) {
        func_002117DC(&SPEED(state), 0.47123894f, 0.025f);
    } else {
        SPEED(state) = func_00211774(SPEED(state), 0.08726647496f, 0.008f);
    }
    TIME(state) += D_800B6D28;
    t = SPEED(state) * 2.122065783f;
    ((Controller *)player->m_ContState.m_pContState)->rumble = t * t * (func_002982F0(ANGLE(state) * 1.5f) * 0.4f + 0.4f) + t * 0.05f;
}

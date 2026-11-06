/* Boundary fixed in versions/cuts.us.txt: splat's AI_AnimReached (0x12C bytes) holds two functions:
 *   0x00265954  AI_AnimReached, the animation check (0xD0 bytes), then a nop;
 *   0x00265A28  two stray words (beq $18, $16, ...; sll $2, $18, 3), never run;
 *   0x00265A30  Destructible_Setup, an init callback (0x50 bytes; its pointer is in
 *               the data as 0x00265A30).
 * The check is the same as the inlined r01_check of Door_CloseUpdate and
 * func_002651FC, here out of line. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0020486C.c, r01_check, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void *table;
    /* 0x30 */ u8 pad30[0x96];
    /* 0xC6 */ u8 anim;
    /* 0xC7 */ s8 active;
    /* 0xC8 */ u8 padC8[0x28];
    /* 0xF0 */ void (*update)(CGameObjectInstance *, void *);
    /* 0xF4 */ u8 padF4[4];
    /* 0xF8 */ void (*onEvent)(CGameObjectInstance *, void *);
} ActionState;

/* Has the actor's animation for this state played up to `scale` of its length? */
s32 AI_AnimReached(CGameObjectInstance *actor, ActionState *state, f32 scale) {
    if (actor->m_nAnims == 1) return 1;
    if (actor->m_CurrentAnimation.m_nAnimIndex != (s8) state->anim || actor->m_CurrentAnimation.m_nRequestedAnimIndex != (s8) state->anim) return 0;
    if (state->active != 0 && !(actor->m_dwFlags & 0x400)) return 1;
    if (actor->m_CurrentAnimation.m_nFrames < 3 && (actor->m_dwFlags & 0x400)) {
        return (f32) actor->m_BlendLength * scale <= actor->m_BlendPercent;
    } else {
        return (f32) actor->m_CurrentAnimation.m_nFrames * scale <= actor->m_CurrentAnimation.m_AnimationFrame;
    }
}

extern u8 D_800B2330[];
extern void Destructible_Step(CGameObjectInstance *actor, void *state);
extern void Destructible_Hurt(CGameObjectInstance *actor, void *state);
extern void TotemDefense_Init(void);


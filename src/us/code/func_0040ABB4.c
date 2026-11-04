/* D_801301CC/D0/D4/EC are fields of D_8012FB70 (phase, activeMask, unk664, counter) that the ROM addresses
 * through their own symbols (as arrays: their loads are reloaded after the ai store). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module phase 2 entry: resets the actors' blend values and the fade timer, sets up phase 2 (timers, anim,
 * slot values), resets its active slots unless ai->m_Mode is 0x13, and queues func_0040C7B4. */


extern s32 D_801301CC[]; /* D_8012FB70.phase */
extern s32 D_801301D0[]; /* D_8012FB70.activeMask */
extern s32 D_801301D4[]; /* D_8012FB70.liveMask */
extern s32 D_801301EC[]; /* D_8012FB70.counter */
extern f32 D_800C1900; /* fade timer (func_0040A25C) */
extern s32 D_800C1904;
extern s32 D_00431DE4[];
extern s32 D_00431DF0[]; /* active slot mask per phase */
extern s32 D_00431DFC[];
extern u8 D_00431E08[];
extern void func_00236314(void *dst, void *src);
extern void func_0040C7B4(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &D_8012FB70;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            func_00236314(state->slots[i].anim, src);
        }
    }
}

static __inline__ void enterPhase2(MotherBossState *s, CAI *ai) {
    s32 mask;

    s->minRadius = 0x200;
    s->phase = 2;
    s->maxRadius = 0x600;
    D_801301EC[0] = 0;
    ai->m_VisBits = D_00431DE4[D_801301CC[0]];
    D_801301D4[0] = D_00431DFC[D_801301CC[0]];
    mask = D_00431DF0[D_801301CC[0]];
    D_800C1900 = 0.0f;
    D_800C1904 = 0;
    s->totalHealth = s->health = s->slots[13].health = 0x1F400;
    D_801301D0[0] = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(D_801301D0[0], D_00431E08);
    }
}

void func_0040ABB4(void *obj, CAI *ai) {
    MotherBossState *state = &D_8012FB70;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 1.0f;
        actor->m_AI.m_Mother.m_PulseScale = 0.0f;
    }
    enterPhase2(&D_8012FB70, ai);
    func_00283FF8(0xDC, func_0040C7B4, NULL);
}

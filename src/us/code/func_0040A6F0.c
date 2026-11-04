/* MATCHES at its real extent (checked with the symbol size patched to 0x20C); both flag masks are
 * computed before the tests (found with the permuter). */
/* WRONG CUT: real function 0x0040A6F0-0x0040A8FC (size 0x20C); splat split it at its tail jumps into
 * func_0040A6F0, func_0040A7AC, func_0040A8A0 and func_0040A8A4. */
#include "common.h"
#include "structs/game_object.h"

/* Picks the next attack anim from a weighted table: close to the target only 0x10/0xD, otherwise 0xF
 * (when flagged and not just used), 0x10 and 0x12 weighted by the module's timer ratio. */

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
    /* 0x668 */ u8 pad668[0x66C - 0x668];
    /* 0x66C */ s32 lastAnim;
    /* 0x670 */ u8 pad670[0x674 - 0x670];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ CAI ai;
} Obj;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

extern State D_8012FB70;
extern void func_00246108(Obj *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern s16 func_0026D518(Pick *table);
extern void func_00243414(Obj *obj, CAI *ai, s32 state);

/* func_0040BD60 */
static __inline__ f32 getTargetDist(Obj *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, &obj->ai, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void func_0040A6F0(Obj *obj, CAI *ai) {
    Pick picks[8];
    CGameObjectInstance *found;
    State *state = &D_8012FB70;
    s32 n;
    s32 flagged;
    f32 dist;
    f32 ratio;
    s32 flag2;
    s32 flag20;

    n = 0;
    dist = getTargetDist(obj, &found);
    flagged = 0;
    flag2 = ai->m_VisBits & 2;
    flag20 = ai->m_VisBits & 0x20;
    if (flag2) {
        if (flag20) {
            flagged = 1;
        }
    }
    if (state->unk674 != 0) {
        ratio = (f32)state->unk678 / (f32)state->unk674;
    } else {
        ratio = 1.0f;
    }
    if (found != NULL) {
        if (dist < 1433.6f) {
            picks[n].id = 0x10;
            picks[n++].weight = 5;
            picks[n].id = 0xD;
            picks[n++].weight = 5;
        } else {
            if (flagged && state->lastAnim != 0xF) {
                picks[n].id = 0xF;
                picks[n++].weight = 5;
            }
            picks[n].id = 0x10;
            picks[n++].weight = 5;
            picks[n].id = 0x12;
            if (ratio < 0.75f) {
                if (ratio < 0.5f) {
                    picks[n].weight = 10;
                } else {
                    picks[n].weight = 5;
                }
            } else {
                picks[n].weight = 2;
            }
            n++;
        }
        picks[n].id = -1;
        state->lastAnim = func_0026D518(picks);
        state->counter = 0;
        func_00243414(obj, ai, state->lastAnim);
    }
}

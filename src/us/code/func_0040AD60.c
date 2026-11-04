#include "common.h"
#include "structs/game_object.h"

/* Module init: collects up to 4 actors of type (0x44E, 9) into the module state, hooks their callbacks,
 * sets up the first phase (func_0040AE24) and queues func_0040C65C. */

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
extern u8 D_800F7078[];
extern s32 func_00225F6C(void *container, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
extern void func_0040BEE4(void);
extern void func_0040A25C(void);
extern void func_0040AE24(void *obj, void *ai);
extern void func_0040C65C(void);
extern s32 func_00283FF8(s32 id, void *arg1, void *arg2);

void func_0040AD60(void *obj, void *ai) {
    State *state = &D_8012FB70;
    CGameObjectInstance *actor;
    s32 i;

    state->count = func_00225F6C(D_800F7078, 0x44E, 9, -1, state->actors, 4);
    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_pfDraw = func_0040BEE4;
        actor->m_AI.m_pfEffects = func_0040A25C;
    }
    func_0040AE24(obj, ai);
    func_00283FF8(0xC8, func_0040C65C, NULL);
}

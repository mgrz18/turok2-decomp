#include "common.h"

/* Draw callback of the module's actors: draws each body part (anim group) whose slot is active in the
 * module object's mask, refreshing the slot's state first unless a cinema path is playing. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ s32 partMask;
} ModuleObj;

typedef struct {
    /* 0x000 */ ModuleObj *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ void *actors[4];
    /* 0x018 */ Slot slots[15];
} State;

typedef struct {
    /* 0x00 */ s32 animate;
    /* 0x04 */ s32 hasEvent;
    /* 0x08 */ u8 pad8[0x4];
    /* 0x0C */ s32 *anim;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ s32 first;
    /* 0x1C */ s32 current;
} AnimRequest;

typedef struct {
    u8 data[0x18];
} AnimSlot;

typedef struct {
    /* 0x000 */ u8 pad0[0x98];
    /* 0x098 */ s32 unk98;
    /* 0x09C */ u8 pad9C[0x110 - 0x9C];
    /* 0x110 */ AnimSlot lights[2];
    /* 0x140 */ u8 pad140[0x151 - 0x140];
    /* 0x151 */ s8 unk151;
} Actor;

extern State D_8012FB70;
extern s32 D_800B6D1C; /* current view */
extern s32 D_800AB8B0; /* fade mode */
extern void func_0021DC84(Actor *actor, void *ai, AnimRequest *req);
extern s32 Cinema_PathPlaying(void);
extern void func_00236324(void *slotData);
extern void func_00235898(void);
extern void func_0020A33C(s32 *anim, s32 model, s32 one, AnimSlot *slot, s32 zero, s32 index, s32 second);

void func_0040BEE4(Actor *actor, void *ai, AnimRequest *req) {
    State *state;
    u8 *data;
    s32 mask;
    s32 i;
    s32 one;

    i = 0;
    one = 1;
    state = &D_8012FB70;
    mask = state->obj->partMask;
    data = state->slots[0].data;
    do {
        if (mask & (one << i)) {
            req->first = i;
            req->current = i;
            func_0021DC84(actor, ai, req);
            if (!Cinema_PathPlaying()) {
                func_00236324(data);
                func_00235898();
            }
            func_0020A33C(req->anim, actor->unk98, 1, &actor->lights[D_800B6D1C], 0, i, actor->unk151);
            D_800AB8B0 = 0;
        }
        i++;
        data += sizeof(Slot);
    } while (i < 15);
}

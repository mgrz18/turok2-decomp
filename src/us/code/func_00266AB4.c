#include "common.h"

typedef struct {
    s32 animate;  /* 0x0 */
    s32 hasEvent; /* 0x4 */
    u8 pad8[0x4];
    s32 *anim;    /* 0xC */
    u8 pad10[0x8];
    s32 first;    /* 0x18 */
    s32 current;  /* 0x1C */
} AnimRequest;

typedef struct {
    u8 data[0x18];
} AnimSlot;

extern s32 D_800B6D1C;
s32 *func_002017D4(s32 base, s32 index);
void func_0021DC84(void *actor, void *state, AnimRequest *req);
void func_0020A33C(s32 *anim, s32 model, s32 one, AnimSlot *slot, s32 zero, s32 index, s32 second);

void func_00266AB4(u8 *actor, u8 *state, AnimRequest *req) {
    s32 i;
    s32 count;

    count = *func_002017D4(*req->anim, 2);
    for (i = 0; i < count; i++) {
        req->first = i;
        req->current = i;
        func_0021DC84(actor, state, req);
        func_0020A33C(req->anim, *(s32 *) (actor + 0x98), 1, &((AnimSlot *) (actor + 0x110))[D_800B6D1C], 0, i,
                      *(s8 *) (actor + 0x151));
    }
}

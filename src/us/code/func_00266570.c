#include "common.h"

typedef struct {
    s32 animate;  /* 0x0 */
    s32 hasEvent; /* 0x4 */
    u8 pad8[0x4];
    s32 anim;     /* 0xC */
    u8 pad10[0x8];
    s32 first;    /* 0x18 */
    s32 current;  /* 0x1C */
} AnimRequest;

typedef struct {
    u8 data[0x18];
} AnimSlot;

extern s32 D_800B6D1C;
void func_0021DC84(void *actor, void *state, AnimRequest *req);
void func_0020B6A0(s32 anim, s32 model, s32 one, AnimSlot *slot, s32 zero, s32 first, s32 second);

void func_00266570(u8 *actor, u8 *state, AnimRequest *req) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    s32 first;
    s32 second;

    if (*(s8 *) (state + 0x34) == 0) {
        first = *(s8 *) (info + 0xE);
        second = *(s8 *) (info + 0xF);
    } else {
        first = *(s8 *) (info + 0x1A);
        second = *(s8 *) (info + 0x1B);
    }
    req->first = first;
    req->current = first;
    *(s8 *) (state + 0x11) = second;
    *(s32 *) (actor + 0x14C) = 1 << req->first;
    if (req->hasEvent != 0) {
        func_0021DC84(actor, state, req);
    }
    if (req->animate != 0) {
        func_0020B6A0(req->anim, *(s32 *) (actor + 0x98), 1, &((AnimSlot *) (actor + 0x110))[D_800B6D1C], 0,
                      first, second);
    }
}

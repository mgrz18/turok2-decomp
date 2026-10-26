#include "common.h"

/* Engine actor instance (only the fields used here). */
typedef struct Actor {
    /* 0x000 */ u8 kind;
    u8 pad1;
    /* 0x002 */ u16 typeId;
    u8 pad4[0x3C];
    /* 0x040 */ u32 flags;
    u8 pad44[0xC4];
    /* 0x108 */ struct Actor *child;
    /* 0x10C */ u8 *parent;
    /* 0x110 */ s32 *liveCount;
    u8 pad114[0x4];
    /* 0x118 */ s32 sound;
    u8 pad11C[0x99];
    /* 0x1B5 */ u8 registered;
    u8 pad1B6[0xA];
    /* 0x1C0 */ void *list;
} Actor;

extern u8 D_800F8928[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];
s16 *func_0025398C(u8 *, s32);
void func_00232B40(Actor *);
void func_002327F4(Actor *);
void func_00234E48(void *, Actor *);
void func_00228B4C(void *, s32);
void CList__Remove(void *, Actor *);
void CList__AddHead(void *, Actor *);

void func_0022F1C8(u8 *mgr, Actor *actor) {
    Actor *child;
    u8 *owner;
    u32 flags;
    u8 kind;

    flags = actor->flags;
    if (!(flags & 0x100)) {
        return;
    }
    if (!(flags & 0x800)) {
        if (actor->typeId == 0x68) {
            owner = *(u8 **) (actor->parent + 0x1A8);
            *(s16 *) (owner + *func_0025398C(owner, 0x15) * 2 + 0x934) = 1;
        }
        child = actor->child;
        if (child != NULL && (kind = child->kind) == 2 && (child->flags & 0x100) &&
            (child->typeId == kind || child->typeId == 0x111)) {
            func_0022F1C8(D_800F8928, child);
            func_00232B40(child);
        }
        func_00232B40(actor);
        if (actor->registered) {
            func_002327F4(actor);
            func_00234E48(D_801102B8, actor);
        }
        if (actor->sound != 0) {
            func_00228B4C(D_8010FF34, actor->sound);
            actor->sound = 0;
        }
        if (actor->liveCount != NULL) {
            (*actor->liveCount)--;
        }
        actor->flags &= ~0x02000100;
        CList__Remove(actor->list, actor);
        actor->list = NULL;
        CList__AddHead(mgr + 0xEC00, actor);
        if (actor->flags & 0x01000000) {
            CList__Remove(mgr + 0xEC14, actor);
        }
    }
}

#include "common.h"

/* Bind an actor to its model and animation set, then choose its first
 * animation: `anim` > 0 is an action to pick at random (func_00219F74),
 * otherwise -anim is the entry itself. */

typedef struct Actor Actor;

struct Actor {
    u8 pad0[0x2];
    u16 unk2;               /* 0x002 */
    u8 pad4[0x54];
    u8 unk58[0x4C];         /* 0x058 */
    s32 unkA4;              /* 0x0A4 */
    s32 unkA8;              /* 0x0A8 */
    s32 unkAC;              /* 0x0AC */
    s32 unkB0;              /* 0x0B0 */
    s32 unkB4;              /* 0x0B4 */
    u8 padB8[0x2];
    u16 setId;              /* 0x0BA */
    u8 unkBC[0x18];         /* 0x0BC */
    u32 flags;              /* 0x0D4 */
    u8 unkD8[0x4];          /* 0x0D8 */
    s16 curAnim;            /* 0x0DC */
    s16 nextAnim;           /* 0x0DE */
    u8 padE0[0x2];
    u8 unkE2;               /* 0x0E2 */
    u8 unkE3;               /* 0x0E3 */
    u8 padE4[0x8];
    u8 unkEC[0x54];         /* 0x0EC */
    u8 ai[0x35];            /* 0x140 */
    s8 unk175;              /* 0x175 */
    u8 pad176[0x90];
    s8 unk206;              /* 0x206 */
    u8 pad207[0x3D];
    void (*init)(Actor *, void *);  /* 0x244 */
};

extern u8 D_800A5AC4[];     /* "anim object info" */
extern u8 D_800A5AD8[];     /* "anim object index" */

s32 *func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_00201848(s32, s32, u16);
s32 func_0020185C(s32, s32, s32, s32 *);
void *func_002017D4(s32, s32);
void func_002051F4(s32, s32 *);
void func_0020C0D0(void *, void *, void *);
void func_00219910(void *);
s32 func_00219F74(Actor *, s32, s32);

void func_0021A150(Actor *actor, s32 a1, s32 a2, s32 anim) {
    s32 *model;
    s32 *set;
    s32 id;
    s32 tree;
    s32 setTree;
    void *data;

    actor->flags &= ~0x40000;
    id = func_00201848(a1, a2, actor->unk2);
    model = func_0020367C(0, id, 0x18, 0, 0, 0, (s32) D_800A5AD8, 1);
    if (model != NULL) {
        tree = *model;
        actor->unkAC = func_0020185C(tree, id, 1, &actor->unkB4);
        actor->unkA4 = func_0020185C(tree, id, 0, &actor->unkB0);
        actor->unkA8 = func_00201848(tree, id, 2);
        set = func_0020367C(0, actor->unkA4, actor->unkB0, 0, 0, 0, (s32) D_800A5AC4, 1);
        if (set != NULL) {
            setTree = *set;
            data = func_002017D4(setTree, 0);
            actor->init(actor, actor->ai);
            func_0020C0D0(actor->unk58, data, actor->unkBC);
            func_00219910(actor->unkD8);
            func_00219910(actor->unkEC);
            actor->setId = ((u16 *) func_002017D4(setTree, 1))[3];
            actor->flags |= 0x40000;
            if (anim > 0) {
                anim = func_00219F74(actor, anim, -1);
            } else {
                anim = -anim;
            }
            if (anim == -1) {
                anim = 0;
            }
            actor->nextAnim = anim;
            actor->unkE2 = 0;
            if (actor->curAnim != anim) {
                actor->unkE3 = 1;
            }
            actor->unk175 = -1;
            actor->unk206 = anim;
            func_002051F4(0, set);
        }
        func_002051F4(0, model);
    }
}

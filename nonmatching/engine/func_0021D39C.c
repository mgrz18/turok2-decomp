/* NON-MATCHING: 35 of 264 words differ: register allocation only (view/model in s1/s2 and anim/inView in s4/s5 swapped); the permuter reaches 11 words only with a fake duplicated call. */
#include "common.h"

/* Draw pass for one actor in one view: if the actor is drawable (flags
 * 0x40000 and a draw callback) and active (0x2000), load its "anim object
 * info", decide whether it is in the view (bounding box against the view
 * frustum, or a box grown by 512 units for actors casting shadows,
 * flag 0x10000000), make sure its model is loaded (func_0021CDAC, playing its
 * spawn effect once, flag 0x200) and call its draw callback. */

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    u8 pad0[0x1C];
    s32 viewMask;           /* 0x1C */
} Region;

typedef struct Actor Actor;

typedef struct {
    s32 inView;             /* 0x00 */
    s32 near;               /* 0x04 */
    struct View *view;      /* 0x08 */
    void **model;           /* 0x0C */
    f32 *offset;            /* 0x10 */
    s32 unk14;              /* 0x14 */
    s32 unk18;              /* 0x18 */
    s32 unk1C;              /* 0x1C */
} DrawArgs;

struct Actor {
    u8 pad0[0x4];
    Vec3 pos;               /* 0x04 */
    Region *region;         /* 0x10 */
    u8 pad14[0x40];
    f32 height;             /* 0x54 */
    u8 matrix[0x4C];        /* 0x58 */
    s32 animFile;           /* 0xA4 */
    u8 padA8[0x8];
    s32 animIndex;          /* 0xB0 */
    u8 padB4[0x8];
    f32 boxMin[3];          /* 0xBC */
    f32 boxMax[3];          /* 0xC8 */
    s32 flags;              /* 0xD4 */
    u8 padD8[0x38];
    u8 lights[2][0x18];     /* 0x110 */
    u8 ai[0xF4];            /* 0x140 */
    void (*draw)(Actor *, void *, DrawArgs *);  /* 0x234 */
    u8 pad238[0xC];
    void (*update)(Actor *, void *);            /* 0x244 */
};

typedef struct View {
    u8 pad0[0x64];
    s32 mask;               /* 0x064 */
    u8 pad68[0x270];
    u8 frustum[0x60];       /* 0x2D8 */
    f32 boxMin[3];          /* 0x338 */
    f32 boxMax[3];          /* 0x344 */
} View;

extern char D_800A5AC4[];   /* "anim object info" */
extern s32 D_8011ACA4;
extern s32 D_800B6D1C;      /* current view */
extern u8 D_800F7078[];

void **func_0020367C(s32, s32, s32, s32, s32, s32, void *, s32);
s32 func_002017D4(void *, s32);
void func_002051F4(s32, void **);
void func_0020C0D0(void *, s32, f32 *);
s32 func_0026F60C(void *, f32 *);
f32 func_0026872C(Actor *, s32);
void **func_0021F358(Actor *);
s32 func_0021CDAC(Actor *, void *, s32);
f32 func_002671B4(Actor *);
void func_002254C0(void *, Vec3 *, void *);

void func_0021D39C(Actor *actor, View *view) {
    f32 offset[3];
    f32 box[6];
    Vec3 pos;
    DrawArgs args;
    s32 flags;
    void **anim;
    void **model;
    s32 info;
    s32 inView;
    s32 near;
    f32 y;

    flags = actor->flags;
    if (!(flags & 0x40000) || actor->draw == NULL) {
        return;
    }
    if (!(flags & 0x2000)) {
        actor->flags = flags & ~0x200;
        return;
    }
    anim = func_0020367C(0, actor->animFile, actor->animIndex, 0, 0, 0, D_800A5AC4, 1);
    if (anim == NULL) {
        return;
    }
    info = func_002017D4(*anim, 0);
    if (D_8011ACA4 != 0) {
        actor->update(actor, actor->ai);
        func_0020C0D0(actor->matrix, info, actor->boxMin);
    }
    if (!(actor->flags & 0x300000)) {
        inView = 0;
        near = 0;
        if (actor->region == NULL || (view->mask & actor->region->viewMask)) {
            if (view->boxMax[0] > actor->boxMin[0] && view->boxMin[0] < actor->boxMax[0]
                && view->boxMax[2] > actor->boxMin[2] && view->boxMin[2] < actor->boxMax[2]
                && view->boxMax[1] > actor->boxMin[1] && view->boxMin[1] < actor->boxMax[1]
                && func_0026F60C(view->frustum, actor->boxMin)) {
                inView = 1;
                if (actor->region != NULL) {
                    near = 1;
                }
            }
            if (!near && actor->region != NULL && (actor->flags & 0x10000000)) {
                y = func_0026872C(actor, 0);
                box[0] = actor->boxMin[0] - 512.0f;
                box[1] = y - 0.5f;
                box[2] = actor->boxMin[2] - 512.0f;
                box[3] = actor->boxMax[0] + 512.0f;
                box[5] = actor->boxMax[2] + 512.0f;
                box[4] = y + 0.5f;
                if (box[0] < view->boxMax[0] && view->boxMin[0] < box[3]
                    && box[2] < view->boxMax[2] && view->boxMin[2] < box[5]
                    && box[1] < view->boxMax[1] && view->boxMin[1] < box[4]
                    && func_0026F60C(view->frustum, box)) {
                set_near:
                    near = 1;
                }
            }
        }
    } else {
        inView = 1;
        goto set_near;
    }
    if (inView || near) {
        model = func_0021F358(actor);
        if (model != NULL) {
            if (!(actor->flags & 0x200)) {
                if (!func_0021CDAC(actor, *model, info)) {
                    goto free_model;
                }
                if (!(actor->flags & 8)) {
                    pos = actor->pos;
                    pos.y += actor->height;
                    pos.y += func_002671B4(actor) * 0.5f;
                    func_002254C0(D_800F7078, &pos, actor->lights[D_800B6D1C]);
                }
                actor->flags |= 0x200;
            }
            args.inView = inView;
            args.near = near;
            args.view = view;
            args.model = model;
            args.offset = offset;
            args.unk14 = 0;
            args.unk18 = 0;
            args.unk1C = 0;
            offset[0] = offset[1] = offset[2] = 0.0f;
            actor->draw(actor, actor->ai, &args);
        free_model:
            func_002051F4(0, model);
        }
    }
    func_002051F4(0, anim);
}

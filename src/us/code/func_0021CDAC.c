#include "common.h"

/* Builds an animated object's bone matrices: binds its animation index,
 * loads the current (and, when blending, the next) animation into the two
 * channels, finds their keyframes, fills a draw context and has
 * func_0021B95C pose the skeleton from the object's matrix (scaled to the
 * fixed-point 65536 units), timing that call in a profile slot. Then draws
 * the object's attachments. Returns whether the matrices were built. */

typedef struct AnimChannel {
    /* 0x00 */ f32 time;
    /* 0x04 */ s16 current;
    /* 0x06 */ s16 next;
    /* 0x08 */ s16 frames;
    /* 0x0A */ s8 active;
    /* 0x0B */ s8 blending;
    /* 0x0C */ u32 loopStart;
    /* 0x10 */ s32 resource;
} AnimChannel;

typedef struct KeyLookup {
    /* 0x00 */ void *posKeys;
    /* 0x04 */ void *rotKeys;
    /* 0x08 */ void *posTracks;
    /* 0x0C */ void *rotTracks;
    /* 0x10 */ s32 offset0;
    /* 0x14 */ s32 offset1;
    /* 0x18 */ s32 rootOffset0;
    /* 0x1C */ s32 rootOffset1;
    /* 0x20 */ f32 frac;
} KeyLookup;

typedef struct AnimObject {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 *def;           /* type id first */
    /* 0x018 */ u8 pad18[0x40];
    /* 0x058 */ f32 mtx[4][4];
    /* 0x098 */ s32 animIndex;
    /* 0x09C */ u8 pad9C[0xC];
    /* 0x0A8 */ s32 animFile;
    /* 0x0AC */ u8 padAC[0xE];
    /* 0x0BA */ s16 animCount;
    /* 0x0BC */ u8 padBC[0x18];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ AnimChannel channel;
    /* 0x0EC */ AnimChannel blendChannel;
    /* 0x100 */ u8 pad100[0xF];
    /* 0x10F */ u8 hasEffects;
    /* 0x110 */ u8 pad110[0x3C];
    /* 0x14C */ u32 attachMask;
    /* 0x150 */ u8 pad150[0x58];
    /* 0x1A8 */ struct AnimObject *owner;
    /* 0x1AC */ u8 pad1AC[0xD0];
    /* 0x27C */ s32 hasAttachments;
} AnimObject;

/* What func_0021B95C reads to pose the skeleton. */
typedef struct PoseContext {
    /* 0x00 */ AnimObject *obj;
    /* 0x04 */ s32 animIndex;
    /* 0x08 */ void *bones;
    /* 0x0C */ s32 *anims;
    /* 0x10 */ s32 *file;
    /* 0x14 */ s32 arg2;
    /* 0x18 */ AnimChannel *channel;
    /* 0x1C */ AnimChannel *blendChannel;
    /* 0x20 */ KeyLookup keys;
    /* 0x44 */ KeyLookup blendKeys;
    /* 0x68 */ s32 visible;
    /* 0x6C */ s32 blend;
    /* 0x70 */ s32 moving;
    /* 0x74 */ s32 f74;
    /* 0x78 */ s32 f78;
    /* 0x7C */ s32 f7C;
    /* 0x80 */ s32 f80;
    /* 0x84 */ s32 f84;
    /* 0x88 */ s32 f88;
    /* 0x8C */ u8 pad8C[0xC];
} PoseContext;

/* A profile slot (osGetCount timings). */
typedef struct Profile {
    /* 0x00 */ u64 start;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s32 frame;
    /* 0x14 */ s32 lastCalls;
    /* 0x18 */ s32 calls;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ f32 last;
    /* 0x24 */ f32 usec;
    /* 0x28 */ f32 average;
} Profile;

extern char D_800A5AF8[];
extern s32 D_800B6D18;
extern Profile D_800E6880;
extern s32 D_800E6898[]; /* &D_800E6880.calls */
extern u8 D_800F7148[];
extern u8 D_801102B8[];
extern u32 D_8011AAD8;

s32 Cinema_PathPlaying(void);
void *func_002017D4(void *file, s32 index);
void func_002051F4(s32, s32 handle);
s32 func_0020367C(s32, s32 file, s32 size, s32, s32, s32, char *name, s32);
s32 func_00206950(void *table, s32 id);
void func_00218B10(AnimChannel *channel, s32 anims, s32 file, s32, s32, s32);
void func_00219328(AnimChannel *channel, KeyLookup *keys);
void func_00219938(AnimChannel *channel);
s32 func_00219968(AnimChannel *channel);
void func_0021B95C(AnimObject *obj, PoseContext *ctx, s32, f32 mtx[4][4]);
void func_002360A0(void *effects, AnimObject *obj, void *bones);
void func_0026D710(void *list, s32 count, void *bones, AnimObject *obj, void *anims);
u32 func_0029DFC0(void);

/* OS_CYCLES_TO_USEC */
static __inline__ void addMicroseconds(f32 *usec, u32 cycles) {
    f32 *dst = usec;
    *dst = *usec + (f32) ((u64) cycles * 64 / 3000);
}

s32 func_0021CDAC(AnimObject *obj, s32 *file, s32 arg2) {
    PoseContext ctx;
    u8 bones[0x2000];
    f32 mtx[4][4];
    s32 *anims;
    s32 *handle;
    s32 ok;
    s32 hidden;
    s32 isType9;
    s32 inCinema;
    s32 type;
    AnimChannel *channel;
    AnimChannel *blendChannel;
    Profile *prof;
    u32 elapsed;
    s32 *lists;
    s32 *count;
    s32 n;
    s32 i;
    s32 *entry;
    s32 len;
    s32 *data;

    anims = func_002017D4(file, 3);
    obj->animIndex = func_00206950(D_800F7148, *anims);
    if (obj->animIndex == 0) {
        return 0;
    }
    ok = 0;
    hidden = obj->flags & 1;
    isType9 = *obj->def == 9;
    inCinema = 0;
    handle = (s32 *) func_0020367C(0, obj->animFile, ((obj->animCount * 4) + 0xF) & ~7, 4, 0, 0, D_800A5AF8 + 4, 0);
    if (handle != NULL) {
        channel = &obj->channel;
        blendChannel = &obj->blendChannel;
        func_00218B10(channel, *handle, obj->animFile, 0, 0, 0);
        if (func_00219968(channel)) {
            if ((obj->flags & 0x400)
                && (func_00218B10(blendChannel, *handle, obj->animFile, 0, 0, 0), func_00219968(blendChannel))) {
                ctx.blend = 1;
            } else {
                ctx.blend = 0;
            }
            ctx.obj = obj;
            ctx.animIndex = obj->animIndex;
            ctx.bones = bones;
            ctx.anims = anims;
            ctx.arg2 = arg2;
            ctx.channel = channel;
            ctx.blendChannel = blendChannel;
            ctx.file = file;
            func_00219328(&obj->channel, &ctx.keys);
            func_00219328(&obj->blendChannel, &ctx.blendKeys);
            ctx.visible = (!hidden || isType9) && !inCinema;
            type = *obj->def;
            if (obj->flags & 0x300000) {
                if (obj == obj->owner) {
                    ctx.visible = 1;
                } else {
                    ctx.visible = 0;
                }
                if (Cinema_PathPlaying()) {
                    ctx.visible = 0;
                }
            }
            if (!Cinema_PathPlaying()) {
                if ((channel->blending || blendChannel->blending) && type != 1) {
                    ctx.moving = 1;
                } else {
                    ctx.moving = 0;
                }
            } else {
                ctx.moving = 1;
            }
            if (!(obj->flags & 0x200000) && (type == 1 || type == 11)) {
                if (D_8011AAD8 & 0x100) {
                    ctx.f78 = 1;
                    ctx.f7C = 1;
                    ctx.f80 = 1;
                    ctx.f84 = 1;
                } else {
                    ctx.f78 = 0;
                    ctx.f7C = 0;
                    ctx.f80 = 0;
                    ctx.f84 = 0;
                }
                if (D_8011AAD8 & 0x20) {
                    ctx.f74 = 1;
                } else {
                    ctx.f74 = 0;
                }
                if (D_8011AAD8 & 0x80) {
                    ctx.f88 = 1;
                } else {
                    ctx.f88 = 0;
                }
            } else {
                ctx.f78 = 0;
                ctx.f7C = 0;
                ctx.f80 = 0;
                ctx.f84 = 0;
                ctx.f74 = 0;
                ctx.f88 = 0;
            }
            mtx[0][0] = obj->mtx[0][0] * 65536.0f;
            mtx[1][0] = obj->mtx[1][0] * 65536.0f;
            mtx[2][0] = obj->mtx[2][0] * 65536.0f;
            mtx[3][0] = obj->mtx[3][0] * 65536.0f;
            mtx[0][1] = obj->mtx[0][1] * 65536.0f;
            mtx[1][1] = obj->mtx[1][1] * 65536.0f;
            mtx[2][1] = obj->mtx[2][1] * 65536.0f;
            mtx[3][1] = obj->mtx[3][1] * 65536.0f;
            mtx[0][2] = obj->mtx[0][2] * 65536.0f;
            mtx[1][2] = obj->mtx[1][2] * 65536.0f;
            mtx[2][2] = obj->mtx[2][2] * 65536.0f;
            mtx[3][2] = obj->mtx[3][2] * 65536.0f;
            mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
            mtx[3][3] = 65536.0f;

            prof = &D_800E6880;
            prof->start = func_0029DFC0();
            func_0021B95C(obj, &ctx, 0, mtx);
            elapsed = func_0029DFC0() - (u32) prof->start;
            if (D_800B6D18 != prof->frame) {
                prof->frame = D_800B6D18;
                prof->average = prof->average * 0.97f + prof->usec * 0.03f;
                prof->last = prof->usec;
                prof->lastCalls = prof->calls;
                prof->usec = 0.0f;
                prof->calls = 0;
            }
            do { /* a profiling macro: its own block */
                addMicroseconds(&prof->usec, elapsed);
                D_800E6898[0]++;
            } while (0);

            if (obj->hasAttachments) {
                count = func_002017D4(file, 2);
                lists = func_002017D4(file, 4);
                n = *count;
                for (i = 0; i < n; i++) {
                    if (obj->attachMask & (1 << i)) {
                        entry = func_002017D4(lists, i);
                        len = entry[1];
                        if (len != 0) {
                            data = entry + 2;
                            func_0026D710(data, len, bones, obj, func_002017D4(file, 3));
                        }
                    }
                }
            }
            if (obj->hasEffects) {
                func_002360A0(D_801102B8, obj, bones);
            }
            if (ctx.blend) {
                func_00219938(blendChannel);
            }
            func_00219938(channel);
            ok = 1;
        }
        func_002051F4(0, (s32) handle);
    }
    if (!ok) {
        obj->animIndex = 0;
    }
    return ok;
}

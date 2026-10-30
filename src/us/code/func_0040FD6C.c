/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80400000_4.CPP, func_804009F4_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 yaw;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 id;
    /* 0x0C */ u32 flags;
    /* 0x10 */ u8 pad10[0x4C - 0x10];
} CinemaSpec;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ CinemaSpec entries[1];
} CinemaTable;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 time;
} Cue; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 count;
    /* 0x08 */ Cue cues[1];
} CueList;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 time;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
} Key; /* 0x24 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 count;
    /* 0x08 */ Key keys[1];
} Clip;

typedef struct {
    /* 0x000 */ void **owner;
    /* 0x004 */ void *resource;
    /* 0x008 */ u8 pad8[0xC - 0x8];
    /* 0x00C */ void (*onClose)(void);
    /* 0x010 */ void (*callback10)(void);
    /* 0x014 */ u8 pad14[0x18 - 0x14];
    /* 0x018 */ Actor *target;
    /* 0x01C */ f32 time;
    /* 0x020 */ f32 time20;
    /* 0x024 */ f32 time24;
    /* 0x028 */ f32 clipStart;
    /* 0x02C */ f32 clipEnd;
    /* 0x030 */ f32 start;
    /* 0x034 */ f32 end;
    /* 0x038 */ s32 active;
    /* 0x03C */ u8 pad3C[0x40 - 0x3C];
    /* 0x040 */ s32 unknown40;
    /* 0x044 */ s32 initialized44;
    /* 0x048 */ s32 closed;
    /* 0x04C */ s32 field4C;
    /* 0x050 */ s32 field50;
    /* 0x054 */ s32 pending;
    /* 0x058 */ s32 field58;
    /* 0x05C */ u8 pad5C[0x68 - 0x5C];
    /* 0x068 */ CinemaSpec spec;
    /* 0x0B4 */ u8 padB4[0xB8 - 0xB4];
    /* 0x0B8 */ s32 mode;
    /* 0x0BC */ u8 padBC[0xE0 - 0xBC];
    /* 0x0E0 */ u32 request;
    /* 0x0E4 */ void (*callbackE4)(void);
    /* 0x0E8 */ void (*callbackE8)(void);
    /* 0x0EC */ u8 padEC[0xF0 - 0xEC];
    /* 0x0F0 */ Vec3f position;
    /* 0x0FC */ f32 yaw;
} Cinema;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 trackLookup;
    /* 0x28 */ u8 pad28[0x60 - 0x28];
    /* 0x60 */ s32 *trackBundle;
} Resources;

extern Cinema *D_800C1BB0;
extern Resources D_800F7078;
extern s32 *D_800F70D8; /* D_800F7078.trackBundle */
extern s32 D_800F709C;  /* D_800F7078.trackLookup */
extern Actor *D_80119890; /* current player */

extern void func_0040F9F4(s32 unused, void **handle);
extern void func_0028441C(void);
extern s32 func_0020185C(s32 *bundle, s32 lookup, s32 index, s32 *size);
extern void **func_0020367C(s32 a, s32 data, s32 size, s32 tag, s32 e, void *prepare, const char *name, s32 h);
extern void func_002051F4(s32 unused, void **handle);
extern CueList *func_00412E1C(void);
extern s32 func_00412DA4(void);
extern Clip *func_00412DD0(s32 index);

static __inline__ void minTime(f32 *dest, f32 *src) {
    if (*dest > *src) {
        *dest = *src;
    }
}

static __inline__ void maxTime(f32 *dest, f32 *src) {
    if (*dest < *src) {
        *dest = *src;
    }
}

static __inline__ void setTimes(f32 *a, f32 *b, f32 *c, f32 *t) {
    *a = *t;
    *b = *t;
    *c = *t;
}

/* Loads cinema `index`: its track, its time range from the clips and cues, and its entry in the cinema table. */
s32 func_0040FD6C(s32 index) {
    s32 size;
    s32 size2;
    s32 i;
    s32 count;
    s32 previous;
    s32 pending;
    s32 loaded;
    Actor *captured;
    Resources *resources;
    void **owner;
    void **table;
    CueList *cues;
    Cue *cue;
    Clip *clip;
    Cinema *context;
    Cinema *scanContext;
    f32 total;

    captured = D_80119890;
    pending = D_800C1BB0->field58;
    resources = &D_800F7078;
    if (resources->trackLookup == 0 || resources->trackBundle == 0) {
    failed:
        return 0;
    } else {
        if (D_800C1BB0->active != 0) {
            func_0028441C();
        }
        loaded = func_0020185C(resources->trackBundle, resources->trackLookup, index + 1, &size);
        owner = func_0020367C(0, loaded, size, 0x13, 0, func_0040F9F4, "Cinema", 1);
        if (owner == NULL) {
            goto failed;
        }
        D_800C1BB0->owner = owner;
        D_800C1BB0->resource = *owner;
        cues = func_00412E1C();
        cue = cues->cues;
        count = cues->count;
        total = 0.0f;
        for (i = 0; i < func_00412DA4(); i++) {
            clip = func_00412DD0(i);
            {
                s32 n = clip->count;
                Key *base = clip->keys;
                if (n > 0) {
                    total += base[n - 1].time;
                }
            }
        }
        D_800C1BB0->start = 0.0f;
        D_800C1BB0->end = total;
        D_800C1BB0->clipStart = 0.0f;
        D_800C1BB0->clipEnd = total;
        i = 0;
        if (count > 0) {
            scanContext = D_800C1BB0;
            do {
                Cue *c = &cue[i];
                minTime(&scanContext->clipStart, &c->time);
                maxTime(&scanContext->clipEnd, &c->time);
                i++;
            } while (i < count);
        }
        setTimes(&D_800C1BB0->time, &D_800C1BB0->time20, &D_800C1BB0->time24, &D_800C1BB0->clipStart);
        previous = D_800C1BB0->spec.id;
        loaded = func_0020185C(D_800F70D8, D_800F709C, 0, &size2);
        if (loaded == 0) {
            table = NULL;
        } else {
            table = func_0020367C(0, loaded, size2, 0x13, 0, NULL, "Cinema", 1);
        }
        {
            CinemaTable *entries = *table;
            CinemaSpec *base = entries->entries;
            D_800C1BB0->pending = pending;
            D_800C1BB0->active = 1;
            D_800C1BB0->spec = base[index];
        }
        D_800C1BB0->field50 = 0;
        D_800C1BB0->unknown40 = 0;
        D_800C1BB0->initialized44 = 0;
        D_800C1BB0->closed = 0;
        D_800C1BB0->request = D_800C1BB0->spec.id;
        if ((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) {
            D_800C1BB0->position = D_800C1BB0->target->pos;
            D_800C1BB0->yaw = D_800C1BB0->target->yaw + 1.5707964f;
        }
        if (D_800C1BB0->spec.flags & 2) {
            if (captured != NULL) {
                D_800C1BB0->position = captured->pos;
                D_800C1BB0->yaw = captured->yaw;
            } else {
                D_800C1BB0->position.x = 0.0f;
                D_800C1BB0->position.y = 0.0f;
                D_800C1BB0->position.z = 0.0f;
                D_800C1BB0->yaw = 0.0f;
            }
        }
        func_002051F4(0, table);
        if (pending) {
            void (*first)(void);
            void (*second)(void);
            context = D_800C1BB0;
            first = context->callbackE4;
            second = context->callbackE8;
            context->pending = 1;
            context->field4C = 0;
            context->callbackE4 = NULL;
            context->callbackE8 = NULL;
            context->mode = context->spec.flags & 8;
            context->onClose = first;
            context->callback10 = second;
            if (context->request - 10U < 0x47U) {
                context->mode = 0;
            }
            if (D_800C1BB0->request - 400U < 4U) {
                D_800C1BB0->mode = 0;
            }
        } else {
            if (D_800C1BB0->spec.id < 3) {
                D_800C1BB0->request = previous;
            }
            D_800C1BB0->pending = 0;
            D_800C1BB0->time = D_800C1BB0->start;
        }
        if (D_800C1BB0->spec.id == 0x9F) {
            D_800C1BB0->mode = 0;
        }
        return 1;
    }
}

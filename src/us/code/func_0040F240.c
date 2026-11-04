#include "common.h"
#include "rw/abu_types.h"
#include "structs/world.h"

/* The cinema system: one file from func_0040F240 to func_004131B0 (0x40F240-0x413228).
 * Its strings ("Cinema temp alloc", "Cinema") are shared by functions all over
 * the block, which GCC does only within one file, and its .rodata is the ROM's
 * 0x198 bytes at 0x800C0950. The second VM module has a copy of the block at the
 * same link address (func_004491B8, func_0044AC94, ...) that reads this pool. */

/* A spline record: a point, the second derivatives of its coordinates over the
 * arc length, the arc length, the weight (time) and the second derivative of the
 * arc length over the weight. */
typedef struct {
    /* 0x00 */ Vec3 p;
    /* 0x0C */ Vec3 d2p;
    /* 0x18 */ f32 s;
    /* 0x1C */ f32 w;
    /* 0x20 */ f32 d2s;
} SplineNode; /* 0x24 bytes */

typedef struct {
    /* 0x00 */ s32 nodeSize;
    /* 0x04 */ s32 count;
    /* 0x08 */ SplineNode nodes[1];
} Spline;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
} ActorHead; /* the part of the actor saved around the collision test */

typedef struct {
    /* 0x00 */ ActorHead head;
    /* 0x34 */ u8 pad34[0x50 - 0x34];
    /* 0x50 */ f32 yaw;
} Actor;

/* An entry of the cinema table (0x4C bytes). */
typedef struct {
    /* 0x00 */ s32 id0;
    /* 0x04 */ s32 id4;
    /* 0x08 */ s32 id;
    /* 0x0C */ u32 flags;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ char name[0x3C - 0x18];
    /* 0x3C */ f32 aspect;
    /* 0x40 */ u8 pad40[0x4C - 0x40];
} CinemaEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ CinemaEntry entries[1];
} CinemaTable;

/* The cinema state, D_800C1BB0. */
typedef struct {
    /* 0x000 */ s32 **owner;
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
    /* 0x05C */ u8 pad5C[0x60 - 0x5C];
    /* 0x060 */ s32 fading;
    /* 0x064 */ f32 fade;
    /* 0x068 */ CinemaEntry spec;
    /* 0x0B4 */ s32 fieldB4;
    /* 0x0B8 */ s32 mode;
    /* 0x0BC */ u8 padBC[0xD8 - 0xBC];
    /* 0x0D8 */ s32 targetD8;
    /* 0x0DC */ s32 resourceDC;
    /* 0x0E0 */ u32 request;
    /* 0x0E4 */ void (*callbackE4)(void);
    /* 0x0E8 */ void (*callbackE8)(void);
    /* 0x0EC */ u8 padEC[0xF0 - 0xEC];
    /* 0x0F0 */ Vec3 position;
    /* 0x0FC */ f32 yaw;
} Cinema;

/* A track header: its stride and record count, the records after it. */
typedef struct {
    /* 0x00 */ s32 stride;
    /* 0x04 */ s32 count;
} Track;

/* A level object a cinema cue refers to. */
typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ u16 id4;
    /* 0x06 */ u8 pad6[0xA - 0x6];
    /* 0x0A */ u16 idA;
    /* 0x0C */ u8 padC[0xE - 0xC];
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 padF[0x11 - 0xF];
    /* 0x11 */ u8 kind;
    /* 0x12 */ u8 val;
} CueObject;

extern Cinema *D_800C1BB0;
extern s32 D_800C1BB8;
extern s32 D_800C1BC4;
extern f32 D_800C1BC8;
extern s32 D_800C2030;
extern s32 D_800C2034;
extern Gfx *D_800D8E24;
/* An array to GCC: a function that reads two of its fields keeps its address in one register. */
extern CWorld D_800F7078[];
extern s32 D_800F709C;  /* D_800F7078.trackLookup */
extern s32 *D_800F70D8; /* D_800F7078.trackBundle */
extern Actor *D_80119890; /* current player */

extern void *func_002017D4(void *node, s32 index); /* child `index` of a resource node */
extern s32 func_0020185C(s32 *bundle, s32 lookup, s32 index, s32 *size);
extern s32 **func_0020367C(s32 a, s32 data, s32 size, s32 tag, s32 e, void *prepare, const char *name, s32 h);
extern s32 **func_00204EDC(s32 heap, s32 size, s32 tag, const char *name);
extern void func_002051F4(s32 unused, s32 **handle);
extern void func_002052D8(s32 heap, s32 *block);
extern void func_0020EEF8(Vec3 *out, Vec3 *a, Vec3 *b);  /* out = a + b */
extern void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);  /* out = b - a */
extern void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale); /* out = in * scale */
extern f32 func_0020EAA0(Vec3 *v);                       /* heading */
extern f32 func_0020F5C0(Vec3 *v);                       /* pitch */

Vec3 func_004101B8(SplineNode *nodes, s32 count, f32 t);
void func_00412314(void *owner, CueObject *obj);
s32 func_00412438(s32 key);
s32 func_00412DA4(void);
Spline *func_00412DD0(s32 index);
void *func_00412E1C(void);

/* Allocates a temporary array; the block's first word points back at the handle. */
static __inline__ f32 *TempAlloc(s32 size) {
    s32 **handle;
    s32 *block;

    handle = func_00204EDC(0, size + 0x10, 0x13, "Cinema temp alloc");
    if (handle != NULL) {
        block = *handle;
        *block = (s32) handle;
        return (f32 *) (block + 4);
    }
    return NULL;
}

static __inline__ void TempFree(f32 *data) {
    if (data != NULL) {
        func_002052D8(0, ((s32 **) data)[-4]);
    }
}

/* Whether a cue's object is one the cinema waits for (a separate function in
 * Rage Wars). Inline but not static: GCC inlines it where it is called and
 * emits its own copy at the end of the file, func_004131B0. */
__inline__ s32 func_004131B0(CueObject *k) {
    s32 r = 0;
    switch (k->kind) {
    case 1:
        if (k->val == 5) r = 1;
        break;
    case 9:
        if (k->val == 4) r = 1;
        break;
    case 10:
        if (k->val == 3) r = 1;
        break;
    default:
        r = 0;
        break;
    }
    return r;
}

/* Computes the second derivatives of a natural cubic spline through n points (ys over xs) into out. */
void func_0040F240(s32 n, f32 *ys, f32 *xs, f32 *out) {
    f32 *diagonal;
    f32 *rhs;
    f32 *width;
    s32 singular;
    s32 i;

    singular = 0;
    if (n == 1) {
        out[0] = 0;
        return;
    }
    diagonal = TempAlloc(n * 4);
    rhs = TempAlloc(n * 4);
    width = TempAlloc(n * 4);
    for (i = 1; i < n - 1; i++) {
        diagonal[i] = 2.0f * (xs[i + 1] - xs[i - 1]);
        if (diagonal[i] == 0.0f) {
            diagonal[i] = 1.0f;
            singular = 1;
        }
    }
    for (i = 0; i < n - 1; i++) {
        width[i] = xs[i + 1] - xs[i];
        if (width[i] == 0.0f) {
            width[i] = 1.0f;
            singular = 1;
        }
    }
    for (i = 1; i < n - 1; i++) {
        rhs[i] = ((ys[i + 1] - ys[i]) / width[i] - (ys[i] - ys[i - 1]) / width[i - 1]) * 6.0f;
    }
    for (i = 1; i < n - 2; i++) {
        rhs[i + 1] -= rhs[i] * width[i] / diagonal[i];
        diagonal[i + 1] -= width[i] * width[i] / diagonal[i];
    }
    for (i = 0; i < n; i++) {
        out[i] = 0;
    }
    if (!singular) {
        for (i = n - 2; i > 0; i--) {
            out[i] = (rhs[i] - width[i] * out[i + 1]) / diagonal[i];
        }
    }
    TempFree(diagonal);
    TempFree(rhs);
    TempFree(width);
}

extern f32 func_00298470(f32 x); /* sqrt */

/* Builds a spline through count points: arc lengths, then second derivatives of x, y and z over the arc length
 * and of the arc length over the weights. */
void func_0040F5B8(s32 count, Vec3 *points, f32 *weights, Spline *spline) {
    Vec3 delta;
    f32 *ys;
    f32 *xs;
    f32 *d2;
    SplineNode *nodes;
    f32 length;
    s32 i;

    length = 0.0f;
    ys = TempAlloc(count * 4);
    xs = TempAlloc(count * 4);
    d2 = TempAlloc(count * 4);
    nodes = spline->nodes;
    spline->nodeSize = 0x24;
    spline->count = count;
    if (count == 1) {
        spline->nodes[0].p.x = points->x;
        nodes->p.y = points->y;
        nodes->p.z = points->z;
        nodes->d2p.x = 0.0f;
        nodes->d2p.y = 0.0f;
        nodes->d2p.z = 0.0f;
        nodes->d2s = 0.0f;
        nodes->w = 0.0f;
        return;
    }
    for (i = 0; i < count; i++) {
        nodes[i].p.x = points[i].x;
        nodes[i].p.y = points[i].y;
        nodes[i].p.z = points[i].z;
        nodes[i].w = weights[i];
    }
    for (i = 0; i < count; i++) {
        nodes[i].s = length;
        if (i == count - 1) {
            break;
        }
        func_0020EF2C(&delta, &nodes[i].p, &nodes[i + 1].p);
        length += func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].p.x;
        xs[i] = nodes[i].s;
    }
    func_0040F240(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2p.x = d2[i];
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].p.y;
        xs[i] = nodes[i].s;
    }
    func_0040F240(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2p.y = d2[i];
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].p.z;
        xs[i] = nodes[i].s;
    }
    func_0040F240(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2p.z = d2[i];
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].s;
        xs[i] = nodes[i].w;
    }
    func_0040F240(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2s = d2[i];
    }
    TempFree(ys);
    TempFree(xs);
    TempFree(d2);
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80400000_4.CPP, func_8040067C_de. */
typedef struct SplineSource { s32 stride,count; union { char data[0]; Vec3 points[0]; f32 weights[0]; }; } SplineSource;
typedef struct SplineTable { s32 count; } SplineTable;
typedef union SplineSourceCursor { SplineSource *header; Vec3 *points; f32 *weights; } SplineSourceCursor;
extern int func_002018D8(int *);
extern void func_00218548(void *,s32);
extern char *func_00218590(Pack *,s32,s32);
extern s32 func_00200B00(void **, s32);
extern struct Shape_typemap_165 *func_00205D7C(s32,void *,s32);
extern void func_002057B0(s32,struct Shape_typemap_165 **,struct Shape_typemap_165 *);
static __inline__ char *packAppend(Pack *pack,void *prepare,s32 size) { return func_00218590(pack,(s32)prepare,size); }
static __inline__ s32 splineRecordSize(s32 left,s32 right) { s32 a=left*9,b=right*9; return (b+a)*4+48; }
void func_0040F9F4(s32 unused, struct Shape_typemap_165 **handle) {
    struct Shape_typemap_165 *output;
    void *source, *a, *b, *c, *part, *extra;
    Pack *pack, *dest;
    SplineTable *parts;
    Pack *table;
    SplineSourceCursor p,q,r,s;
    s32 count, i, size, sa, sb, sc, np, nr, byteP, byteR;
    void *dp, *dr;
    if (!func_00200B00((void **)handle, 1)) return;
    source = (*handle)->pack;
    parts = func_002017D4(source, 0);
    a = func_002017D4(source, 1);
    b = func_002017D4(source, 2);
    c = func_002017D4(source, 3);
    count = parts->count;
    size = (count * 4 + 15) & ~7;
    for (i = 0; i < parts->count; i++) {
        SplineSource *firstP,*firstR;
        void *firstPart;
        s32 byteP,byteR;
        firstPart = func_002017D4(parts, i);
        firstP = func_002017D4(firstPart, 0);
        firstR = func_002017D4(firstPart, 2);
        size += splineRecordSize(firstP->count,firstR->count);
    }
    sc = func_002018D8(c);
    sa = func_002018D8(a);
    sb = func_002018D8(b);
    output = func_00205D7C(0, handle, size + sa + sb + sc + 24);
    if (output == 0) return;
    pack = output->pack;
    func_00218548(pack, 4);
    table = (void *)packAppend(pack, 0, size);
    packAppend(pack, a, sa);
    packAppend(pack, b, sb);
    packAppend(pack, c, sc);
    func_00218548(table, count);
    for (i = 0; i < count; i++) {
        s32 byteP,byteR;
        SplineSource *sizeP,*sizeR;
        part = func_002017D4(parts, i);
        sizeP = func_002017D4(part, 0);
        sizeR = func_002017D4(part, 2);
        byteP = sizeP->count * 9;
        byteR = sizeR->count * 9;
        dest = (void *)packAppend(table, 0, (byteR + byteP)*4 + 48);
        func_00218548(dest, 3);
        p.header = func_002017D4(part, 0);
        q.header = func_002017D4(part, 1);
        r.header = func_002017D4(part, 2);
        s.header = func_002017D4(part, 3);
        extra = func_002017D4(part, 4);
        np = p.header->count;
        nr = r.header->count;
        { s32 pointBytes,otherBytes;
        pointBytes = np * 36 + 8;
        otherBytes = nr * 36 + 8;
        dp = packAppend(dest, 0, pointBytes);
        dr = packAppend(dest, 0, otherBytes);
        }
        p.points = p.header->points;
        q.weights = q.header->weights;
        r.points = r.header->points;
        s.weights = s.header->weights;
        packAppend(dest, extra, 8);
        func_0040F5B8(np, p.points, q.weights, dp);
        func_0040F5B8(nr, r.points, s.weights, dr);
    }
    func_002057B0(0, handle, output);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 time;
} Cue; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 count;
    /* 0x08 */ Cue cues[1];
} CueList;

extern void func_0028441C(void);

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
    CWorld *resources;
    s32 **owner;
    s32 **table;
    CueList *cues;
    Cue *cue;
    Spline *clip;
    Cinema *context;
    Cinema *scanContext;
    f32 total;

    captured = D_80119890;
    pending = D_800C1BB0->field58;
    resources = D_800F7078;
    if (resources->m_CinemasKey == 0 || resources->m_pCinemas == 0) {
    failed:
        return 0;
    } else {
        if (D_800C1BB0->active != 0) {
            func_0028441C();
        }
        loaded = func_0020185C(resources->m_pCinemas, resources->m_CinemasKey, index + 1, &size);
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
                SplineNode *base = clip->nodes;
                if (n > 0) {
                    total += base[n - 1].w;
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
            CinemaTable *entries = (CinemaTable *) *table;
            CinemaEntry *base = entries->entries;
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
            D_800C1BB0->position = D_800C1BB0->target->head.pos;
            D_800C1BB0->yaw = D_800C1BB0->target->yaw + 1.5707964f;
        }
        if (D_800C1BB0->spec.flags & 2) {
            if (captured != NULL) {
                D_800C1BB0->position = captured->head.pos;
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

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00400E50.c. */
Vec3 func_004101B8(SplineNode *rec, s32 n, f32 t) {
    Vec3 res;
    SplineNode *p = 0;
    SplineNode *q = 0;
    s32 i;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 m0;
    f32 m1;
    f32 A;
    f32 B;
    f32 u;
    f32 s;
    f32 h2;
    f32 w;
    f32 h;
    f32 pa;
    f32 qa;
    if (n == 0) {
        res.x = 0;
        res.y = 0;
        res.z = 0;
    } else if (n == 1 || t <= rec[0].w) {
        res.x = rec[0].p.x;
        res.y = rec[0].p.y;
        res.z = rec[0].p.z;
    } else if (rec[n - 1].w <= t) {
        res.x = rec[n - 1].p.x;
        res.y = rec[n - 1].p.y;
        res.z = rec[n - 1].p.z;
    } else {
        for (i = 0; i < n - 1; i++) {
            p = &rec[i];
            q = &rec[i + 1];
            if (p->w <= t && t < q->w) {
                break;
            }
        }
        d0 = 0.0f;
        d1 = 0.0f;
        d2 = 0.0f;
        if (i > 0) {
            d0 = rec[i].s - rec[i - 1].s;
        }
        if (i < n - 1) {
            d1 = rec[i + 1].s - rec[i].s;
        }
        if (i < n - 2) {
            d2 = rec[i + 2].s - rec[i + 1].s;
        }
        m0 = d1;
        if (!(m0 <= d0)) {
            m0 = d0;
        }
        m1 = d2;
        if (!(m1 <= d1)) {
            m1 = d1;
        }
        A = p->s;
        B = q->s;
        if (p->w != q->w) {
            u = (t - p->w) / (q->w - p->w);
        } else {
            u = 0.0f;
        }
        s = A * (1.0f - u) + B * u + (m0 - (B - A)) * (u - 2.0f * u * u + u * u * u) + (m1 - (B - A)) * (u * u * u - u * u);
        for (i = 0; i < n - 1; i++) {
            p = &rec[i];
            q = &rec[i + 1];
            if (p->s <= s && s < q->s) {
                break;
            }
        }
        pa = p->s;
        qa = q->s;
        h = qa - pa;
        w = 0.0f;
        if (h != 0.0f) {
            w = (s - pa) / h;
        }
        h2 = h * h;
        res.x = p->p.x * (1.0f - w) + q->p.x * w + (p->d2p.x * ((1.0f - w) * (1.0f - w) * (1.0f - w) - (1.0f - w)) + q->d2p.x * (w * w * w - w)) * h2 * 0.16666667f;
        res.y = p->p.y * (1.0f - w) + q->p.y * w + (p->d2p.y * ((1.0f - w) * (1.0f - w) * (1.0f - w) - (1.0f - w)) + q->d2p.y * (w * w * w - w)) * h2 * 0.16666667f;
        res.z = p->p.z * (1.0f - w) + q->p.z * w + (p->d2p.z * ((1.0f - w) * (1.0f - w) * (1.0f - w) - (1.0f - w)) + q->d2p.z * (w * w * w - w)) * h2 * 0.16666667f;
    }
    return res;
}

Vec3 func_0041057C(f32 t) {
    s32 i;
    s32 idx;
    s32 count;
    SplineNode *keys;
    s32 *clip;
    f32 start;
    f32 end;
    start = 0.0f;
    count = 0;
    keys = 0;
    idx = 0;
    i = 0;
    end = start;
    while (i < *(s32 *)func_002017D4(D_800C1BB0->resource, 0)) {
        idx = i;
        clip = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), idx), 0);
        count = clip[1];
        keys = (SplineNode *)(clip + 2);
        start = end;
        end = start + keys[count - 1].w;
        if (idx == 0 && t <= start) {
            break;
        }
        if (idx == *(s32 *)func_002017D4(D_800C1BB0->resource, 0) - 1 && end < t) {
            break;
        }
        i = idx + 1;
        if (start <= t && t <= end) {
            break;
        }
    }
    if (*(s32 *)func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), idx), 2) != 0) {
        t = start;
    }
    return func_004101B8(keys, count, t - start);
}

Vec3 func_00410724(f32 t) {
    s32 i;
    s32 idx;
    s32 count;
    SplineNode *keys;
    s32 *clip;
    f32 start;
    f32 end;
    start = 0.0f;
    count = 0;
    keys = 0;
    idx = 0;
    i = 0;
    end = start;
    while (i < *(s32 *)func_002017D4(D_800C1BB0->resource, 0)) {
        idx = i;
        clip = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), idx), 1);
        count = clip[1];
        keys = (SplineNode *)(clip + 2);
        start = end;
        end = start + keys[count - 1].w;
        if (idx == 0 && t <= start) {
            break;
        }
        if (idx == *(s32 *)func_002017D4(D_800C1BB0->resource, 0) - 1 && end < t) {
            break;
        }
        i = idx + 1;
        if (start <= t && t <= end) {
            break;
        }
    }
    if (((s32 *)func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), idx), 2))[1] != 0) {
        t = start;
    }
    return func_004101B8(keys, count, t - start);
}

Vec3 func_004108CC(f32 t) {
    s32 *track;
    PositionKey *key;
    s32 count;
    f32 u;
    Vec3 result;
    Vec3 a;
    Vec3 b;
    track = func_002017D4(D_800C1BB0->resource, 1);
    count = track[1];
    key = (PositionKey *)(track + 2);
    if (count == 0) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return result;
    } else if (t <= key[0].time) {
        result.x = key[0].x;
        result.y = key[0].y;
        result.z = key[0].z;
        return result;
    } else if (key[count - 1].time <= t) {
        result.x = key[count - 1].x;
        result.y = key[count - 1].y;
        result.z = key[count - 1].z;
        return result;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        a.x = key[-1].x;
        a.y = key[-1].y;
        a.z = key[-1].z;
        b.x = key->x;
        b.y = key->y;
        b.z = key->z;
        func_0020EF60(&a, &a, (1.0f) - u);
        func_0020EF60(&b, &b, u);
        func_0020EEF8(&result, &a, &b);
    }
    return result;
}

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 value;
    /* 0x10 */ f32 time;
} ScalarKey; /* 0x14 bytes */

/* Evaluates the scalar key track 1 of the current cinema at time t, then rescales and clamps it to [0, 1]. */
f32 func_00410A74(f32 t) {
    s32 *track;
    ScalarKey *key;
    s32 count;
    f32 u;
    f32 v;

    track = func_002017D4(D_800C1BB0->resource, 1);
    count = track[1];
    key = (ScalarKey *) (track + 2);
    if (count == 0) {
        v = 0.0f;
    } else if (t <= key[0].time) {
        v = key[0].value;
    } else if (key[count - 1].time <= t) {
        v = key[count - 1].value;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        v = key[-1].value * (1.0f - u) + key->value * u;
    }
    v *= 2.0f * D_800C1BC8 + 1.0f;
    v -= D_800C1BC8;
    if (1.0f < v) {
        v = 1.0f;
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    return v;
}

f32 func_00410BB4(f32 t) {
    s32 *track;
    D_800C7470_Pair *key;
    s32 count;
    s32 i;
    f32 u;
    track = func_002017D4(D_800C1BB0->resource, 3);
    key = (D_800C7470_Pair *)(track + 2);
    count = track[1];
    for (i = 0; i < count; i++) {
    }
    if (count == 0) {
        return 47.5f;
    }
    if (t <= key[0].second) {
        return key[0].first;
    }
    if (key[count - 1].second <= t) {
        return key[count - 1].first;
    }
    while (key->second < t) {
        key++;
    }
    u = (t - key[-1].second) / (key->second - key[-1].second);
    u = u * (u * 3.0f) - 2.0f * u * u * u;
    return key[-1].first * (1.0f - u) + key->first * u;
}

typedef struct {
    f32 m[4][4];
} Mtx4f;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ f32 yaw;
    /* 0x2C */ f32 pitch;
    /* 0x30 */ u8 pad30[0x38 - 0x30];
    /* 0x38 */ Vec3 eye;
} Camera;

extern u8 D_800F1B40[];   /* collision info */
extern Vec3 D_800F1CE8;   /* collision point */

extern s32 Cinema_PathPlaying(void);
extern Vec3 func_0041057C(f32 t); /* eye path */
extern Vec3 func_00410724(f32 t); /* target path */
extern void func_0020F79C(Mtx4f *m);                         /* identity */
extern void func_0020FE00(Mtx4f *m, f32 x, f32 y, f32 z);    /* rotation */
extern void func_0020F85C(Mtx4f *m, Vec3 *in, Vec3 *out);    /* transform */
extern void func_002216DC(void *world);
extern s32 func_00239F00(Actor *actor, Vec3 to, void *info);
extern void func_00226724(void *world);

/* Places the cinema relative to its target (flag 1) or to the player (flag 2). */
static __inline__ void Cinema_Anchor(Actor *player) {
    if ((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) {
        D_800C1BB0->position = D_800C1BB0->target->head.pos;
        D_800C1BB0->yaw = D_800C1BB0->target->yaw + 1.5707964f;
    }
    if (D_800C1BB0->spec.flags & 2) {
        if (player != NULL) {
            D_800C1BB0->position = player->head.pos;
            D_800C1BB0->yaw = player->yaw;
        } else {
            D_800C1BB0->position.x = 0.0f;
            D_800C1BB0->position.y = 0.0f;
            D_800C1BB0->position.z = 0.0f;
            D_800C1BB0->yaw = 0.0f;
        }
    }
}

/* Drives the camera along the cinema paths. */
void func_00410CE8(Camera *cam) {
    Vec3 eye;
    Vec3 target;
    Vec3 dir;
    ActorHead saved;
    Mtx4f mtx;
    Vec3 tmp;
    Actor *player;
    s32 hit;
    f32 yaw;

    if (!D_800C1BB0->active || !Cinema_PathPlaying()) {
        return;
    }
    if (D_800C1BB0->time < D_800C1BB0->start || D_800C1BB0->end < D_800C1BB0->time) {
        return;
    }
    eye = func_0041057C(D_800C1BB0->time);
    target = func_00410724(D_800C1BB0->time);
    func_0020EF60(&eye, &eye, 10.24f);
    func_0020EF60(&target, &target, 10.24f);
    cam->yaw = 0.0f;
    func_0020EF2C(&dir, &target, &eye);
    if (((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) || (D_800C1BB0->spec.flags & 2)) {
        player = D_80119890;
        Cinema_Anchor(player);
        func_0020F79C(&mtx);
        func_0020FE00(&mtx, 0.0f, D_800C1BB0->yaw, 0.0f);
        func_0020F85C(&mtx, &target, &tmp);
        target = tmp;
        func_0020F85C(&mtx, &eye, &tmp);
        eye = tmp;
        func_0020EEF8(&target, &target, &D_800C1BB0->position);
        func_0020EEF8(&eye, &eye, &D_800C1BB0->position);
        func_0020EF2C(&dir, &target, &eye);
        if (player != NULL) {
            saved = player->head;
            player->head.pos = target;
            func_002216DC(D_800F7078);
            hit = func_00239F00(player, eye, D_800F1B40);
            func_00226724(D_800F7078);
            player->head = saved;
            if (hit) {
                eye = D_800F1CE8;
            }
        }
    }
    yaw = func_0020EAA0(&dir);
    cam->pitch = -func_0020F5C0(&dir);
    cam->yaw += yaw;
    cam->eye = eye;
}

extern void func_0020B5B8(void);

void func_004110CC(s32 inset) {
    f32 left;
    f32 top;
    f32 width;
    f32 height;
    func_0020B5B8();
    left = 0.0f;
    top = (f32)inset;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x0)) << 20))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->words.w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (3) - (29)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((29) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((( (((unsigned int)(0x1)) << (3 + 3)) | (((unsigned int)(0x2)) << (3 + 5)) | (((unsigned int)(0x0)) << (3 + 7)) | (((unsigned int)(0x1)) << (3 + 11)) | ( (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (30)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (26)) | (((unsigned int)(((unsigned int)(0x1))) & (0xFFFFFFFFU >> (32 - (2)))) << (22)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) ) )) | (( (((unsigned int)(0x1)) << (3 + 3)) | (((unsigned int)(0x2)) << (3 + 5)) | (((unsigned int)(0x0)) << (3 + 7)) | (((unsigned int)(0x1)) << (3 + 11)) | ( (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (28)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (24)) | (((unsigned int)(((unsigned int)(0x1))) & (0xFFFFFFFFU >> (32 - (2)))) << (20)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (16)) ) )))); };
    width = (f32)D_800C2030;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    height = (f32)(D_800C2034 - inset * 2);
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((width - 1.0f)) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)((top + (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((left + (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - 1.0f)) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(((left + width) - (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x3)) << 20))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->words.w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (3) - (29)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((29) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((0) | (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = ((unsigned int)(0x10001)); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - 1.0f)) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((left + width)) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
}

s32 func_00207FEC(s32 a);
s32 func_00207A70(s32 a);

void func_00411C94(void) {
    f32 fs;
    f32 k;
    f32 q;
    s32 sh;
    fs = ((f32)D_800C2034 - (f32)D_800C2030 / D_800C1BB0->spec.aspect) * 0.5f;
    q = 0.3f;
    k = 1.0f;
    if (D_800C1BB0->time < D_800C1BB0->start) {
        k = D_800C1BB0->start - D_800C1BB0->time;
        k = k / q;
        if (1.0f < k) {
            k = 1.0f;
        }
        k = 1.0f - k;
    }
    if (D_800C1BB0->time > D_800C1BB0->end) {
        k = D_800C1BB0->time - D_800C1BB0->end;
        k = k / q;
        if (1.0f < k) {
            k = 1.0f;
        }
        k = 1.0f - k;
    }
    fs = fs * k;
    { Gfx *_g = D_800D8E24++; _g->words.w0 = (0xE7000000); _g->words.w1 = (0); }
    { Gfx *_g = D_800D8E24++; _g->words.w0 = (0xE3000A01); _g->words.w1 = (0); }
    func_00207FEC(0x13);
    func_00207A70(0x19);
    { Gfx *_g = D_800D8E24++; _g->words.w0 = (0xFA00FFFF); _g->words.w1 = (0xFF); }
    { Gfx *_g = D_800D8E24++; _g->words.w0 = (0xF6000000 | ((((D_800C2030) > (0) ? (D_800C2030) : (0)) & 0x3FF) << 14) | (((((s32)fs) > (0) ? ((s32)fs) : (0)) & 0x3FF) << 2)); _g->words.w1 = (0); }
    { Gfx *_g = D_800D8E24++; _g->words.w0 = (0xF6000000 | ((((D_800C2030) > (0) ? (D_800C2030) : (0)) & 0x3FF) << 14) | ((((D_800C2034) > (0) ? (D_800C2034) : (0)) & 0x3FF) << 2)); _g->words.w1 = (({ f32 _y = (f32)D_800C2034 - fs; if (!(0.0f <= _y)) _y = 0.0f; (((u32)_y) & 0x3FF) << 2; })); }
    sh = (s32)fs;
    func_004110CC(sh);
}

typedef struct {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ f32 x;
    /* 0x28C */ f32 y;
    /* 0x290 */ f32 width;
    /* 0x294 */ f32 height;
} ScreenRectangle;

extern u8 D_800F6CB0[];
extern void func_002883E0(void *ctx, u8 *rgba, s32 left, s32 top, s32 right, s32 bottom);

/* Fills the cinema's screen rectangle with the colour and alpha of the current time. */
void func_00411EF8(ScreenRectangle *rectangle) {
    Vec3 color;
    Vec3 baseColor;
    u8 rgba[4];
    u32 width;
    u32 height;
    u32 right;
    u32 bottom;
    f32 alpha;
    f32 scale;
    f32 value;

    if (!D_800C1BB0->active) {
        if (!D_800C1BC4) {
            return;
        }
    } else {
        width = rectangle->width;
        height = rectangle->height;
        right = (f32) width + rectangle->x;
        bottom = (f32) height + rectangle->y;
        baseColor = func_004108CC(D_800C1BB0->time);
        alpha = func_00410A74(D_800C1BB0->time);
        color = baseColor;
        if (D_800C1BB0->fading) {
            scale = D_800C1BB0->fade;
            if (scale < 0.0f) {
                scale = 0.0f;
            }
            color.x = baseColor.x * scale;
            color.y = baseColor.y * scale;
            color.z = baseColor.z * scale;
            alpha += 1.0f - scale;
        }
        value = color.x * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[0] = (u32) value;
        value = color.y * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[1] = (u32) value;
        value = color.z * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[2] = (u32) value;
        value = alpha * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[3] = (u32) value;
        func_002883E0(D_800F6CB0, rgba, width, height, right, bottom);
    }
    func_00411C94();
}

/* Finds the last cinema entry whose ids match the key's and makes it pending, with owner as its target. */
void func_00412314(void *owner, CueObject *obj) {
    s32 size;
    s32 **table;
    s32 key;
    s32 found;
    s32 i;
    s32 n;
    CinemaEntry *entry;
    s32 id4;
    s32 idA;

    found = -1;
    if (D_800C1BB8 != 0) {
        id4 = obj->id4;
        idA = obj->idA;
        key = func_0020185C(D_800F70D8, D_800F709C, 0, &size);
        if (key == 0) {
            table = NULL;
        } else {
            table = func_0020367C(0, key, size, 0x13, 0, NULL, "Cinema", 1);
        }
        n = *D_800F70D8 - 1;
        entry = (CinemaEntry *) (*table + 2);
        for (i = 0; i < n; i++, entry++) {
            if (entry->id4 == id4 && entry->id0 == idA) {
                found = i;
            }
        }
        if (found != -1) {
            D_800C1BB0->target = owner;
            D_800C1BB0->targetD8 = idA;
            D_800C1BB0->resourceDC = found;
            D_800C1BB0->field58 = 1;
        }
        func_002051F4(0, table);
    }
}

/* Makes every cinema entry with id `key` pending; returns the resulting target id. */
s32 func_00412438(s32 key) {
    s32 size;
    s32 **table;
    s32 loaded;
    s32 i;
    s32 n;
    CinemaEntry *entries;

    if (D_800C1BB8 == 0) {
        return 0;
    }
    loaded = func_0020185C(D_800F70D8, D_800F709C, 0, &size);
    if (loaded == 0) {
        table = NULL;
    } else {
        table = func_0020367C(0, loaded, size, 0x13, 0, NULL, "Cinema", 1);
    }
    i = 0;
    entries = (CinemaEntry *) (*table + 2);
    n = *D_800F70D8 - 1;
    for (; i < n; i++) {
        if (entries[i].id == key) {
            if ((entries[i].flags & 3) == 0) {
                D_800C1BB0->targetD8 = entries[i].id0;
            }
            D_800C1BB0->request = key;
            D_800C1BB0->resourceDC = i;
            D_800C1BB0->field58 = 1;
        }
    }
    func_002051F4(0, table);
    return D_800C1BB0->targetD8;
}

extern s32 func_00288780(const char *a, const char *b); /* strcmp */

/* Makes the cinema entry called `name` pending. */
void func_00412548(const char *name) {
    s32 size;
    s32 **table;
    s32 loaded;
    s32 i;
    s32 id;
    s32 n;
    CinemaEntry *entries;

    if (D_800C1BB8 != 0) {
        loaded = func_0020185C(D_800F70D8, D_800F709C, 0, &size);
        if (loaded == 0) {
            table = NULL;
        } else {
            table = func_0020367C(0, loaded, size, 0x13, 0, NULL, "Cinema", 1);
        }
        i = 0;
        entries = (CinemaEntry *) (*table + 2);
        n = *D_800F70D8 - 1;
        for (; i < n; i++) {
            if (func_00288780(name, entries[i].name) == 0) {
                D_800C1BB0->request = entries[i].id;
                id = entries[i].id0;
                D_800C1BB0->resourceDC = i;
                D_800C1BB0->field58 = 1;
                D_800C1BB0->targetD8 = id;
                break;
            }
        }
        func_002051F4(0, table);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} CueRef;

s32 func_0041266C(void) {
    s32 any = 0;
    s32 i;
    Track *n = func_002017D4(D_800C1BB0->resource, 2);
    s32 cnt = n->count;
    CueRef *ents = (CueRef *)((u8 *)n + 8);
    CueRef *e;
    CueObject *k;
    for (i = 0; i < cnt; i++) {
        e = &ents[i];
        if (e->unk4 == 0) {
            k = (CueObject *)(D_800F7078->m_pRecords + e->unk8);
        } else {
            k = (CueObject *)(D_800F7078->m_pAltRecords + e->unk8);
        }
        if (func_004131B0(k) != 0) {
            any = 1;
        }
    }
    return any ^ 1;
}

typedef struct {
    f32 time;
    s32 unk4;
    s32 unk8;
} TimedCueRef;

s32 func_00284174(void);
void func_0026E210(CueObject *k);
void func_00224A68(CWorld *g, CueObject *k, s32 z);

void func_00412798(void) {
    f32 now = D_800C1BB0->time;
    s32 flag = 0;
    Track *n;
    TimedCueRef *e;
    TimedCueRef *ents;
    CueObject *k;
    s32 cnt;
    s32 i;
    if (func_00284174() != 0) {
        if (D_800C1BB0->field50 != 0) {
            n = func_002017D4(D_800C1BB0->resource, 2);
            cnt = n->count;
            ents = (TimedCueRef *)((u8 *)n + 8);
            if (cnt != 0) {
                if (D_800C1BB0->end <= now) {
                    flag = 1;
                }
                for (i = 0; i < cnt; i++) {
                    e = &ents[i];
                    if (e->time <= -10000.0f) {
                        continue;
                    }
                    if (now < e->time) {
                        if (flag == 0) {
                            continue;
                        }
                    }
                    if (e->unk4 == 0) {
                        k = (CueObject *)(D_800F7078->m_pRecords + e->unk8);
                    } else {
                        k = (CueObject *)(D_800F7078->m_pAltRecords + e->unk8);
                    }
                    if (func_004131B0(k) != 0) {
                        func_00412314(0, k);
                        D_800C1BB0->field58 = 0;
                    } else {
                        func_0026E210(k);
                    }
                    e->time = -20000.0f;
                    k->unkE = (k->unkE | 1) & 0xEF;
                    func_00224A68(D_800F7078, k, 0);
                }
            }
        }
    }
}

static __inline__ Vec3 getA(void) {
    Vec3 t;
    Vec3 v;
    Track *n;
    if (D_800C1BB0->active == 0) {
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    n = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), 0), 0);
    t = func_004101B8((SplineNode *)((u8 *)n + 8), n->count, 0.0f);
    return t;
}
static __inline__ Vec3 getB(void) {
    Vec3 t;
    Vec3 v;
    Track *n;
    if (D_800C1BB0->active == 0) {
        v.x = 10.0f;
        v.y = 0;
        v.z = 0;
        return v;
    }
    n = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), 0), 1);
    t = func_004101B8((SplineNode *)((u8 *)n + 8), n->count, 0.0f);
    return t;
}
Vec3 func_00412998(void) {
    Vec3 a;
    Vec3 b;
    f32 dir[4];
    Vec3 res;
    if (D_800C1BB0->active == 0) {
        res.x = 0;
        res.y = 0;
    } else {
        a = getA();
        b = getB();
        func_0020EF2C((Vec3 *) dir, &b, &a);
        res.x = -func_0020F5C0((Vec3 *) dir);
        res.y = func_0020EAA0((Vec3 *) dir);
    }
    do {
        res.z = 0;
    } while (0);
    return res;
}

void func_00412B14(void) {
    D_800C1BB0->fieldB4 = 1;
}

void func_00412B28(s32 a0) {
    register s32 a asm("$7");
    register u8 *bits asm("$4");
    register s32 id asm("$5");
    u8 b;
    s32 m;
    s32 i;
    a = a0;
    bits = (u8 *) D_80119890;
    if (D_800C1BB8 != 0) {
        id = a - 0x8D;
        if ((u32)id < 6 && bits != 0) {
            bits += 0x9F4;
            i = id >> 3;
            m = 1 << (id & 7);
            b = bits[i];
            if ((b & m) != 0) return;
            bits[i] = b | m;
        }
        func_00412438(a);
    }
}

Vec3 func_00412B9C(void) {
    Vec3 sp10;
    Vec3 sp20;
    StridedTable *temp_v0;
    if (D_800C1BB0->active == 0) {
        sp20.x = 0;
        sp20.y = 0;
        sp20.z = 0;
        return sp20;
    }
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), 0), 0);
    sp10 = func_004101B8((SplineNode *) temp_v0->data, temp_v0->count, 0.0f);
    return sp10;
}

Vec3 func_00412C4C(void) {
    Vec3 sp10;
    Vec3 sp20;
    StridedTable *temp_v0;
    if (D_800C1BB0->active == 0) {
        sp20.y = 0;
        sp20.z = 0;
        sp20.x = 10.0f;
        return sp20;
    }
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), 0), 1);
    sp10 = func_004101B8((SplineNode *) temp_v0->data, temp_v0->count, 0.0f);
    return sp10;
}

f32 func_00412D04(void) {
    f32 temp_f0;
    f32 var_f20;
    s32 *temp_v0;
    s32 var_s0;
    var_f20 = 0.0f;
    var_s0 = 0;
loop_1:
    if (var_s0 < *(s32 *)func_002017D4(D_800C1BB0->resource, 0)) {
        temp_v0 = func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), var_s0), 0);
        temp_v0 = (s32 *)((char *)temp_v0 + temp_v0[1] * 0x24);
        temp_f0 = *(f32 *)temp_v0;
        var_s0 += 1;
        var_f20 += temp_f0;
        goto loop_1;
    }
    return var_f20;
}

s32 func_00412DA4(void) {
    return *(s32 *)func_002017D4(D_800C1BB0->resource, 0);
}

Spline *func_00412DD0(s32 index) {
    return func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), index), 0);
}

void *func_00412E1C(void) {
    return func_002017D4(D_800C1BB0->resource, 1);
}

s32 func_00412E44(s32 arg0) {
    return arg0 + 8;
}

u32 func_00412E4C(u8 *bits, s32 index) {
    s32 byte = index >> 3;
    u32 mask;
    index &= 7;
    mask = 1;
    bits += byte;
    return *bits & (mask << index);
}

void func_00412E6C(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    s32 temp_a3;
    u8 *temp_v0;
    u8 *temp_v1;

    temp_a3 = arg1 >> 3;
    temp_a0 = 1 << (arg1 & 7);
    if (arg2 != 0) {
        temp_v1 = (u8 *) arg0 + temp_a3;
        *temp_v1 |= temp_a0;
        return;
    }
    temp_v0 = (u8 *) arg0 + temp_a3;
    *temp_v0 &= ~temp_a0;
}

/* Allocates a temporary block of `size` bytes; the word before the data's 0x10-byte header points back at the handle. */
void *func_00412EB0(s32 size) {
    s32 **handle;
    s32 *block;

    handle = func_00204EDC(0, size + 0x10, 0x13, "Cinema temp alloc");
    if (handle != NULL) {
        block = *handle;
        *block = (s32) handle;
        return block + 4;
    }
    return NULL;
}

void func_00412EF4(void *arg0) {
    if (arg0 != NULL) {
        func_002052D8(0, ((s32 **) arg0)[-4]);
    }
}

s32 func_00412F18(void *object) {
    struct IntPair *first = func_002017D4(object, 0);
    struct IntPair *second = func_002017D4(object, 2);
    s32 size = first->field_4 * 36;
    return size + second->field_4 * 36 + 0x30;
}

s32 func_00412F74(void) {
    return *D_800F70D8 - 1;
}

/* Loads the cinema table. */
s32 **func_00412F88(void) {
    s32 size;
    s32 loaded;
    CWorld *resources = D_800F7078;

    loaded = func_0020185C(resources->m_pCinemas, resources->m_CinemasKey, 0, &size);
    if (loaded == 0) {
        return NULL;
    }
    return func_0020367C(0, loaded, size, 0x13, 0, NULL, "Cinema", 1);
}

/* A Hermite basis term, (1 - t)^3 - (1 - t) and t^3 - t, weighted. */
f32 func_00412FF8(f32 a, f32 b, f32 t) {
    f32 u;
    u = 1.0f - t;
    return (a * ((u * u * u) - u)) + (b * ((t * t * t) - t));
}

f32 func_00413038(f32 a, f32 b, f32 t) {
    return (a * (1.0f - t)) + (b * t);
}

void *func_0041305C(s32 index) {
    return func_002017D4(func_002017D4(D_800C1BB0->resource, 0), index);
}

s32 func_0041309C(s32 index) {
    return *(s32 *)func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), index), 2);
}

s32 func_004130EC(s32 index) {
    return ((s32 *)func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), index), 2))[1];
}

void *func_0041313C(s32 index) {
    return func_002017D4(func_002017D4(func_002017D4(D_800C1BB0->resource, 0), index), 1);
}

void *func_00413188(void) {
    return func_002017D4(D_800C1BB0->resource, 3);
}

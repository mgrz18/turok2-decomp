/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00257DF4.c, func_00257DF4, with Turok 2's layouts. */
#include "common.h"

/* Plays a sound or a music cue at pos: refuses while another cue is pending,
 * when 6 of the 17 voices already play this sound id (or for cue 0x44C while
 * it is muted). Picks one of the id's variants at random by weight (a sound
 * id avoids repeating its last variant), then starts each of its layers under
 * a new play handle, which it returns (-1 when nothing plays). */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 q[0x18];
    /* 0x18 */ u8 pad18[0x4];
    /* 0x1C */ s32 count;
} SndLock;

typedef struct {
    /* 0x00 */ u8 pad0[0xC8];
    /* 0xC8 */ s32 sound;
} SndVoice;

typedef struct {
    /* 0x00 */ s32 stride;
    /* 0x04 */ u32 count;
} SndLayers;

typedef struct {
    /* 0x0000 */ u8 pad0[0x102];
    /* 0x0102 */ s16 pendingCue;
    /* 0x0104 */ u8 pad104[0x10C - 0x104];
    /* 0x010C */ s32 handle;
    /* 0x0110 */ SndLock lock;
    /* 0x0130 */ s32 lastVariant;
    /* 0x0134 */ u8 pad134[0x1D9C - 0x134];
    /* 0x1D9C */ SndVoice voices[17];
    /* 0x2B28 */ u8 pad2B28[0x2B4C - 0x2B28];
    /* 0x2B4C */ void *variants;
    /* 0x2B50 */ u8 pad2B50[0x2B54 - 0x2B50];
    /* 0x2B54 */ s32 rangeCount;
    /* 0x2B58 */ void *ranges;
    /* 0x2B5C */ s32 *weights;
    /* 0x2B60 */ u8 pad2B60[0x2B88 - 0x2B60];
    /* 0x2B88 */ void *cue;
    /* 0x2B8C */ s16 cueId;
    /* 0x2B8E */ u8 pad2B8E[0x2B90 - 0x2B8E];
    /* 0x2B90 */ s32 cueArg;
    /* 0x2B94 */ s8 cueVolume;
    /* 0x2B95 */ u8 pad2B95[0x2BB8 - 0x2B95];
    /* 0x2BB8 */ s32 muted;
} SndMgr;

extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern s32 osSendMesg(void *queue, void *msg, s32 flag);
extern u32 func_0029DFF0(void); /* __osDisableInt */
extern void func_0029E010(u32 mask); /* __osRestoreInt */
extern s32 func_002113A4(void);
extern f32 func_0021149C(f32 range);
extern s32 func_00266D8C(void *ranges, s32 count, s16 id, s32 *lo, s32 *hi);
extern void *func_002017D4(void *list, s32 index);
extern void func_00270A60(SndMgr *m, void *layer, Vec3f *pos, s32 variant, s32 handle, s32 a5, s32 a6);

static __inline__ void SndMgr_Lock(SndMgr *m) {
    SndLock *l = &m->lock;
    u32 mask = func_0029DFF0();

    if (++l->count != 1) {
        func_0029E010(mask);
        osRecvMesg(l, 0, 1);
    } else {
        func_0029E010(mask);
    }
}

static __inline__ void SndMgr_Unlock(SndMgr *m) {
    SndLock *l = &m->lock;
    u32 mask = func_0029DFF0();

    if (--l->count != 0) {
        func_0029E010(mask);
        osSendMesg(l, 0, 1);
    } else {
        func_0029E010(mask);
    }
}

s32 func_00270C80(SndMgr *m, s32 id, Vec3f pos, s32 a5, s32 a6) {
    s32 i;
    s32 j;
    s32 busy;
    s32 count;
    s32 sum;
    s32 r;
    s32 thresh;
    s32 lo;
    s32 hi;
    s32 lo2;
    s32 hi2;
    s32 idx;
    s32 sh;
    s32 ret;
    s32 n;
    u32 layers;
    s16 last;
    SndLayers *nd;

    if (m->muted != 0 && id == 0x44C) {
        busy = 1;
        goto chk;
    }
    if (m->pendingCue != -1) {
        busy = 1;
        goto chk;
    }
    count = 0;
    if (id < 0x100) {
        for (j = 0; j < 17; j++) {
            if (m->voices[j].sound == id) {
                count++;
            }
        }
        busy = 1;
        if (count >= 6) {
            goto chk;
        }
    }
    busy = 0;
chk:
    if (busy != 0) {
        return -1;
    }

    if (id < 0x100) {
        last = *(u16 *)((u8 *)m + 0x132);
        sum = 0;
        if (m->rangeCount != 0) {
            if (func_00266D8C(m->ranges, m->rangeCount, id, &lo, &hi) != 0) {
                i = lo;
                if (i != hi) {
                    for (count = lo; count <= hi; count++) {
                        sum += m->weights[count] * 10;
                    }
                    r = func_002113A4() % sum;
                    thresh = r;
                    sum = 0;
                    for (i = lo; i < hi; i++) {
                        sum += m->weights[i] * 10;
                        if (sum >= thresh) {
                            break;
                        }
                    }
                    if (last != 0 && i == m->lastVariant) {
                        if (i == hi) {
                            i = i - 1;
                        } else {
                            i = i + 1;
                        }
                    }
                }
                idx = i;
            } else {
                idx = -1;
            }
        } else {
            idx = -1;
        }
        m->cueVolume = func_0021149C(127.0f);
    } else {
        sum = 0;
        if (m->rangeCount != 0) {
            if (func_00266D8C(m->ranges, m->rangeCount, id, &lo2, &hi2) != 0) {
                i = lo2;
                if (i != hi2) {
                    for (r = lo2; r <= hi2; r++) {
                        sum += m->weights[r] * 10;
                    }
                    r = func_002113A4() % sum;
                    thresh = r;
                    sum = 0;
                    for (i = lo2; i < hi2; i++) {
                        sum += m->weights[i] * 10;
                        if (sum >= thresh) {
                            break;
                        }
                    }
                }
                idx = i;
            } else {
                idx = -1;
            }
        } else {
            idx = -1;
        }
    }
    if (idx == -1) {
        return -1;
    }
    sh = (s16)idx;
    m->cue = func_002017D4(func_002017D4(m->variants, sh), 0);
    m->cueId = id;
    m->cueArg = a6;
    nd = (SndLayers *)func_002017D4(func_002017D4(m->variants, sh), 1);
    layers = nd->count;
    if (layers != 0) {
        SndMgr_Lock(m);
        ret = (m->handle = m->handle + 1);
        SndMgr_Unlock(m);
        if (id < 0x100) {
            m->lastVariant = idx;
        }
    } else {
        ret = -1;
    }
    n = 0;
    if (layers != 0) {
        do {
            func_00270A60(m, (u8 *)nd + (n * nd->stride + 8), &pos, idx, ret, 0, a5);
            n++;
        } while ((u32)n < layers);
    }
    m->cueArg = -1;
    return ret;
}

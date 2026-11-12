/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00257DF4.c, func_00257DF4, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/sound.h"
#include "structs/table.h"
#include "protos/iset.h"
#include "protos/ultra.h"

/* Plays a sound or a music cue at pos: refuses while another cue is pending,
 * when 6 of the 17 voices already play this sound id (or for cue 0x44C while
 * it is muted). Picks one of the id's variants at random by weight (a sound
 * id avoids repeating its last variant), then starts each of its layers under
 * a new play handle, which it returns (-1 when nothing plays). */

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern s32 Random_Next(void);
extern f32 Random_Float(f32 range);
extern s32 BinaryRange(void *ranges, s32 count, s16 id, s32 *lo, s32 *hi);
extern void CAudioWorld__PlayLayer(CAudioWorld *m, void *layer, CVector3 *pos, s32 variant, s32 handle, s32 a5, sptr a6);

static __inline__ void CAudioWorld_Lock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();

    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(l, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static __inline__ void CAudioWorld_Unlock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();

    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(l, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

s32 CAudioWorld__PlaySound(CAudioWorld *m, s32 id, CVector3 pos, sptr a5, s32 a6) {
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
    StrideTable *nd;

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
            if (m->pool.voices[j].soundId == id) {
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
#ifdef TARGET_PORT
        last = (u16) m->lastVariant; /* the N64's 0x132: the low half of the big-endian word (round 12 a5) */
#else
        last = *(u16 *)((u8 *)m + 0x132);
#endif
        sum = 0;
        if (m->rangeCount != 0) {
            if (BinaryRange(m->ranges, m->rangeCount, id, &lo, &hi) != 0) {
                i = lo;
                if (i != hi) {
                    for (count = lo; count <= hi; count++) {
                        sum += m->weights[count] * 10;
                    }
                    r = Random_Next() % sum;
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
        m->cueVolume = Random_Float(127.0f);
    } else {
        sum = 0;
        if (m->rangeCount != 0) {
            if (BinaryRange(m->ranges, m->rangeCount, id, &lo2, &hi2) != 0) {
                i = lo2;
                if (i != hi2) {
                    for (r = lo2; r <= hi2; r++) {
                        sum += m->weights[r] * 10;
                    }
                    r = Random_Next() % sum;
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
    m->cue = CISet__GetBlock(CISet__GetBlock(m->variants, sh), 0);
    m->cueId = id;
    m->cueArg = a6;
    nd = CISet__GetBlock(CISet__GetBlock(m->variants, sh), 1);
    layers = nd->count;
    if (layers != 0) {
        CAudioWorld_Lock(m);
        ret = (m->handle = m->handle + 1);
        CAudioWorld_Unlock(m);
        if (id < 0x100) {
            m->lastVariant = idx;
        }
    } else {
        ret = -1;
    }
    n = 0;
    if (layers != 0) {
        do {
            CAudioWorld__PlayLayer(m, nd->entries + n * nd->stride, &pos, idx, ret, 0, a5);
            n++;
        } while ((u32)n < layers);
    }
    m->cueArg = -1;
    return ret;
}

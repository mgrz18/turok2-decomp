/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/8025D470.CPP, func_8025D450_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"
#include "protos/iset.h"
#include "protos/libaudio.h"

s32 BinarySearch(void *table, s32 count, sptr key); /* code/BinaryRange */
f32 HalfToFloat(s32 half); /* code/HalfToFloat */
struct Track;
typedef struct Track Track;
struct Resource;
typedef struct Resource Resource;
struct Resource {
    s32 unk0;
    u16 unk4;
};
struct Track {
    struct ContextRecAt2BB8 *unk0;
    s32 unk4;
    struct IntPair *unk8;
    void *unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
};

static __inline__ f32 fade_rate(Track *track,f32 time,f32 rate) {track->unk30=time;return (f32)track->unk20/(time*rate);}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_Update) */
void SndSeq_Update(Track *arg0) {
    f32 temp_f0;
    f32 temp_f20;
    f32 var_f20;
    f32 var_f2;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_s0;
    s32 var_v0;
    void *temp_s0_2;
    ContextRecAt2BB8 *temp_v0_2;
    struct Resource *temp_v0_4;
    ContextRecAt2BB8 *temp_v0_5;
    void *var_v0_2;
    temp_v1 = arg0->unk18;
    switch (temp_v1) {
    case 0x20:
        if (arg0->unk1C & 4) {
            arg0->unk1C = 0;
            sSndSeqCurrentId = -1;
            if (arg0->unk28 >= 0) {
                arg0->unk18 = 0x40;
                return;
            }
            arg0->unk18 = 0x10;
            return;
        }
        return;
    case 0x100:
        if (arg0->unk1C & 8) {
            arg0->unk18 = 0x80;
            arg0->unk1C = 0;
            return;
        }
        temp_v0 = sSndSeqLoadRetries + 1;
        sSndSeqLoadRetries = temp_v0;
        if (temp_v0 >= 0xA) {
            sSndSeqCurrentId = -1;
            arg0->unk18 = 0x40;
            return;
        }
        break;
    case 0x40:
        temp_a2 = arg0->unk28;
        var_v0 = 0x10;
        if (temp_a2 != -1) {
            temp_v0_2 = arg0->unk0;
            temp_v0_3 = BinarySearch((void *)temp_v0_2->unk2B60, temp_v0_2->unk2B64, temp_a2);
            temp_s0 = temp_v0_3;
            if (temp_s0 != -1) {
                temp_s0 *= 2;
                temp_s0_2 = CISet__GetBlock(arg0->unk0->unk2B50, temp_s0 | 1);
                temp_v0_4 = CISet__GetBlock(arg0->unk0->unk2B50, temp_s0);
                arg0->unk24 = (s32) temp_v0_4->unk0;
                temp_f0 = HalfToFloat(temp_v0_4->unk4);
                arg0->unk30 = temp_f0;
                if (temp_f0 <= 0.0f) {
                    arg0->unk30 = 0.1f;
                }
                var_v0_2 = temp_s0_2;
            } else {
                var_v0_2 = ((void *)0);
            }
            arg0->unkC = var_v0_2;
            if (var_v0_2 == ((void *)0)) {
                arg0->unk18 = 0x100;
                sSndSeqLoadRetries = 0;
                arg0->unk1C = 0;
                return;
            }
            arg0->unk18 = 0x80;
            arg0->unk4 = 1;
        case 0x80:
            var_s0 = 0;
            alCSeqNew((ALCSeq *)arg0->unk10, (u8 *)arg0->unkC);
            alCSPSetSeq((ALCSPlayer *)arg0->unk14, (ALCSeq *)arg0->unk10);
            alCSPSetBank((ALCSPlayer *)arg0->unk14, (ALBank *)arg0->unk8->field_4);
            do {
                alCSPSetChlFXMix((ALCSPlayer *)arg0->unk14, var_s0 & 0xFF, 0);
                var_s0 += 1;
            } while (var_s0 < 0x14);
            temp_f20 = (f32) arg0->unk24 * (arg0->unk0->unk2BA4 * 0.01f);
            alCSPSetVol((ALCSPlayer *)arg0->unk14, (s16) (s32) (temp_f20 * 32767.0f));
            arg0->unk2C = temp_f20;
            alCSPPlay((ALCSPlayer *)arg0->unk14);
            arg0->unk1C = 0;
            arg0->unk18 = 0x10;
            sSndSeqCurrentId = arg0->unk28;
            return;
        }
        goto block_44;
    case 0x10:
    default:
        if ((sSndSeqCurrentId > 0) && (alCSPGetState((ALCSPlayer *)arg0->unk14) == 0)) {
            sSndSeqCurrentId = 0;
            arg0->unk28 = 0;
        } else {
            if (arg0->unk38 != 0) {
                var_f20 = (f32) arg0->unk24;
                var_f20 *= arg0->unk0->unk2BA4 * 0.01f;
                var_f20 *= arg0->unk3C;
                goto block_37;
            }
            temp_v0_5 = arg0->unk0;
            var_f20 = (f32) arg0->unk24 * (temp_v0_5->unk2BA4 * 0.01f);
            if (temp_v0_5->unk2BB8 != 0) {
                var_f20 *= 0.7f;
            }
            if (var_f20 != arg0->unk2C) {
block_37:
                alCSPSetVol((ALCSPlayer *)arg0->unk14, (s16) (s32) (var_f20 * 32767.0f));
                arg0->unk2C = var_f20;
            }
        }
        if (arg0->unk28 != sSndSeqCurrentId) {
            if (sSndSeqCurrentId > 0) {
                var_f2 = arg0->unk30;
                arg0->unk1C = 2;
                arg0->unk18 = 0x20;
                arg0->unk20 = (s32) (arg0->unk2C * 32767.0f);
                if (!(var_f2 >= 0.1f)) {
                    var_f2 = 0.1f;
                }
                arg0->unk34 = fade_rate(arg0,var_f2,6.0e1f);
                return;
            }
            var_v0 = 0x40;
            arg0->unk1C = 0;
block_44:
            arg0->unk18 = var_v0;
        }
        break;
    }
}
#endif

#ifdef TARGET_PORT
/* The host layout: AudioStateD844 / AudioRootD844 (rw/abu_types.h) are N64-offset views of
 * SndSeq and CAudioWorld (structs/sound.h); the same stores through the real types. */
#include "structs/sound.h"
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void alCSPNew(ALCSPlayer *seqp, ALSeqpConfig *config);
void SndSeq_Construct(SndSeq *state, CAudioWorld *root) {
    ALSeqpConfig params;
    ALCSPlayer *player;
    state->owner = root;
    state->unk18 = 16;   /* mode */
    state->track = -1;   /* index */
    state->flags = 0;
    params.maxVoices = 20;
    params.maxEvents = 192;
    params.maxChannels = 20;
    params.heap = &root->heap;
    params.initOsc = 0;
    params.updateOsc = 0;
    params.stopOsc = 0;
    player = alHeapDBAlloc(0, 0, &root->heap, 1, sizeof(ALCSPlayer));
    state->csp = (sptr)player;
    alCSPNew(player, &params);
    state->unk10 = alHeapDBAlloc(0, 0, &root->heap, 1, sizeof(ALCSeq)); /* allocation */
    state->unk24 = 50;    /* timer */
    state->sliding = 0;   /* counter */
    state->unk30 = 1.0f;  /* first */
    state->fadeStep = 125.0f;
    state->current = 1.0f;
    state->target = 1.0f;
}
#else
sptr alHeapDBAlloc(sptr a, s32 b, void *c, s32 d, s32 e);
void SndSeq_Construct(struct AudioStateD844 *state, struct AudioRootD844 *root) {
    ALSeqpConfig params;
    void *player;
    state->root = root;
    state->mode = 16;
    state->index = -1;
    state->flags = 0;
    params.maxVoices = 20;
    params.maxEvents = 192;
    params.maxChannels = 20;
    params.heap = &state->root->heap;
    params.initOsc = 0;
    params.updateOsc = 0;
    params.stopOsc = 0;
    player = alHeapDBAlloc(0, 0, &state->root->heap, 1, 0x7C);
    state->player = player;
    alCSPNew(player, &params);
    state->allocation = alHeapDBAlloc(0, 0, &state->root->heap, 1, 0xF8);
    state->timer = 50;
    state->counter = 0;
    state->first = 1.0f;
    state->second = 125.0f;
    state->third = 1.0f;
    state->fourth = 1.0f;
}
#endif
/* NEAR MATCH: only differs by the FP-compare hazard nop that the ROM has at the merged
 * "bc1fl" tail (label .L002750E0); cc1 here emits no nop there. */
#include "common.h"
#include "structs/sound.h"

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_UpdateFade) */
void SndSeq_UpdateFade(SndSeq *ch) {
    f32 current;
    f32 target;
    f32 next;

    if (ch->flags & 2) {
        alCSPSetVol((ALCSPlayer *)ch->csp, ch->volume.h.lo);
        ch->volume.w = (f32) ch->volume.w - ch->fadeStep;
        if (ch->volume.w <= 0) {
            ch->volume.w = 0;
            alCSPStop((ALCSPlayer *)ch->csp);
            ch->flags |= 4;
        }
    } else if (ch->sliding) {
        current = ch->current;
        target = ch->target;
        if (target < current) {
            next = current - 1.0f / 60;
            if (!(target <= next)) {
                next = target;
            }
        } else if (current < target) {
            next = current + 1.0f / 60;
            if (!(next <= target)) {
                next = target;
            }
        } else {
            return;
        }
        ch->current = next;
    }
}
#endif

#include "rw/junk_types.h" /* SndNodeRec */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_SelectSong) */
s32 SndSeq_SelectSong(SndSeq *s, s32 key) {
    s32 i;
    SndNodeRec *r;
    SndNodeRec *n;
    sptr res;
    i = BinarySearch((void *)s->owner->table2B60, s->owner->count2B64, key);
    if (i != -1) {
        i <<= 1;
        r = CISet__GetBlock(s->owner->node2B50, i | 1);
        n = CISet__GetBlock(s->owner->node2B50, i);
        s->unk24 = n->w0;
        s->unk30 = HalfToFloat(n->h4);
        if (s->unk30 <= 0.0f) {
            s->unk30 = 0.1f;
        }
        res = (sptr)r;
    } else {
        res = 0;
    }
    s->unkC = res;
    return res != 0;
}
#endif

void SndSeq_FadeOut(SndSeq *s) {
    f32 f;
    s->flags = 2;
    s->unk18 = 0x20;
    s->volume.w = (s32)(s->vol * 32767.0f);
    f = s->unk30;
    if (!(0.1f <= f)) {
        f = 0.1f;
    }
    s->unk30 = f;
    s->track = 0;
    s->fadeStep = (f32)s->volume.w / (f * 6.0e1f);
}

/* Only what this writes. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ s32 unk40;
} Flags38;

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_Duck) */
void SndSeq_Duck(Flags38 *arg0) {
    arg0->unk38 = 1;
    arg0->unk40 = 0;
}
#endif

/* Ends a duck: the music stops sliding and returns to full volume. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_Unduck) */
void SndSeq_Unduck(SndSeq *arg0) {
    arg0->sliding = 0;
    arg0->target = 1.0f;
}
#endif

/* A record that owns a handle at 0x14 (passed to alCSPStop) and an index at 0x28. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 handle;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 index;
} HandleOwner;

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_Stop) */
void SndSeq_Stop(HandleOwner *arg0) {
    arg0->index = -1;
    alCSPStop((ALCSPlayer *)arg0->handle);
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_GetSongData) */
void *SndSeq_GetSongData(SequenceState *state, s32 index) {
    s32 selection;
    s32 section;
    void *result;
    SequenceCommand *command;
    f32 duration;
    selection = BinarySearch((void *)state->resource->lookup, state->resource->count, index);
    if (selection != -1) {
        section = selection * 2;
        result = CISet__GetBlock(state->resource->sections, section | 1);
        command = CISet__GetBlock(state->resource->sections, section);
        state->command = command->command;
        duration = HalfToFloat(command->duration);
        state->duration = duration;
        if (duration <= 0.0f) state->duration = 0.1f;
        return result;
    }
    return ((void *)0);
}
#endif

/* Only what this reads and writes: a voice and its volume. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 voice;
    /* 0x18 */ u8 pad18[0x2C - 0x18];
    /* 0x2C */ f32 volume;
} SoundChannel;

/* Sets the volume (0..1, sent as 0..32767). */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_SetVolume) */
void SndSeq_SetVolume(SoundChannel *arg0, f32 arg1) {
    alCSPSetVol((ALCSPlayer *)arg0->voice, (s16) (s32) (arg1 * 32767.0f));
    arg0->volume = arg1;
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_UpdateVolume) */
void SndSeq_UpdateVolume(SndSeq *s) {
    f32 f;
    if (s->sliding != 0) {
        f = (f32)s->unk24;
        f *= s->owner->volume2BA4 * 0.01f;
        f *= s->current;
    } else {
        f = (f32)s->unk24 * (s->owner->volume2BA4 * 0.01f);
        if (s->owner->muted != 0) {
            f *= 0.7f;
        }
        if (f == s->vol) {
            return;
        }
    }
    alCSPSetVol((ALCSPlayer *)s->csp, f * 32767.0f);
    s->vol = f;
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndSeq_Load) */
void SndSeq_Load(void *arg0)
{
  f32 temp_f20;
  s32 temp_a1;
  s32 var_s0;
  var_s0 = 0;
  alCSeqNew((ALCSeq *)(*((s32 *) (((s8 *) arg0) + 0x10))), (u8 *)(*((s32 *) (((s8 *) arg0) + 0xC))));
  alCSPSetSeq((ALCSPlayer *)(*((s32 *) (((s8 *) arg0) + 0x14))), (ALCSeq *)(*((s32 *) (((s8 *) arg0) + 0x10))));
  alCSPSetBank((ALCSPlayer *)(*((s32 *) (((s8 *) arg0) + 0x14))), (ALBank *)(*((s32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 8)))) + 4))));
  do
  {
    temp_a1 = var_s0 & 0xFF;
    alCSPSetChlFXMix((ALCSPlayer *)(*((s32 *) (((s8 *) arg0) + 0x14))), temp_a1, 0);
    var_s0 += 1;
  }
  while (var_s0 < 0x14);
  temp_f20 = ((f32) (*((s32 *) (((s8 *) arg0) + 0x24)))) * ((*((f32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0)))) + 0x2BA4))) * 0.01f);
  alCSPSetVol((ALCSPlayer *)(*((s32 *) (((s8 *) arg0) + 0x14))), (s16) ((s32) (temp_f20 * 32767.0f)));
  *((f32 *) (((s8 *) arg0) + 0x2C)) = temp_f20;
}
#endif

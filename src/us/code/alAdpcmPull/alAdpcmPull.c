/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/alAdpcmPull.c, alAdpcmPull (libaudio load.c), with Turok 2's layouts. */
#include "common.h"
#include "structs/libaudio.h"
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_CLEARBUFF 2
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define aClearBuffer(pkt, d, c) \
    { \
        Acmd *_a = (Acmd *)(pkt); \
        _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); \
        _a->words.w1 = (u32)(c); \
    }
#define aDMEMMove(pkt, i, o, c) \
    { \
        Acmd *_a = (Acmd *)(pkt); \
        _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24); \
        _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); \
    }
#define aLoadADPCM(pkt, c, d) \
    { \
        Acmd *_a = (Acmd *)(pkt); \
        _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24); \
        _a->words.w1 = (u32)(d); \
    }


u32 AudioDMA_CacheBlock(void *addr, s32 len); /* game DMA cache: returns the physical address of a cached copy */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_9.c, func_802BE514_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
Acmd *_decodeChunk(Acmd *ptr, ALLoadFilter *f, sptr tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

/* alAdpcmPull (libaudio load.c): pull handler of the loader filter for ADPCM wave tables: load the codebook, decode
   whole 16 sample frames (9 bytes each) through _decodeChunk, restarting from the loop start (and merging the
   sections in DMEM) when the request crosses the loop end, and clear what runs past the end of the table. */
Acmd *alAdpcmPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALLoadFilter *f = (ALLoadFilter *)filter;
    s32 nbytes;
    s32 nframes;
    s32 nSam;
    s32 tsam;
    sptr overFlow;
    s32 dmemOut;
    sptr overBytes;
    sptr off;
    s32 dmemAlign;
    s32 looped;
    s32 decoded = 0;
    s32 tmp;
    s16 inp;

    if (outCount == 0)
        return ptr;

    /* the codebook of the wave table goes through the game's DMA cache */
    aLoadADPCM(ptr++, f->bookSize, AudioDMA_CacheBlock((void *)((uptr)f->table->waveInfo.adpcmWave.book + 8), f->bookSize));

    looped = 0;
    inp = 0;
    if (f->loop.end < outCount + f->sample)
        looped = 0 < f->loop.count;
    if (looped)
        nSam = f->loop.end - f->sample;
    else
        nSam = outCount;
    if (f->lastsam)
        dmemAlign = 16 - f->lastsam;
    else
        dmemAlign = 0;
    tsam = nSam - dmemAlign;
    if (tsam < 0)
        tsam = 0;
    nframes = (tsam + 15) >> 4;
    nbytes = nframes * 9;

    if (looped) {
        ptr = _decodeChunk(ptr, f, tsam, nbytes, *outp, inp, f->first);
        if (f->lastsam)
            *outp += f->lastsam << 1;
        else
            *outp += 32;
        f->lastsam = f->loop.start & 0xF;
        f->memin = (sptr)f->table->base + ((f->loop.start >> 4) + 1) * 9;
        f->sample = f->loop.start;
        dmemOut = *outp;
        while (nSam < outCount) {
            tmp = dmemOut + (nframes + 1) * 32;
            dmemOut += nSam << 1;
            dmemAlign = tmp & ~31;
            outCount -= nSam;
            if (f->loop.count != -1 && f->loop.count != 0)
                f->loop.count--;
            nSam = (outCount < f->loop.end - f->loop.start) ? outCount : f->loop.end - f->loop.start;
            tsam = nSam - 16 + f->lastsam;
            if (tsam < 0)
                tsam = 0;
            nframes = (tsam + 15) >> 4;
            ptr = _decodeChunk(ptr, f, tsam, nframes * 9, dmemAlign, inp, f->first | 2);
            aDMEMMove(ptr++, dmemAlign + (f->lastsam << 1), dmemOut, nSam << 1);
        }
        f->lastsam = (outCount + f->lastsam) & 0xF;
        f->sample += outCount;
        f->memin += nframes * 9;
        return ptr;
    } else {
        nSam = nframes << 4;
        overBytes = f->memin + nbytes - ((sptr)f->table->base + f->table->len);
        if (overBytes < 0)
            overBytes = 0;
        overFlow = (overBytes / 9) << 4;
        if (overFlow > nSam + dmemAlign)
            overFlow = nSam + dmemAlign;
        nbytes -= overBytes;
        if (overFlow - (overFlow & 0xF) < outCount) {
            decoded = 1;
            ptr = _decodeChunk(ptr, f, nSam - overFlow, nbytes, *outp, inp, f->first);
            if (f->lastsam)
                *outp += f->lastsam << 1;
            else
                *outp += 32;
            f->lastsam = (outCount + f->lastsam) & 0xF;
            f->sample += outCount;
            f->memin += nframes * 9;
        } else {
            f->lastsam = 0;
            f->memin += nframes * 9;
        }
        if (overFlow) {
            f->lastsam = 0;
            if (decoded)
                off = (dmemAlign + nSam - overFlow) << 1;
            else
                off = 0;
            aClearBuffer(ptr++, off + *outp, overFlow << 1);
        }
    }
    return ptr;
}
Acmd *alRaw16Pull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);

Acmd *alRaw16Pull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    s32 nbytes;
    s32 dramLoc;
    s32 dramAlign;
    s32 dmemAlign;
    sptr overFlow;
    sptr startZero;
    s32 nSam;
    s32 op;
    ALLoadFilter *f = (ALLoadFilter *)filter;
    ALFilter *a = (ALFilter *)filter;
    if (outCount == 0)
        return ptr;
    if ((outCount + f->sample > f->loop.end) && (f->loop.count != 0)) {
        nSam = f->loop.end - f->sample;
        nbytes = nSam << 1;
        if (nSam > 0) {
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(*outp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nbytes + 8 - (nbytes & 0x7)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(dramLoc - dramAlign); };
        } else
            dramAlign = 0;
        *outp += dramAlign;
        f->memin = (sptr)f->table->base + (f->loop.start << 1);
        f->sample = f->loop.start;
        op = *outp;
        while (outCount > nSam) {
            op += (nSam << 1);
            outCount -= nSam;
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            nSam = ((outCount) < (f->loop.end - f->loop.start) ? (outCount) : (f->loop.end - f->loop.start));
            nbytes = nSam << 1;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            if (op & 0x7)
                dmemAlign = 8 - (op & 0x7);
            else
                dmemAlign = 0;
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(op + dmemAlign) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nbytes + 8 - (nbytes & 0x7)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(dramLoc - dramAlign); };
            if (dramAlign || dmemAlign)
                { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(op + dramAlign + dmemAlign) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (((unsigned int)(op) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nSam << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        }
        f->sample += outCount;
        f->memin += (outCount << 1);
        return ptr;
    }
    nbytes = outCount << 1;
    overFlow = f->memin + nbytes - ((sptr)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    if (overFlow > nbytes)
        overFlow = nbytes;
    if (overFlow < nbytes) {
        if (outCount > 0) {
            nbytes -= overFlow;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(*outp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nbytes + 8 - (nbytes & 0x7)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(dramLoc - dramAlign); };
        } else
            dramAlign = 0;
        *outp += dramAlign;
        f->sample += outCount;
        f->memin += outCount << 1;
    } else {
        f->memin += outCount << 1;
    }
    if (overFlow) {
        startZero = (outCount << 1) - overFlow;
        if (startZero < 0)
            startZero = 0;
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(startZero + *outp) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(overFlow); };
    }
    return ptr;
}

#include "globals/adec.h"
#include "protos/ultra.h"

Acmd *alAdecPull(ALLoadFilter *filter, s16 *outp, s32 byteCount, s32 sampleOffset, Acmd *p);
extern sptr adecNextFrame();

#ifndef TARGET_PORT /* the port defines alAdecPull in port/audio/t2audio_adec.c (one object now holds the module) */
Acmd *alAdecPull(ALLoadFilter *filter, s16 *outp, s32 byteCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    s32 nbytes;
    s32 dramAlign;
    s32 dmemAlign;
    s32 nSam;
    s32 op;
    ALLoadFilter *f = filter;
    s32 *stream;
    op = *outp;
    if (f->table->base == 0) {
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(op) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(byteCount << 1); };
        return ptr;
    }
    if (byteCount != 0) {
        stream = &sAdecFrame;
        do {
            nSam = byteCount;
            if (f->sample == 0) {
                sAdecFrame = adecNextFrame();
                if (sAdecFrame == 0) {
                    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(op) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(byteCount << 1); };
                    return ptr;
                }
                osWritebackDCache(sAdecFrame, 0x200);
                f->sample = 0x100;
            }
            if (f->sample < byteCount) {
                nSam = f->sample;
            }
            nbytes = nSam << 1;
            dramAlign = sAdecFrame & 0x7;
            nbytes += dramAlign;
            if (op & 0x7) {
                dmemAlign = 8 - (op & 0x7);
            } else {
                dmemAlign = 0;
            }
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(op + dmemAlign) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nbytes + 8 - (nbytes & 0x7)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)((sAdecFrame & 0x1FFFFFFF) - dramAlign); };
            if (dramAlign || dmemAlign) {
                { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(op + dramAlign + dmemAlign) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (((unsigned int)(op) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nSam << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            }
            byteCount -= nSam;
            op += nSam << 1;
            *stream += nSam << 1;
            f->sample -= nSam;
        } while (byteCount != 0);
    }
    return ptr;
}
#endif /* !TARGET_PORT */

extern void alCopy(unsigned char *source, unsigned char *destination, int count);

s32 alLoadParam(void *filter, s32 paramID, ALWaveTable *param)
{
    ALLoadFilter *a = (ALLoadFilter *)filter;
    ALFilter *f = (ALFilter *)filter;
    switch (paramID) {
        case 5:
            a->table = param;
            a->memin = (sptr)a->table->base;
            a->sample = 0;
            switch (a->table->type) {
                case 0:
                    f->handler = (void *) alAdpcmPull;
                    a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order *
                        a->table->waveInfo.adpcmWave.book->npredictors * 8;
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                        a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                        ((void (*)(void *, void *, s32))alCopy)(a->table->waveInfo.adpcmWave.loop->state,
                                      a->lstate, sizeof(ADPCM_STATE));
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 1:
                    f->handler = alRaw16Pull;
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.start = a->table->waveInfo.rawWave.loop->start;
                        a->loop.end = a->table->waveInfo.rawWave.loop->end;
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 2:
                    f->handler = alAdecPull;
                    a->loop.start = a->loop.end = a->loop.count = 0;
                    break;
                default:
                    break;
            }
            break;
        case 4:
            a->lastsam = 0;
            a->first = 1;
            a->sample = 0;
            if (a->table) {
                a->memin = (sptr)a->table->base;
                if (a->table->type == 0) {
                    if (a->table->waveInfo.adpcmWave.loop)
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                } else if (a->table->type == 1) {
                    if (a->table->waveInfo.rawWave.loop)
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                }
            }
            break;
        default:
            break;
    }
}

#ifdef TARGET_PORT
/* K0_TO_PHYS masked a host address to 29 bits: the port's RSP addresses come from
 * osVirtualToPhysical (t2ultra.c), which names any host address (docs/port/AUDIO.md). */
extern u32 osVirtualToPhysical(void *);
#define DECODE_K0_TO_PHYS(p) osVirtualToPhysical((void *)(p))
#else
#define DECODE_K0_TO_PHYS(p) ((uptr)(p) & 0x1FFFFFFF)
#endif

Acmd *_decodeChunk(Acmd *ptr, ALLoadFilter *f, sptr tsam, s32 nbytes, s16 outp, s16 inp, u32 flags)
{
    s32 dramAlign, dramLoc;
    if (nbytes > 0) {
        dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 0x7;
        nbytes += dramAlign;
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(inp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nbytes + 8 - (nbytes & 0x7)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(dramLoc - dramAlign); };
    } else
        dramAlign = 0;
    if (flags & 0x02) {
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(15) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(DECODE_K0_TO_PHYS(f->lstate)); };
    }
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(inp + dramAlign) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(tsam << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(flags) & (0xFFFFFFFFU >> (32 - (8)))) << (16)); _a->words.w1 = (unsigned int)(DECODE_K0_TO_PHYS(f->state)); };
    f->first = 0;
    return ptr;
}

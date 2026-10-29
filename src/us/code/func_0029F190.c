/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/alAdpcmPull.c, alAdpcmPull (libaudio load.c), with Turok 2's layouts. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *(*handler)(struct ALFilter_s *filter, s16 *outp, s32 outCount, s32 sampleOffset, void *cmdp);
    s32 (*setParam)(struct ALFilter_s *filter, s32 type, void *param);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    u32 w0;
    u32 w1;
} Acmd;
typedef struct {
    u32 start;
    u32 end;
    u32 count;
    s16 state[16];
} S13AdpcmLoop;
typedef struct {
    u32 start;
    u32 end;
    u32 count;
} S13RawLoop;
typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1];
} S13AdpcmBook;
typedef struct {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        struct {
            S13AdpcmLoop *loop;
            S13AdpcmBook *book;
        } adpcmWave;
        struct {
            S13RawLoop *loop;
        } rawWave;
    } waveInfo;
} S13WaveTable;
typedef s32 (*S13DMAproc)(s32 addr, s32 len, void *state);
typedef struct {
    ALFilter filter;
    s16 *state;
    s16 *lstate;
    S13RawLoop loop;
    S13WaveTable *table;
    s32 bookSize;
    S13DMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
} S13Loader;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_CLEARBUFF 2
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define aClearBuffer(pkt, d, c) \
    { \
        Acmd *_a = (Acmd *)(pkt); \
        _a->w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); \
        _a->w1 = (u32)(c); \
    }
#define aDMEMMove(pkt, i, o, c) \
    { \
        Acmd *_a = (Acmd *)(pkt); \
        _a->w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24); \
        _a->w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); \
    }
#define aLoadADPCM(pkt, c, d) \
    { \
        Acmd *_a = (Acmd *)(pkt); \
        _a->w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24); \
        _a->w1 = (u32)(d); \
    }

Acmd *func_0029FCF4(Acmd *ptr, S13Loader *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags); /* _decodeChunk */
u32 func_0026FFA0(void *addr, s32 len); /* game DMA cache: returns the physical address of a cached copy */

/* alAdpcmPull (libaudio load.c): pull handler of the loader filter for ADPCM wave tables: load the codebook, decode
   whole 16 sample frames (9 bytes each) through func_0029FCF4, restarting from the loop start (and merging the
   sections in DMEM) when the request crosses the loop end, and clear what runs past the end of the table. */
Acmd *func_0029F190(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    S13Loader *f = (S13Loader *)filter;
    s32 nbytes;
    s32 nframes;
    s32 nSam;
    s32 tsam;
    s32 overFlow;
    s32 dmemOut;
    s32 overBytes;
    s32 off;
    s32 dmemAlign;
    s32 looped;
    s32 decoded = 0;
    s32 tmp;
    s16 inp;

    if (outCount == 0)
        return ptr;

    /* the codebook of the wave table goes through the game's DMA cache */
    aLoadADPCM(ptr++, f->bookSize, func_0026FFA0((void *)((u32)f->table->waveInfo.adpcmWave.book + 8), f->bookSize));

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
        ptr = func_0029FCF4(ptr, f, tsam, nbytes, *outp, inp, f->first);
        if (f->lastsam)
            *outp += f->lastsam << 1;
        else
            *outp += 32;
        f->lastsam = f->loop.start & 0xF;
        f->memin = (s32)f->table->base + ((f->loop.start >> 4) + 1) * 9;
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
            ptr = func_0029FCF4(ptr, f, tsam, nframes * 9, dmemAlign, inp, f->first | 2);
            aDMEMMove(ptr++, dmemAlign + (f->lastsam << 1), dmemOut, nSam << 1);
        }
        f->lastsam = (outCount + f->lastsam) & 0xF;
        f->sample += outCount;
        f->memin += nframes * 9;
        return ptr;
    } else {
        nSam = nframes << 4;
        overBytes = f->memin + nbytes - ((s32)f->table->base + f->table->len);
        if (overBytes < 0)
            overBytes = 0;
        overFlow = (overBytes / 9) << 4;
        if (overFlow > nSam + dmemAlign)
            overFlow = nSam + dmemAlign;
        nbytes -= overBytes;
        if (overFlow - (overFlow & 0xF) < outCount) {
            decoded = 1;
            ptr = func_0029FCF4(ptr, f, nSam - overFlow, nbytes, *outp, inp, f->first);
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

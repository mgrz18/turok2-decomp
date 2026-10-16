/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_10.c, func_802BEAA4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;
typedef union {
    Awords words;
    long long int force_union_align;
} Acmd;
typedef Acmd *(*ALCmdHandler)(void *, short *, int, int, Acmd *);
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
extern void func_00291270(unsigned char *source, unsigned char *destination, int count);
struct ALFilter_s14;
typedef struct ALFilter_s14 ALFilter_s14;
struct ALFilter_s14;
struct ALFilter_s14 {
    struct ALFilter_s14 *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
};
struct ALRawLoop;
typedef struct ALRawLoop ALRawLoop;
struct ALRawLoop;
struct ALRawLoop {
    u32 start;
    u32 end;
    u32 count;
};
struct ALADPCMBook;
typedef struct ALADPCMBook ALADPCMBook;
struct ALADPCMBook;
struct ALADPCMBook {
    s32 order;
    s32 npredictors;
    s16 book[1];
};
extern char D_0029F190;
struct ALADPCMWaveInfo;
typedef struct ALADPCMWaveInfo ALADPCMWaveInfo;
struct ALADPCMloop;
typedef struct ALADPCMloop ALADPCMloop;
struct ALLoadFilter48;
typedef struct ALLoadFilter48 ALLoadFilter48;
struct ALRAWWaveInfo;
typedef struct ALRAWWaveInfo ALRAWWaveInfo;
struct ALWaveTable_s;
typedef struct ALWaveTable_s ALWaveTable_s;
typedef short ADPCM_STATE[16];
struct ALADPCMloop;
struct ALADPCMloop {
    u32 start;
    u32 end;
    u32 count;
    ADPCM_STATE state;
};
struct ALADPCMWaveInfo;
struct ALADPCMWaveInfo {
    ALADPCMloop *loop;
    ALADPCMBook *book;
};
struct ALRAWWaveInfo;
struct ALRAWWaveInfo {
    ALRawLoop *loop;
};
struct ALWaveTable_s;
struct ALWaveTable_s {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;
};
struct ALLoadFilter48;
struct ALLoadFilter48 {
    ALFilter_s14 filter;
    ADPCM_STATE *state;
    ADPCM_STATE *lstate;
    ALRawLoop loop;
    ALWaveTable_s *table;
    s32 bookSize;
    void *dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
};
extern char func_0029F5D4;
extern char func_0029F958;
s32 func_0029FB64(void *filter, s32 paramID, struct ALWaveTable_s *param)
{
    ALLoadFilter48 *a = (ALLoadFilter48 *)filter;
    ALFilter_s14 *f = (ALFilter_s14 *)filter;
    switch (paramID) {
        case 5:
            a->table = param;
            a->memin = (s32)a->table->base;
            a->sample = 0;
            switch (a->table->type) {
                case 0:
                    f->handler = &D_0029F190;
                    a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order *
                        a->table->waveInfo.adpcmWave.book->npredictors * 8;
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                        a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                        ((void (*)(void *, void *, s32))func_00291270)(a->table->waveInfo.adpcmWave.loop->state,
                                      a->lstate, sizeof(ADPCM_STATE));
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 1:
                    f->handler = &func_0029F5D4;
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.start = a->table->waveInfo.rawWave.loop->start;
                        a->loop.end = a->table->waveInfo.rawWave.loop->end;
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 2:
                    f->handler = &func_0029F958;
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
                a->memin = (s32)a->table->base;
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

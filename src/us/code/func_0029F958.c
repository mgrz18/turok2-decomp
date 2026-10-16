/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_9.c, func_802C3988.
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
typedef int (*ALDMAproc)(int addr, int len, void *state);
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
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
struct ALADPCMWaveInfo_func_802BE514_de;
typedef struct ALADPCMWaveInfo_func_802BE514_de ALADPCMWaveInfo_func_802BE514_de;
struct ALADPCMBook;
typedef struct ALADPCMBook ALADPCMBook;
struct ALADPCMBook;
struct ALADPCMBook {
    s32 order;
    s32 npredictors;
    s16 book[1];
};
struct ALADPCMWaveInfo_func_802BE514_de;
struct ALADPCMWaveInfo_func_802BE514_de {
    void *loop;
    ALADPCMBook *book;
};
struct ALWaveTableSCode802BE0D0;
struct ALWaveTableSCode802BE0D0 {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo_func_802BE514_de adpcmWave;
    } waveInfo;
};
struct ALLoadFilter_func_802BE514_de;
struct ALWaveTableSCode802BE0D0;
struct ALLoadFilter_func_802BE514_de {
    ALFilter_s14 filter;
    void *state;
    void *lstate;
    ALRawLoop loop;
    struct ALWaveTableSCode802BE0D0 *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
};
struct ALLoadFilter_func_802BE514_de;
typedef struct ALLoadFilter_func_802BE514_de ALLoadFilter_func_802BE514_de;
extern Acmd *func_0029F958(ALLoadFilter_func_802BE514_de *filter, s16 *outp, s32 byteCount, s32 sampleOffset, Acmd *p);
extern s32 D_800B8A20;
extern s32 func_002A122C();
extern void func_0029E340(s32 buffer, s32 size);
Acmd *func_0029F958(ALLoadFilter_func_802BE514_de *filter, s16 *outp, s32 byteCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    s32 nbytes;
    s32 dramAlign;
    s32 dmemAlign;
    s32 nSam;
    s32 op;
    ALLoadFilter_func_802BE514_de *f = filter;
    s32 *stream;
    op = *outp;
    if (f->table->base == 0) {
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(op) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(byteCount << 1); };
        return ptr;
    }
    if (byteCount != 0) {
        stream = &D_800B8A20;
        do {
            nSam = byteCount;
            if (f->sample == 0) {
                D_800B8A20 = func_002A122C();
                if (D_800B8A20 == 0) {
                    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(op) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(byteCount << 1); };
                    return ptr;
                }
                func_0029E340(D_800B8A20, 0x200);
                f->sample = 0x100;
            }
            if (f->sample < byteCount) {
                nSam = f->sample;
            }
            nbytes = nSam << 1;
            dramAlign = D_800B8A20 & 0x7;
            nbytes += dramAlign;
            if (op & 0x7) {
                dmemAlign = 8 - (op & 0x7);
            } else {
                dmemAlign = 0;
            }
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(op + dmemAlign) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(nbytes + 8 - (nbytes & 0x7)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)((D_800B8A20 & 0x1FFFFFFF) - dramAlign); };
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

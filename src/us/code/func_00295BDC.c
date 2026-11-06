/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_9.c, func_802B4C7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel);
extern s16 D_800B7900[128];
extern char D_800AA540[];
extern char D_800AA544[];
extern void __assert(const char *, const char *, s32);
extern unsigned int osVirtualToPhysical(void *);
Acmd *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    ALFilter *source = e->filter.source;
    if (e->motion != 1 || !outCount)
        return ptr;
    ((source) ? ((void)0) : __assert(D_800AA540, D_800AA544, 366));
    ptr = (*source->handler)(source, inp, outCount, sampleOffset, p);
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(*inp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(1088 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x08) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(1408 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(1728 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(2048 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * D_800B7900[e->pan]) >> 15;
        e->lratm = _getRate((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &(e->lratl));
        e->rtgt = (e->volume * D_800B7900[128 - e->pan - 1]) >> 15;
        e->rratm = _getRate((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &(e->rratl));
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x02 | 0x04) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->cvolL) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00 | 0x04) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->cvolR) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x02 | 0x00) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->ltgt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(e->lratm) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->lratl) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00 | 0x00) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->rtgt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(e->rratm) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->rratl) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x08) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->dryamt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->wetamt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x01 | 0x08) & (0xFFFFFFFFU >> (32 - (8)))) << (16)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(e->state)); };
    }
    else
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00 | 0x08) & (0xFFFFFFFFU >> (32 - (8)))) << (16)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(e->state)); };
    *inp += (outCount << 1);
    e->delta += outCount;
    return ptr;
}
s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel);

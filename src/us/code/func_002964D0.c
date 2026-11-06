/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B5570_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern unsigned int osVirtualToPhysical(void *);
Acmd *alResamplePull(ALResampler *filter, s16 *outp, s32 outCnt, s32 sampleOffset, Acmd *p)
{
    ALResampler *f = filter;
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    ALFilter *source = f->filter.source;
    s32 incr;
    f32 finCount;
    f32 ratio;
    f32 unity;
    inp = 320;
    if (!outCnt)
        return ptr;
    if (f->upitch) {
        ptr = (*source->handler)(source, &inp, outCnt, sampleOffset, p);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(inp) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (((unsigned int)(*outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCnt << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    } else {
        if (f->ratio > 1.99996f) f->ratio = 1.99996f;
        ratio = f->ratio;
        unity = 32768.0f;
        f->ratio = (f32)(s32)(ratio * unity) * 3.0517578e-5f;
        finCount = f->delta + (f->ratio * (f32)outCnt);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;
        ptr = (*source->handler)(source, &inp, inCount, sampleOffset, p);
        incr = (s32)(f->ratio * unity);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(inp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(*outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCnt << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(f->first) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(incr) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(f->state)); };
        f->first = 0;
    }
    return ptr;
}

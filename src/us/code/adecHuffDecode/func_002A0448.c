/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecDecodeFrame, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"
#include "globals/adec.h"
#include "structs/adec.h"

/* Acclaim compressed-audio stream decoder state (one instance) */

s32 adecReadBits(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st);
struct DecodeBlock;
s16 adecHuffDecode(struct DecodeBlock *segs, u16 *out, s16 n, u16 *cnt, AdecStream *st);

/* Decodes one frame into st: the 16 scale indices st->band[] (the very first frame raw 6 bit fields, afterwards Huffman coded
   differences against the previous frame) and the 256 coefficients st->coef[]. Returns 1, or -1 on a stream error. */
s32 adecDecodeFrame(AdecStream *st) {
    s16 cnt;
    s16 nv;
    s16 i;
    s32 rv;
    AdecHuffSeg *seg = gAdecHuffSegments;
    if (sAdecHeaderDone == 0) {
        sAdecHeaderDone = 1;
        if ((s16)adecReadBits((u16 *)st->band, 6, 16, 0, st) <= 0) {
            return -1;
        }
        seg->pos += 0x60;
        for (i = 0; i < 16; i++) {
            sAdecPrevScale[i] = st->band[i];
        }
        while (seg->kind != 0) {
            seg++;
        }
    } else {
        rv = adecHuffDecode((struct DecodeBlock *)seg, (u16 *)st->band, 16, (u16 *)&cnt, st);
        if (rv != 16) {
            goto fail;
        }
        seg = &seg[nv = cnt];
        for (i = 0; i < 16; i++) {
            nv = sAdecPrevScale[i];
            st->band[i] += nv;
            sAdecPrevScale[i] = st->band[i];
        }
    }
    rv = adecHuffDecode((struct DecodeBlock *)seg, (u16 *)st->coef, 256, (u16 *)&cnt, st);
    if (rv != 256) {
    fail:
        return -1;
    }
    return 1;
}

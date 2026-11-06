/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecReadBits, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"
#include "globals/adec.h"

/* Acclaim compressed-audio stream decoder state (one instance) */
typedef struct {
    /* 0x00 */ u32 hdr0;
    /* 0x04 */ u32 hdr2;
    /* 0x08 */ u32 *hdr;
    /* 0x0C */ u32 nframes;
    /* 0x10 */ u32 remaining;
    /* 0x14 */ u16 *rd;       /* bitstream read pointer */
} AdecStream;

/* Bit reader: fetches `count` fields of `nbits` bits (MSB first) from the stream of 16-bit words at st->rd into dst[],
   sign-extending each field to 16 bits when `sign` is set. Always returns 1. */
s32 adecReadBits(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st)
{
    s16 i;
    u16 extra;     /* bits needed from the next word */
    u16 maskLeft;
    u16 curWord;
    int fldA;
    int fldB;
    if (sAdecBitsLeft == 0) {
        *(volatile u16 *)&sAdecBitWord = *(st->rd++);
        sAdecBitsLeft = 16;
    }
    for (i = 0; i < count; i++) {
        if (sAdecBitsLeft < nbits) {
            /* the field straddles two words: the low sAdecBitsLeft bits of this one, then the top `extra` bits of the next */
            extra = nbits - sAdecBitsLeft;
            curWord = sAdecBitWord;
            maskLeft = gAdecBitMasks[sAdecBitsLeft];
            dst[i] = (curWord & maskLeft) << extra;
            sAdecBitWord = *(st->rd++);
            sAdecBitWord += 0;
            fldA = (sAdecBitWord >> (16 - extra)) & gAdecBitMasks[extra];
            dst[i] |= fldA;
            if (sign) {
                if (((s16)dst[i]) & gAdecSignBits[nbits]) {
                    dst[i] |= ~gAdecBitMasks[nbits];
                }
            }
            sAdecBitsLeft = 16 - extra;
        } else {
            fldB = (sAdecBitWord >> (sAdecBitsLeft - nbits)) & gAdecBitMasks[nbits];
            dst[i] = fldB;
            if (sign) {
                if (((s16)dst[i]) & gAdecSignBits[nbits]) {
                    dst[i] |= ~gAdecBitMasks[nbits];
                }
            }
            sAdecBitsLeft -= nbits;
        }
    }
    return 1;
}

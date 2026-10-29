/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecReadBits, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"

/* Acclaim compressed-audio stream decoder state (one instance) */
typedef struct {
    /* 0x00 */ u32 hdr0;
    /* 0x04 */ u32 hdr2;
    /* 0x08 */ u32 *hdr;
    /* 0x0C */ u32 nframes;
    /* 0x10 */ u32 remaining;
    /* 0x14 */ u16 *rd;       /* bitstream read pointer */
} AdecStream;

extern u16 D_800B8A40[];   /* mask (1 << n) - 1 */
extern u16 D_800B8A64[];   /* sign bit 1 << (n - 1) */
extern u16 D_800B8BA8;     /* bits still unread in the current word */
extern u16 D_80125000;     /* current stream word */

/* Bit reader: fetches `count` fields of `nbits` bits (MSB first) from the stream of 16-bit words at st->rd into dst[],
   sign-extending each field to 16 bits when `sign` is set. Always returns 1. */
s32 func_0029FE70(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st)
{
    s16 i;
    u16 extra;     /* bits needed from the next word */
    u16 maskLeft;
    u16 curWord;
    int fldA;
    int fldB;
    if (D_800B8BA8 == 0) {
        *(volatile u16 *)&D_80125000 = *(st->rd++);
        D_800B8BA8 = 16;
    }
    for (i = 0; i < count; i++) {
        if (D_800B8BA8 < nbits) {
            /* the field straddles two words: the low D_800B8BA8 bits of this one, then the top `extra` bits of the next */
            extra = nbits - D_800B8BA8;
            curWord = D_80125000;
            maskLeft = D_800B8A40[D_800B8BA8];
            dst[i] = (curWord & maskLeft) << extra;
            D_80125000 = *(st->rd++);
            D_80125000 += 0;
            fldA = (D_80125000 >> (16 - extra)) & D_800B8A40[extra];
            dst[i] |= fldA;
            if (sign) {
                if (((s16)dst[i]) & D_800B8A64[nbits]) {
                    dst[i] |= ~D_800B8A40[nbits];
                }
            }
            D_800B8BA8 = 16 - extra;
        } else {
            fldB = (D_80125000 >> (D_800B8BA8 - nbits)) & D_800B8A40[nbits];
            dst[i] = fldB;
            if (sign) {
                if (((s16)dst[i]) & D_800B8A64[nbits]) {
                    dst[i] |= ~D_800B8A40[nbits];
                }
            }
            D_800B8BA8 -= nbits;
        }
    }
    return 1;
}

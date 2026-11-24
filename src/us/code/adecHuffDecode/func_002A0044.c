/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* Re-matched under -fforce-addr (round 26), the flags of the other adecHuffDecode files: the
 * phrase address is an integer sum (dictOffset first), no longer the (phrases + 1) - 1 form. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_12.c, func_802BEF84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/adec.h"

s32 adecReadBits(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st);
#ifdef TARGET_PORT
#define readBits adecReadBits
#else
/* The ROM caller takes the result as a u16 (andi 0xFFFF): called through a type of its own. */
#define readBits ((u16 (*)(u16 *, u16, u16, s16, AdecStream *))adecReadBits)
#endif
struct DecodeBlock;
s16 adecHuffDecode(struct DecodeBlock *segs, u16 *out, s16 n, u16 *cnt, AdecStream *st);
s16 adecHuffDecode(DecodeBlock *arg0, u16 *arg1, s16 arg2,
                    u16 *arg3, AdecStream *arg4) {
    u16 node;
    s16 block, output;
    DecodeTable *table;
    DecodeNode *tree;
    u16 value;
    s32 mask, high;
    u16 topbit;
    s16 j;
    s32 dictOffset;
    u16 next;
    u16 *bits, *available;
    DecodePhrase *phrase;
    u16 *out;
    node = 0;
    block = 0;
    *arg3 = 0;
    table = arg0->table;
    tree = table->tree;
    output = 0;
    if (arg2 > 0) {
        available = &sAdecHuffBitsAvail;
        bits = &sAdecHuffBits;
        do {
            if (*available != 0) {
                (*available)--;
                topbit = *bits & 0x8000;
                *bits <<= 1;
                if (topbit) {
                    mask = tree[(s16)node].second;
                } else {
                    mask = tree[(s16)node].first;
                }
                if (tree[(s16)mask].first == 0x800) {
                    value = tree[(s16)mask].second;
                    mask = value & 0xF00;
                    node = 0;
                    if (mask == 0 || mask == 0xF00) {
                        arg1[(s16)output] = value;
                        output++;
                    } else if ((s16)value == 0x900) {
                        value = *bits;
                        if (readBits(bits, table->width, 1, 0, arg4) == 0) {
                            return -1;
                        }
                        mask = (s16)output;
                        output++;
                        high = *bits;
                        next = sAdecHuffBitsAvail;
                        *bits = high << (16 - next);
                        arg1[(s16)mask] = value | (high >> next);
                    } else if (mask == 0xA00) {
                        j = 0;
                        dictOffset = (value & 0xFF) * sizeof(DecodePhrase);
                        out = (arg1 + ((s16)output + 1)) - 1;
                        phrase = (DecodePhrase *)(dictOffset + (sptr)table->phrases);
                        while ((s16)j < phrase->count) {
                            out[(s16)j] = phrase->samples[(s16)j].value;
                            j++;
                        }
                        output += (s32)((u32)j << 16) >> 16;
                    } else if (mask == 0xB00) {
                        while ((s16)output < arg2) {
                            arg1[(s16)output++] = 0;
                        }
                        while (arg2 >= arg0[(s16)block].start +
                                       arg0[(s16)block].length) {
                            block++;
                            (*arg3)++;
                        }
                        return arg2;
                    }
                    if ((s16)output >= arg0[(s16)block].start +
                                       arg0[(s16)block].length) {
                        block++;
                        (*arg3)++;
                        table = arg0[(s16)block].table;
                        tree = table->tree;
                    }
                } else {
                    node = mask;
                }
            } else {
                if (readBits(&sAdecHuffBits, 16, 1, 0, arg4) == 0) {
                    return -1;
                }
                *available += 16;
            }
        } while ((s16)output < arg2);
    }
    return (s16)output;
}

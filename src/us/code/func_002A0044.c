/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_12.c, func_802BEF84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern unsigned short D_800B8BAA;
extern unsigned short D_800B8BAC;
typedef struct DecodeNode {
    u16 first;
    u16 second;
} DecodeNode;
typedef struct DecodeSample {
    u16 tag;
    u16 value;
} DecodeSample;
typedef struct DecodePhrase {
    u8 pad0[0xA0];
    DecodeSample samples[64];
    s32 count;
    u8 pad1A4[0x14];
} DecodePhrase;
typedef struct DecodeTable {
    u8 pad0[0xE];
    u16 width;
    u8 pad10[0xC];
    DecodePhrase *phrases;
    DecodeNode *tree;
} DecodeTable;
typedef struct DecodeBlock {
    s32 kind;
    s32 start;
    s32 length;
    s32 offset;
    DecodeTable *table;
} DecodeBlock;
extern u16 func_0029FE70(u16 *, u16, s32, s32, void *);
s32 func_002A0044(DecodeBlock *arg0, u16 *arg1, s16 arg2,
                    u16 *arg3, void *arg4) {
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
        available = &D_800B8BAC;
        bits = &D_800B8BAA;
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
                        if (func_0029FE70(bits, table->width, 1, 0, arg4) == 0) {
                            return -1;
                        }
                        mask = (s16)output;
                        output++;
                        high = *bits;
                        next = D_800B8BAC;
                        *bits = high << (16 - next);
                        arg1[(s16)mask] = value | (high >> next);
                    } else if (mask == 0xA00) {
                        j = 0;
                        dictOffset = (value & 0xFF) * sizeof(DecodePhrase);
                        out = (arg1 + ((s16)output + 1)) - 1;
                        phrase = (DecodePhrase *)(dictOffset +
                                   (u8 *)(table->phrases + 1)) - 1;
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
                if (func_0029FE70(&D_800B8BAA, 16, 1, 0, arg4) == 0) {
                    return -1;
                }
                *available += 16;
            }
        } while ((s16)output < arg2);
    }
    return (s16)output;
}

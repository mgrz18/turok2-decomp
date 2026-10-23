typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;

typedef struct {
    char pad0[0x4];
    u8 *data;                   /* 0x04 */
    u32 bitPos;                 /* 0x08 */
    s32 value;                  /* 0x0C */
} BitReader;

/* Reads count bits, most significant first. */
static __inline__ s32 readBits(BitReader *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->bitPos++;
        v |= (r->data[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

void func_0042EC30(BitReader *r) {
    s32 n = readBits(r, 3);

    if (n != 0) {
        r->value = readBits(r, n) + 1;
    } else {
        r->value = 1;
    }
}

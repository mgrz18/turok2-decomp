#include "common.h"

typedef struct {
    /* 0x0 */ char pad[8];
    /* 0x8 */ u8 *p;
} Reader;

/* Reads a MIDI-style variable-length quantity: 7 bits per byte, high bit set
 * on every byte but the last. */
u32 func_002932BC(Reader *r) {
    u32 v;
    u32 c;
    c = *r->p++;
    v = c;
    if (v & 0x80) {
        v &= 0x7F;
        do {
            v <<= 7;
            c = *r->p++;
            v += c & 0x7F;
        } while (c & 0x80);
    }
    return v;
}

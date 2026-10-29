/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/xlitob.c, _Litob, with Turok 2's layouts. */

#include "common.h"

#define BUFF_LEN 0x18
#define FLAGS_MINUS 4
#define FLAGS_ZERO 16

typedef struct {
    /* 0x00 */ union {
        s64 ll;
        f64 ld;
    } v;
    /* 0x08 */ char *s;
    /* 0x0C */ s32 n0;
    /* 0x10 */ s32 nz0;
    /* 0x14 */ s32 n1;
    /* 0x18 */ s32 nz1;
    /* 0x1C */ s32 n2;
    /* 0x20 */ s32 nz2;
    /* 0x24 */ s32 prec;
    /* 0x28 */ s32 width;
    /* 0x2C */ u32 nchar;
    /* 0x30 */ u32 flags;
    /* 0x34 */ u8 qual;
} _Pft;

typedef struct {
    s64 quot;
    s64 rem;
} lldiv_t;

extern char D_800B89F0[]; /* ldigs "0123456789abcdef" */
extern char D_800B8A04[]; /* udigs "0123456789ABCDEF" */
extern void *func_0029E460(void *dst, const void *src, u32 n); /* memcpy */

static __inline__ void lldiv(lldiv_t *ans, s64 num, s64 den) {
    ans->quot = num / den;
    ans->rem = num % den;
}

void func_0029EEA0(_Pft *x, u8 type) {
    char buff[BUFF_LEN];
    const char *digs;
    s32 base;
    s32 i;
    u64 ullval;

    digs = D_800B89F0;
    if (type == 'X') {
        digs = D_800B8A04;
    }

    base = (type == 'o') ? 8 : (type != 'x' && type != 'X') ? 10 : 16;

    i = BUFF_LEN;
    ullval = x->v.ll;

    if ((type == 'd' || type == 'i') && x->v.ll < 0) {
        ullval = -ullval;
    }

    if (ullval != 0 || x->prec != 0) {
        buff[--i] = digs[ullval % base];
    }

    x->v.ll = ullval / base;

    while (x->v.ll > 0 && i > 0) {
        lldiv_t qr;

        lldiv(&qr, x->v.ll, base);

        x->v.ll = qr.quot;
        buff[--i] = digs[qr.rem];
    }

    x->n1 = BUFF_LEN - i;
    func_0029E460(x->s, buff + i, x->n1);

    if (x->n1 < x->prec) {
        x->nz0 = x->prec - x->n1;
    }

    if (x->prec < 0 && (x->flags & (FLAGS_MINUS | FLAGS_ZERO)) == FLAGS_ZERO) {
        i = x->width - x->n0 - x->nz0 - x->n1;

        if (i > 0) {
            x->nz0 += i;
        }
    }
}

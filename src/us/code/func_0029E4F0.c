/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/xprintf.c, _Printf (libultra xprintf.c), with Turok 2's symbols. */
/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */

#include "rw/ultralib_types.h"

#define NULL 0
#define TRUE 1
#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16

#define MAX_PAD 32

#define isdigit(c) ((c) >= '0' && (c) <= '9')

extern char D_800B89A0[]; /* spaces: 32 blanks */
extern char D_800B89C4[]; /* zeroes: 32 zeros */

char *func_0029E488(const char *s, int c); /* strchr */
void _Putfld(_Pft *px, va_list *pap, char code, char *ac);

static const u8 fchar[] = " +-#0";
static const u32 fbit[] = { FLAGS_SPACE, FLAGS_PLUS, FLAGS_MINUS, FLAGS_HASH, FLAGS_ZERO, 0 };

#define PUT(s, n)                          \
    if ((n) > 0) {                         \
        arg = (*prout)(arg, (s), (n));     \
        if (arg != NULL) {                 \
            x.nchar += (n);                \
        } else {                           \
            return x.nchar;                \
        }                                  \
    }

#define PAD(s, n)                                      \
    if ((n) > 0) {                                     \
        s32 i;                                         \
        s32 j = (n);                                   \
        for (; j > 0; j -= i) {                        \
            i = (u32)j > MAX_PAD ? MAX_PAD : j;        \
            PUT(s, i)                                  \
        }                                              \
    }


s32 func_0029E4F0(char *(*prout)(char *, const char *, u32), char *arg, const char *fmt, va_list args) {
    _Pft x;
    u8 ac[32];

    x.nchar = 0;

    while (TRUE) {
        const u8 *s;
        s32 c;
        s32 n;
        const u8 *t;

        s = (const u8 *)fmt;
        while ((c = *s) != 0 && c != '%') {
            s++;
        }
        n = s - (const u8 *)fmt;
        PUT(fmt, n)
        if (c == 0) {
            return x.nchar;
        }
        fmt = (const char *)++s;

        for (x.flags = 0; (t = func_0029E488(fchar, *s)) != NULL; s++) {
            x.flags |= fbit[t - fchar];
        }

        if (*s == '*') {
            { s32 w; s32 *ip = (s32 *)(((u32)args + 3) & ~3); w = *ip; args = (va_list)(ip + 1); x.width = w; }
            if (x.width < 0) {
                x.width = -x.width;
                x.flags |= FLAGS_MINUS;
            }
            s++;
        } else {
            for (x.width = 0; isdigit(*s); s++) {
                if (x.width < 999) {
                    x.width = x.width * 10 + *s - '0';
                }
            }
        }

        if (*s != '.') {
            x.prec = -1;
        } else if (*++s == '*') {
            { s32 w; s32 *ip = (s32 *)(((u32)args + 3) & ~3); w = *ip; ++s; args = (va_list)(ip + 1); x.prec = w; }
        } else {
            for (x.prec = 0; isdigit(*s); s++) {
                if (x.prec < 999) {
                    x.prec = x.prec * 10 + *s - '0';
                }
            }
        }

        x.qual = func_0029E488("hlL", *s) ? *s++ : '\0';
        if (x.qual == 'l' && *s == 'l') {
            x.qual = 'L';
            ++s;
        }

        _Putfld(&x, &args, *s, ac);

        x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;

        if (!(x.flags & FLAGS_MINUS)) {
            PAD(D_800B89A0, x.width)
        }
        PUT(ac, x.n0)
        PAD(D_800B89C4, x.nz0)
        PUT(x.s, x.n1)
        PAD(D_800B89C4, x.nz1)
        PUT(x.s + x.n1, x.n2)
        PAD(D_800B89C4, x.nz2)
        if (x.flags & FLAGS_MINUS) {
            PAD(D_800B89A0, x.width)
        }

        fmt = (const char *)s + 1;
    }
}

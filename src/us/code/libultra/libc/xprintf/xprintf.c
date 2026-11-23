/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/xprintf.c, _Printf (libultra xprintf.c), with Turok 2's symbols. */

#include "rw/ultralib_types.h"
#include "globals/libc.h"
#define NULL 0
#define TRUE 1
#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16
#define MAX_PAD 32
#define isdigit(c) ((c) >= '0' && (c) <= '9')

char *strchr(const char *s, int c); /* strchr */
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

s32 _Printf(char *(*prout)(char *, const char *, u32), char *arg, const char *fmt, va_list args) {
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

        for (x.flags = 0; (t = strchr(fchar, *s)) != NULL; s++) {
            x.flags |= fbit[t - fchar];
        }

        if (*s == '*') {
            { s32 w; s32 *ip = (s32 *)(((uptr)args + 3) & ~3); w = *ip; args = (va_list)(ip + 1); x.width = w; }
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
            { s32 w; s32 *ip = (s32 *)(((uptr)args + 3) & ~3); w = *ip; ++s; args = (va_list)(ip + 1); x.prec = w; }
        } else {
            for (x.prec = 0; isdigit(*s); s++) {
                if (x.prec < 999) {
                    x.prec = x.prec * 10 + *s - '0';
                }
            }
        }

        x.qual = strchr("hlL", *s) ? *s++ : '\0';
        if (x.qual == 'l' && *s == 'l') {
            x.qual = 'L';
            ++s;
        }

        _Putfld(&x, &args, *s, ac);

        x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;

        if (!(x.flags & FLAGS_MINUS)) {
            PAD(spaces, x.width)
        }
        PUT(ac, x.n0)
        PAD(zeroes, x.nz0)
        PUT(x.s, x.n1)
        PAD(zeroes, x.nz1)
        PUT(x.s + x.n1, x.n2)
        PAD(zeroes, x.nz2)
        if (x.flags & FLAGS_MINUS) {
            PAD(spaces, x.width)
        }

        fmt = (const char *)s + 1;
    }
}

size_t strlen(const char *);
void _Litob(_Pft *px, char code);
void _Ldtob(_Pft* px, char code);

void _Putfld(_Pft *px, va_list *pap, char code, char *ac) {
    px->n0 = px->nz0 = px->n1 = px->nz1 = px->n2 =
        px->nz2 = 0;
    switch (code) {
        case 'c':
            ac[px->n0++] = ((int *) (void *) (*pap = (char *) ((__alignof__(int) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (int) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            break;
        case 'd':
        case 'i':
            if (px->qual == 'l') {
                px->v.ll = ((long *) (void *) (*pap = (char *) ((__alignof__(long) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else if (px->qual == 'L') {
                px->v.ll = ((long long *) (void *) (*pap = (char *) ((__alignof__(long long) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (long long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else {
                px->v.ll = ((int *) (void *) (*pap = (char *) ((__alignof__(int) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (int) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            }
            if (px->qual == 'h') {
                px->v.ll = (short)px->v.ll;
            }
            if (px->v.ll < 0) {
                ac[px->n0++] = '-';
            } else if (px->flags & 2) {
                ac[px->n0++] = '+';
            } else if (px->flags & 1) {
                ac[px->n0++] = ' ';
            }
            px->s = (char *)&ac[px->n0];
            _Litob(px, code);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            if (px->qual == 'l') {
                px->v.ll = ((long *) (void *) (*pap = (char *) ((__alignof__(long) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else if (px->qual == 'L') {
                px->v.ll = ((long long *) (void *) (*pap = (char *) ((__alignof__(long long) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (long long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else {
                px->v.ll = ((int *) (void *) (*pap = (char *) ((__alignof__(int) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (int) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            }
            if (px->qual == 'h') {
                px->v.ll = (unsigned short)px->v.ll;
            } else if (px->qual == 0) {
                px->v.ll = (unsigned int)px->v.ll;
            }
            if (px->flags & 8) {
                ac[px->n0++] = '0';
                if (code == 'x' || code == 'X') {
                    ac[px->n0++] = code;
                }
            }
            px->s = (char *)&ac[px->n0];
            _Litob(px, code);
            break;
        case 'e':
        case 'f':
        case 'g':
        case 'E':
        case 'G':
            px->v.ld = px->qual == 'L' ? ((ldouble *) (void *) (*pap = (char *) ((__alignof__(ldouble) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (ldouble) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] : ((double *) (void *) (*pap = (char *) ((__alignof__(double) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (double) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            if ((((unsigned short *)&(px->v.ld))[0] & 0x8000))
                ac[px->n0++] = '-';
            else if (px->flags & 2)
                ac[px->n0++] = '+';
            else if (px->flags & 1)
                ac[px->n0++] = ' ';
            px->s = (char *)&ac[px->n0];
            _Ldtob(px, code);
            break;
        case 'n':
            if (px->qual == 'h') {
                *((unsigned short * *) (void *) (*pap = (char *) ((__alignof__(unsigned short *) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (unsigned short *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            } else if (px->qual == 'l') {
                *((unsigned long * *) (void *) (*pap = (char *) ((__alignof__(unsigned long *) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (unsigned long *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            } else if (px->qual == 'L') {
                *((unsigned long long * *) (void *) (*pap = (char *) ((__alignof__(unsigned long long *) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (unsigned long long *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            } else {
                *((unsigned int * *) (void *) (*pap = (char *) ((__alignof__(unsigned int *) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (unsigned int *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            }
            break;
        case 'p':
            px->v.ll = (long)((void * *) (void *) (*pap = (char *) ((__alignof__(void *) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (void *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            px->s = (char *)&ac[px->n0];
            _Litob(px, 'x');
            break;
        case 's':
            px->s = ((char * *) (void *) (*pap = (char *) ((__alignof__(char *) > 4 ? ((sptr)*pap + 8 - 1) & -8 : ((sptr)*pap + 4 - 1) & -4) + (((sizeof (char *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            px->n1 = strlen(px->s);
            if (px->prec >= 0 && px->prec < px->n1) {
                px->n1 = px->prec;
            }
            break;
        case '%':
            ac[px->n0++] = '%';
            break;
        default:
            ac[px->n0++] = code;
            break;
    }
}

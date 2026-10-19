/* From decompals/ultralib (libultra 2.0I): src/libc/xprintf.c, _Putfld.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */

#include "rw/ultralib_types.h"

size_t func_0029E4C0(const char *);
void func_0029EEA0(_Pft *px, char code);
void func_0029F180(_Pft* px, char code);
void _Putfld(_Pft *px, va_list *pap, char code, char *ac);
void _Putfld(_Pft *px, va_list *pap, char code, char *ac) {
    px->n0 = px->nz0 = px->n1 = px->nz1 = px->n2 =
        px->nz2 = 0;
    switch (code) {
        case 'c':
            ac[px->n0++] = ((int *) (void *) (*pap = (char *) ((__alignof__(int) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (int) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            break;
        case 'd':
        case 'i':
            if (px->qual == 'l') {
                px->v.ll = ((long *) (void *) (*pap = (char *) ((__alignof__(long) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else if (px->qual == 'L') {
                px->v.ll = ((long long *) (void *) (*pap = (char *) ((__alignof__(long long) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (long long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else {
                px->v.ll = ((int *) (void *) (*pap = (char *) ((__alignof__(int) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (int) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
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
            func_0029EEA0(px, code);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            if (px->qual == 'l') {
                px->v.ll = ((long *) (void *) (*pap = (char *) ((__alignof__(long) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else if (px->qual == 'L') {
                px->v.ll = ((long long *) (void *) (*pap = (char *) ((__alignof__(long long) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (long long) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            } else {
                px->v.ll = ((int *) (void *) (*pap = (char *) ((__alignof__(int) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (int) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
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
            func_0029EEA0(px, code);
            break;
        case 'e':
        case 'f':
        case 'g':
        case 'E':
        case 'G':
            px->v.ld = px->qual == 'L' ? ((ldouble *) (void *) (*pap = (char *) ((__alignof__(ldouble) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (ldouble) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] : ((double *) (void *) (*pap = (char *) ((__alignof__(double) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (double) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            if ((((unsigned short *)&(px->v.ld))[0] & 0x8000))
                ac[px->n0++] = '-';
            else if (px->flags & 2)
                ac[px->n0++] = '+';
            else if (px->flags & 1)
                ac[px->n0++] = ' ';
            px->s = (char *)&ac[px->n0];
            func_0029F180(px, code);
            break;
        case 'n':
            if (px->qual == 'h') {
                *((unsigned short * *) (void *) (*pap = (char *) ((__alignof__(unsigned short *) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (unsigned short *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            } else if (px->qual == 'l') {
                *((unsigned long * *) (void *) (*pap = (char *) ((__alignof__(unsigned long *) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (unsigned long *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            } else if (px->qual == 'L') {
                *((unsigned long long * *) (void *) (*pap = (char *) ((__alignof__(unsigned long long *) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (unsigned long long *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            } else {
                *((unsigned int * *) (void *) (*pap = (char *) ((__alignof__(unsigned int *) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (unsigned int *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1] = px->nchar;
            }
            break;
        case 'p':
            px->v.ll = (long)((void * *) (void *) (*pap = (char *) ((__alignof__(void *) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (void *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            px->s = (char *)&ac[px->n0];
            func_0029EEA0(px, 'x');
            break;
        case 's':
            px->s = ((char * *) (void *) (*pap = (char *) ((__alignof__(char *) > 4 ? ((int)*pap + 8 - 1) & -8 : ((int)*pap + 4 - 1) & -4) + (((sizeof (char *) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)))))[-1];
            px->n1 = func_0029E4C0(px->s);
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

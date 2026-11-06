#ifndef GLOBALS_LIBC_H
#define GLOBALS_LIBC_H

/* The SDK libc's printf tables (xprintf.c, xlitob.c), 0x800B89A0-0x800B8A13. */

extern char spaces[33]; /* 32 blanks (_Printf) */
extern char zeroes[33]; /* 32 '0's (_Printf) */
extern char ldigs[17];  /* "0123456789abcdef" (_Litob) */
extern char udigs[17];  /* "0123456789ABCDEF" (_Litob) */

#endif

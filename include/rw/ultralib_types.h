/* Types of the functions taken from decompals/ultralib (tools/import_ref.py),
 * gathered in one place by tools/centralize_types.py. Their names are that
 * decomp's own. */

#ifndef RW_ULTRALIB_TYPES_H
#define RW_ULTRALIB_TYPES_H

#include "common.h"
#include "ultra_types.h" /* OSThread, OSMesgQueue, OSPfs, __OSInode, __OSInodeCache */

#ifdef TARGET_PORT
/* The host's own size_t (64-bit; Windows predeclares it and rejects another
 * type). These printf internals are replaced by the host libc in the port. */
typedef __SIZE_TYPE__ size_t;
#else
typedef unsigned size_t;
#endif
typedef char * __gnuc_va_list;
typedef __gnuc_va_list va_list;
typedef long double ldouble;
typedef struct {
              union {
                  long long ll;
                  ldouble ld;
    } v;
              unsigned char* s;
              int n0;
               int nz0;
               int n1;
               int nz1;
               int n2;
               int nz2;
               int prec;
               int width;
               size_t nchar;
               unsigned int flags;
               char qual;
} _Pft;

#endif

/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mhard-float -mgp32 -mno-fix4300 -mfp32 -msplit-addresses */
/* GCC 2.8.1 libgcc2.c, L_udivdi3 unit (GPLv2+ with the libgcc exception), with the MIPS parts of longlong.h:
 * the same text AbuN64/ragewars-decomp (Turok: Rage Wars) keeps in src/libgcc/libgcc2.inc.c.
 * The shared text is include/libgcc2.h. */

#define L_udivdi3
#include "libgcc2.h"

UDItype
__udivdi3 (UDItype n, UDItype d)
{
  return __udivmoddi4 (n, d, (UDItype *) 0);
}

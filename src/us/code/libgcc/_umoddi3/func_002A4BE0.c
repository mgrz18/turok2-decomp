/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mhard-float -mgp32 -mno-fix4300 -mfp32 -msplit-addresses */
/* GCC 2.8.1 libgcc2.c, L_umoddi3 unit (GPLv2+ with the libgcc exception), with the MIPS parts of longlong.h:
 * the same text AbuN64/ragewars-decomp (Turok: Rage Wars) keeps in src/libgcc/libgcc2.inc.c.
 * The shared text is include/libgcc2.h. */

#define L_umoddi3
#include "libgcc2.h"

UDItype
__umoddi3 (UDItype u, UDItype v)
{
  UDItype w;

  (void) __udivmoddi4 (u, v, &w);

  return w;
}

/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mhard-float -mgp32 -mno-fix4300 -mfp32 -msplit-addresses */
/* GCC 2.8.1 libgcc2.c, L_divdi3 unit (GPLv2+ with the libgcc exception), with the MIPS parts of longlong.h:
 * the same text AbuN64/ragewars-decomp (Turok: Rage Wars) keeps in src/libgcc/libgcc2.inc.c.
 * The shared text is include/libgcc2.h. */

#define L_divdi3
#include "libgcc2.h"

DItype
__divdi3 (DItype u, DItype v)
{
  word_type c = 0;
  DIunion uu, vv;
  DItype w;

  uu.ll = u;
  vv.ll = v;

  if (uu.s.high < 0)
    c = ~c,
    uu.ll = __negdi2 (uu.ll);
  if (vv.s.high < 0)
    c = ~c,
    vv.ll = __negdi2 (vv.ll);

  w = __udivmoddi4 (uu.ll, vv.ll, (UDItype *) 0);
  if (c)
    w = __negdi2 (w);

  return w;
}

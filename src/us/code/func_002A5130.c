/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mhard-float -mgp32 -mno-fix4300 -mfp32 */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/LIBGCC/802C6A00_25.CPP, __floatdisf (GCC 2.8.0 libgcc2.c), with Turok 2 (no layout changes). */
typedef unsigned int USItype;
typedef long long DItype;
typedef unsigned long long UDItype;
typedef int SItype;
typedef double DFtype;
typedef float SFtype;
SFtype
__floatdisf (DItype u)
{
  /* Do the calculation in DFmode
     so that we don't lose any of the precision of the high word
     while multiplying it.  */
  DFtype f;
  SItype negate = 0;

  if (u < 0)
    u = -u, negate = 1;

  /* Protect against double-rounding error.
     Represent any low-order bits, that might be truncated in DFmode,
     by a bit that won't be lost.  The bit can go in anywhere below the
     rounding position of the SFmode.  A fixed mask and bit position
     handles all usual configurations.  It doesn't handle the case
     of 128-bit DImode, however.  */
  if (53 < 64
      && 53 > (64 - 53 + 24))
    {

      if (u >= ((UDItype) 1 << 53))
	{
	  if ((USItype) u & (2048U - 1))
	    u |= 2048U;
	}
    }
  f = (USItype) (u >> 32);
  f *= 65536.0;
  f *= 65536.0;
  f += (USItype) (u & (4294967296ULL - 1));

  return (SFtype) (negate ? -f : f);
}

#include "common.h"
#include "m2c_macros.h"
#include "globals/libultra.h"
/* WRONG CUT: real function is 0x266B90 size 0xCC (all 51 words byte-identical to the ROM). splat cut it at 0x266C00, so its word-copy tail became func_00266C00 (now separately in src/us/code/func_00266C00.c, a false function: the beqz at +0x10 is this function branching to its own else-block). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265444_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265650_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_802656DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265714_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"
#include "globals/boss.h"

s32 GetMemSize(void) {
    return osMemSize;
}

/* memmove: bytes unless everything is 4-aligned, then words. */
void memmove(u8 *dst, u8 *src, s32 len) {
    u8 *d;
    u8 *s;
    s32 *dw;
    s32 *sw;

    if (len == 0) {
        return;
    }
    if (((uptr)src | (uptr)dst | len) & 3) {
        d = dst;
        s = src;
        if (dst < src) {
            do {
                *d++ = *s++;
            } while (--len != 0);
        } else {
            d += len - 1;
            s += len - 1;
            do {
                *d-- = *s--;
            } while (--len != 0);
        }
    } else {
        dw = (s32 *)dst;
        sw = (s32 *)src;
        len >>= 2;
        if (dw < sw) {
            do {
                *dw++ = *sw++;
            } while (--len != 0);
        } else {
            dw += len - 1;
            sw += len - 1;
            do {
                *dw-- = *sw--;
            } while (--len != 0);
        }
    }
}

void memset(s8 *arg0, s8 arg1, sptr arg2) {
    sptr var_v0;
    s8 *var_a0;

    var_a0 = arg0;
    var_v0 = arg2 - 1;
    if (arg2 != 0) {
        do {
            *var_a0 = arg1;
            var_v0 -= 1;
            var_a0 += 1;
        } while (var_v0 != -1);
    }
}

s32 memcmp(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *end = arg0 + arg2;
    if (arg2 >= 4 && !((uptr)arg0 & 3) && !((uptr)arg1 & 3)) {
        end -= 4;
        if (end < arg0) {
            end += 4;
            goto byte_loop;
        }
word_loop:
        if (*(u32 *)arg0 != *(u32 *)arg1) {
            goto word_mismatch;
        }
        arg0 += 4;
        goto word_continue;
word_mismatch:
        arg0 -= 4;
        arg1 -= 4;
        goto word_done;
word_continue:
        arg1 += 4;
        if (arg0 <= end) {
            goto word_loop;
        }
word_done:
        end += 4;
    }
byte_loop:
    while (arg0 < end) {
        u8 left = *arg0;
        u8 right = *arg1;
        if (left != right) {
            if (left < right) {
                return -1;
            }
            return 1;
        }
        arg0++;
        arg1++;
    }
    return 0;
}

/* Binary search for key in a sorted u32 array; returns its index or -1. */
s32 BinarySearch(u32 *array, u32 count, u32 key) {
    u32 lo;
    u32 hi;
    u32 mid;

    if (count != 0) {
        hi = count - 1;
        lo = 0;
        while (lo < hi) {
            mid = (lo + hi) >> 1;
            if (array[mid] < key) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (array[lo] == key) {
            return lo;
        }
        return -1;
    }
    return -1;
}

/* Binary search of a sorted array; index of key, or -1. */
static __inline__ s32 findSorted(u32 *array, u32 count, u32 key) {
    u32 lo;
    u32 hi;
    u32 mid;

    if (count != 0) {
        hi = count - 1;
        lo = 0;
        while (lo < hi) {
            mid = (lo + hi) >> 1;
            if (array[mid] < key) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (array[lo] == key) {
            return lo;
        }
        return -1;
    }
    return -1;
}

/* Finds the run of entries equal to key; returns 0 when there is none. */
s32 BinaryRange(u32 *array, s32 count, u32 key, s32 *first, s32 *last) {
    s32 index = findSorted(array, count, key);
    s32 i;

    if (index == -1) {
        return 0;
    }
    for (i = index - 1; i != -1; i--) {
        if (array[i] != key) {
            break;
        }
    }
    *first = i + 1;
    for (index++; index < count; index++) {
        if (array[index] != key) {
            break;
        }
    }
    *last = index - 1;
    return 1;
}

s32 BitBytes(s32 arg0) {
    s32 var_v0;

    var_v0 = arg0 + 7;
    if (var_v0 < 0) {
        var_v0 = arg0 + 0xE;
    }
    return var_v0 >> 3;
}

s32 BitTest(void *arg0, s32 arg1)
{
  s32 temp_a1;
  s32 temp_v0;
  s32 var_a1;
  u8 *temp_a1_2;
  int new_var;
  var_a1 = arg1;
  temp_v0 = var_a1;
  if (temp_v0 < 0)
  {
    var_a1 = temp_v0 + 7;
  }
  temp_a1 = var_a1 >> 3;
  new_var = 1 << (temp_v0 - (temp_a1 * 8));
  temp_a1_2 = (u8 *)arg0 + temp_a1;
  return (*temp_a1_2 & new_var) != 0;
}

void BitSet(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    s32 temp_a3;
    s32 var_v0;
    u8 *temp_v0;
    u8 *temp_v1;

    var_v0 = arg1;
    if (arg1 < 0) {
        var_v0 = arg1 + 7;
    }
    temp_a3 = var_v0 >> 3;
    temp_a0 = 1 << (arg1 - (temp_a3 * 8));
    if (arg2 != 0) {
        temp_v1 = arg0 + temp_a3;
        *temp_v1 |= temp_a0;
        return;
    }
    temp_v0 = arg0 + temp_a3;
    *temp_v0 &= ~temp_a0;
}
void BitToggle(s32 arg0, s32 arg1);

void BitToggle(s32 arg0, s32 arg1)
{
  s32 temp_a1;
  s32 temp_v0;
  s32 var_a1;
  u8 *temp_a1_2;
  int new_var;
  var_a1 = arg1;
  temp_v0 = var_a1;
  if (temp_v0 < 0)
  {
    var_a1 = temp_v0 + 7;
  }
  temp_a1 = var_a1 >> 3;
  new_var = 1 << (temp_v0 - (temp_a1 * 8));
  temp_a1_2 = arg0 + temp_a1;
  *temp_a1_2 ^= new_var;
}

extern float func_002974C0(float x);
f32 SmoothStep(f32 arg0);

f32 SmoothStep(f32 arg0)
{
  f32 var_f1;
  f32 var_f0;
  var_f0 = arg0;
  if (var_f0 < 0.0f)
  {
    var_f1 = 0.0f;
    var_f0 = var_f1;
  }
  else
  {
    var_f1 = 1.0f;
    if (var_f1 < var_f0)
    {
      var_f0 = var_f1;
    }
  }
  return (1.0f - func_002974C0(var_f0 * 3.141593f)) * 0.5f;
}

#ifdef TARGET_PORT
/* Host body (round 7 v2): the N64 tests the game's data segments (gHashSize + the size symbol
 * D_11059, gPrimagenPaths + D_94F), meaningless in the host link. The only caller
 * (CAI__UpdateModeAnim) asks it of an animation word that is not code: an id (small) or a
 * table in the game data. */
extern int IsCodePointer(uptr arg0);
int IsDataPointer(uptr arg0) {
    return arg0 >= 0x10000 && !IsCodePointer(arg0);
}
#else
extern char D_11059;
extern char D_94F;
int IsDataPointer(uptr arg0) {
    if (arg0 >= (uptr)&gHashSize &&
        arg0 < (uptr)&gHashSize + (uptr)&D_11059) {
        return 1;
    }
    if (arg0 >= (uptr)&gPrimagenPaths &&
        arg0 < (uptr)&gPrimagenPaths + (uptr)&D_94F) {
        return 1;
    }
    return 0;
}
#endif
#ifdef TARGET_PORT
/* Host body (round 7 v2): the N64 tests the address against the game's code segments
 * (CList__Construct + the size symbol D_A4694, Automap_RevealRegion + D_29E60), which mean
 * nothing in the host link (and the u32 argument cut host pointers). A mode row's animation
 * word (CAIModeInfo.unk_10, CAI__UpdateModeAnim) is an id, an animation picker function or a
 * table: here a code pointer is one in the executable's text. */
#if defined(__APPLE__)
#include <mach-o/getsect.h>
#include <mach-o/ldsyms.h>
int IsCodePointer(uptr arg0) {
    static uptr sBegin, sEnd;
    if (sEnd == 0) {
        unsigned long size = 0;
        u8 *text = getsectiondata(&_mh_execute_header, "__TEXT", "__text", &size);
        sBegin = (uptr) text;
        sEnd = (uptr) text + size;
    }
    return arg0 >= sBegin && arg0 < sEnd;
}
#else
extern char __executable_start[];
extern char etext[];
int IsCodePointer(uptr arg0) {
    return arg0 >= (uptr) __executable_start && arg0 < (uptr) etext;
}
#endif
#else
extern char CList__Construct;      /* local: a code address, the start of a range (a function, not data) */
extern char D_A4694;
extern char Automap_RevealRegion;  /* local: a code address, the start of a range (a function, not data) */
extern char D_29E60;
int IsCodePointer(u32 arg0) {
    if (arg0 >= (uptr)&CList__Construct &&
        arg0 < (uptr)&CList__Construct + (uptr)&D_A4694) {
        return 1;
    }
    if (arg0 >= (uptr)&Automap_RevealRegion &&
        arg0 < (uptr)&Automap_RevealRegion + (uptr)&D_29E60) {
        return 1;
    }
    return 0;
}
#endif

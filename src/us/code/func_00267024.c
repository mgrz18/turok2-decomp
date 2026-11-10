/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

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
extern char CList__Construct;
extern char D_A4694;
extern char Automap_RevealRegion;
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

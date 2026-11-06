/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404018.c, func_00404018.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "structs/controller.h"
#include "globals/pak.h"

extern OSMesgQueue gSiMessageQueue;
s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, int channel);
s32 Pak_RefreshNotes(s32 pad);
s32 Pak_Mount(s32 pad) {
    s32 i;
    s32 r;
    s32 v;
    s32 code;
    s32 ok;
    s32 ret;
    if (gContStatus[pad].errno != 0) {
        gPakError[pad] = -2;
        ret = 0;
    } else {
        i = 0;
        do {
            r = osPfsInitPak(&gSiMessageQueue, &gPakFileSystems[pad], pad);
            switch (r) {
            case 0:
            case 2:
                gPakError[pad] = -5;
                break;
            case 1:
                gPakError[pad] = -2;
                break;
            case 4:
            case 11:
                code = -3;
                goto st;
            case 10:
                code = -4;
            st:
                gPakError[pad] = code;
                break;
            }
            if (gPakError[pad] == -5) {
                goto got5;
            }
            if (gPakError[pad] == -2) {
                goto fin;
            }
            i++;
        } while (i < 4);
        goto fin2;
    got5:
        gPakState[pad] = 3;
        if (Pak_RefreshNotes(pad) != 0) {
            gPakError[pad] = -4;
            goto fin2;
        }
        gPakError[pad] = 0;
    fin:
    fin2:
        v = gPakError[pad];
        ok = 0;
        if (v == 0 || v == -4) {
            ok = 1;
        }
        ret = ok;
    }
    return ret;
}

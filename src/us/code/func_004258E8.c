/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404018.c, func_00404018.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern O01Pad4 D_800F6778[];
extern OSMesgQueue D_800F6798;
extern OSPfs D_80130C80[];
extern s32 D_80130C70[];
extern s32 D_80130C60[];
s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, int channel);
s32 func_00425760(s32 pad);
s32 func_004258E8(s32 pad) {
    s32 i;
    s32 r;
    s32 v;
    s32 code;
    s32 ok;
    s32 ret;
    if (D_800F6778[pad].unk3 != 0) {
        D_80130C70[pad] = -2;
        ret = 0;
    } else {
        i = 0;
        do {
            r = osPfsInitPak(&D_800F6798, &D_80130C80[pad], pad);
            switch (r) {
            case 0:
            case 2:
                D_80130C70[pad] = -5;
                break;
            case 1:
                D_80130C70[pad] = -2;
                break;
            case 4:
            case 11:
                code = -3;
                goto st;
            case 10:
                code = -4;
            st:
                D_80130C70[pad] = code;
                break;
            }
            if (D_80130C70[pad] == -5) {
                goto got5;
            }
            if (D_80130C70[pad] == -2) {
                goto fin;
            }
            i++;
        } while (i < 4);
        goto fin2;
    got5:
        D_80130C60[pad] = 3;
        if (func_00425760(pad) != 0) {
            D_80130C70[pad] = -4;
            goto fin2;
        }
        D_80130C70[pad] = 0;
    fin:
    fin2:
        v = D_80130C70[pad];
        ok = 0;
        if (v == 0 || v == -4) {
            ok = 1;
        }
        ret = ok;
    }
    return ret;
}

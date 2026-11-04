/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC.CPP, func_804041F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"

extern s32 func_00285878(s32 arg0);
extern u8 D_800F677B[];
extern void func_002858A4();
extern void func_002847E0(void);
extern u8 D_800F6750;
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
extern s32 func_004258E8(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
s32 func_004258E8(s32 ch);
void func_00425AB8(void) {
    extern u8 D_800F6798[];
    s32 channel;
    s32 present;
    RumblePak *record;
    func_00285878(1);
    func_002847E0();
    D_800F6750 = 2;
    for (channel = 0; channel < 4; channel++) {
        record = &D_80130AA0[channel];
        if (D_800F677B[channel * 4] != 0) {
            present = 0;
            record->present = 0;
        } else {
            record->present = osMotorInit(D_800F6798, &record->pfs, channel) == 0;
            present = record->present;
        }
        if (present) D_80130C60[channel] = 2;
        else if (func_004258E8(channel) != 0) D_80130C60[channel] = 3;
        else D_80130C60[channel] = 1;
    }
    func_002858A4();
}

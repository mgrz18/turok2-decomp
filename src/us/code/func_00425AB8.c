/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC.CPP, func_804041F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern s32 Controller_Lock(s32 arg0);
extern u8 D_800F677B[];
extern void Controller_Unlock();
extern void Controller_ReadAll(void);
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
extern s32 Pak_Mount(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
s32 Pak_Mount(s32 ch);
void Pak_ProbeAll(void) {
    extern u8 gSiMessageQueue[];
    s32 channel;
    s32 present;
    RumblePak *record;
    Controller_Lock(1);
    Controller_ReadAll();
    gPakQueueState = 2;
    for (channel = 0; channel < 4; channel++) {
        record = &gRumblePaks[channel];
        if (D_800F677B[channel * 4] != 0) {
            present = 0;
            record->present = 0;
        } else {
            record->present = osMotorInit(gSiMessageQueue, &record->pfs, channel) == 0;
            present = record->present;
        }
        if (present) gPakState[channel] = 2;
        else if (Pak_Mount(channel) != 0) gPakState[channel] = 3;
        else gPakState[channel] = 1;
    }
    Controller_Unlock();
}

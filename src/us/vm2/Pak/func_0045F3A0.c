/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_7.CPP, func_80404E28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "structs/controller.h"

extern s32 Controller_Lock(s32 flag);
extern void Controller_ReadAll(void);
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
extern s32 Pak_Mount_2(s32 a0);
extern void Controller_Unlock(void);
void Pak_ProbeSlot_2(s32 kind)
{
    RumblePak *cur;
    s32 ready;
    ((void (*)(s32))Controller_Lock)(1);
    Controller_ReadAll();
    gPakQueueState = 2;
    cur = &gRumblePaks[kind];
    if (gContStatus[kind].errno != 0) {
        cur->present = 0;
        ready = 0;
    } else {
        cur->present = osMotorInit(&gSiMessageQueue, &cur->pfs, kind) == 0;
        ready = cur->present;
    }
    if (ready != 0) {
        gPakState[kind] = 2;
    } else if (Pak_Mount_2(kind) != 0) {
        gPakState[kind] = 3;
    } else {
        gPakState[kind] = 1;
    }
    Controller_Unlock();
}

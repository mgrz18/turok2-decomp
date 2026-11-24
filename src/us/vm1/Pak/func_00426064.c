/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404858.c, func_00404858.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"
#include "globals/pak.h"

s32 Controller_Lock(s32 flag);
void Controller_ReadAll();
void Controller_Unlock(void);
s32 Pak_Mount(s32 ch);
s32 osPfsDeleteFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);
s32 Pak_DeleteNote(s32 pad, s32 idx) {
    s32 r;
    if (gPakState[pad] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    r = gPakError[pad];
    gPakQueueState = 2;
    if (r == 0) {
        r = osPfsDeleteFile(&gPakFileSystems[pad], gPakDirectories[pad].notes[idx].company_code, gPakDirectories[pad].notes[idx].game_code, gPakDirectories[pad].notes[idx].game_name, gPakDirectories[pad].notes[idx].ext_name);
        if (r != 0) {
            r = -1;
        }
        Pak_Mount(pad);
    }
    Controller_Unlock();
    return r;
}

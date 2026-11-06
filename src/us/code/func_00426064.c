/* WRONG CUT: REAL Pak_DeleteNote size 0x100 (splat cut it into Pak_DeleteNote + func_00426084). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404858_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern u8 gPakQueueState;

extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern s32 Pak_Mount(s32 ch);
extern s32 osPfsDeleteFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);

s32 Pak_DeleteNote(s32 ch, s32 index) {
    s32 result;

    if (gPakState[ch] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        result = osPfsDeleteFile(&gPakFileSystems[ch], gPakDirectories[ch].notes[index].company_code,
                                 gPakDirectories[ch].notes[index].game_code,
                                 gPakDirectories[ch].notes[index].game_name,
                                 gPakDirectories[ch].notes[index].ext_name);
        if (result != 0) {
            result = -1;
        }
        Pak_Mount(ch);
    }
    Controller_Unlock();
    return result;
}

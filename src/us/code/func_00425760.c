#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

/* Refreshes the cached state of the 16 notes of a controller pak (osPfsFileState, retried once; an invalid
 * note is cleared) and its free space. */
extern s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state); /* osPfsFileState */
extern s32 osPfsFreeBlocks(OSPfs *pfs, s32 *bytes);

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 Pak_RefreshNotes(s32 channel) {
    s32 i;
    s32 ret;
    s32 free;

    if (gPakDirectories != NULL) {
        for (i = 0; i < 16; i++) {
            ret = osPfsFileState(&gPakFileSystems[channel], i, &gPakDirectories[channel].notes[i]);
            if (ret != 0 && ret != 5) {
                ret = osPfsFileState(&gPakFileSystems[channel], i, &gPakDirectories[channel].notes[i]);
            }
            if (ret == 5) {
                gPakDirectories[channel].notes[i].file_size = 0;
                gPakDirectories[channel].notes[i].game_code = 0;
                gPakDirectories[channel].notes[i].company_code = 0;
                gPakDirectories[channel].notes[i].ext_name[0] = 0;
                gPakDirectories[channel].notes[i].game_name[0] = 0;
            }
            if (osPfsFreeBlocks(&gPakFileSystems[channel], &free) == 0) {
                gPakDirectories[channel].freeBytes = free;
            }
        }
    }
    return osPfsFreeBlocks(&gPakFileSystems[channel], &free);
}
#else
void Pak_RefreshNotes(s32 channel) {
    s32 i;
    s32 ret;
    s32 free;

    if (gPakDirectories != NULL) {
        for (i = 0; i < 16; i++) {
            ret = osPfsFileState(&gPakFileSystems[channel], i, &gPakDirectories[channel].notes[i]);
            if (ret != 0 && ret != 5) {
                ret = osPfsFileState(&gPakFileSystems[channel], i, &gPakDirectories[channel].notes[i]);
            }
            if (ret == 5) {
                gPakDirectories[channel].notes[i].file_size = 0;
                gPakDirectories[channel].notes[i].game_code = 0;
                gPakDirectories[channel].notes[i].company_code = 0;
                gPakDirectories[channel].notes[i].ext_name[0] = 0;
                gPakDirectories[channel].notes[i].game_name[0] = 0;
            }
            if (osPfsFreeBlocks(&gPakFileSystems[channel], &free) == 0) {
                gPakDirectories[channel].freeBytes = free;
            }
        }
    }
    osPfsFreeBlocks(&gPakFileSystems[channel], &free);
}
#endif

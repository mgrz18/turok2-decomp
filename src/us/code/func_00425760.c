#include "common.h"
#include "structs/pak.h"

/* Refreshes the cached state of the 16 notes of a controller pak (osPfsFileState, retried once; an invalid
 * note is cleared) and its free space. */
extern s32 func_00427C20(OSPfs *pfs, s32 fileNo, OSPfsState *state); /* osPfsFileState */
extern s32 osPfsFreeBlocks(OSPfs *pfs, s32 *bytes);

void func_00425760(s32 channel) {
    s32 i;
    s32 ret;
    s32 free;

    if (D_800C2224 != NULL) {
        for (i = 0; i < 16; i++) {
            ret = func_00427C20(&D_80130C80[channel], i, &D_800C2224[channel].notes[i]);
            if (ret != 0 && ret != 5) {
                ret = func_00427C20(&D_80130C80[channel], i, &D_800C2224[channel].notes[i]);
            }
            if (ret == 5) {
                D_800C2224[channel].notes[i].file_size = 0;
                D_800C2224[channel].notes[i].game_code = 0;
                D_800C2224[channel].notes[i].company_code = 0;
                D_800C2224[channel].notes[i].ext_name[0] = 0;
                D_800C2224[channel].notes[i].game_name[0] = 0;
            }
            if (osPfsFreeBlocks(&D_80130C80[channel], &free) == 0) {
                D_800C2224[channel].freeBytes = free;
            }
        }
    }
    osPfsFreeBlocks(&D_80130C80[channel], &free);
}

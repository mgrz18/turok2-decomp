/* WRONG CUT: real function Pak_RefreshNotes_2, size 0x188: splat's func_0045E674 starts with 3 junk words (0x45E674-0x45E680, linker fill after the hand-written func_0045E630). */
#include "common.h"
#include "structs/pak.h"

extern s32 func_00427B40(OSPfs *pfs, s32 file_no, OSPfsState *state); /* osPfsFileState */
extern s32 func_00427D80(OSPfs *pfs, s32 *bytes);                  /* osPfsFreeBlocks */

/* Re-reads the directory of the pak on channel ch into the cache. */
void Pak_RefreshNotes_2(s32 ch) {
    s32 i;
    s32 ret;
    s32 bytes;

    if (D_800C2224 != NULL) {
        for (i = 0; i < 16; i++) {
            ret = func_00427B40(&D_80130C80[ch], i, &D_800C2224[ch].notes[i]);
            if (ret != 0 && ret != 5) {
                ret = func_00427B40(&D_80130C80[ch], i, &D_800C2224[ch].notes[i]);
            }
            if (ret == 5) {
                D_800C2224[ch].notes[i].file_size = 0;
                D_800C2224[ch].notes[i].game_code = 0;
                D_800C2224[ch].notes[i].company_code = 0;
                D_800C2224[ch].notes[i].ext_name[0] = 0;
                D_800C2224[ch].notes[i].game_name[0] = 0;
            }
            if (func_00427D80(&D_80130C80[ch], &bytes) == 0) {
                D_800C2224[ch].freeBytes = bytes;
            }
        }
    }
    func_00427D80(&D_80130C80[ch], &bytes);
}

/* WRONG CUT: real function func_0045E680, size 0x188: splat's func_0045E674 starts with 3 junk words (0x45E674-0x45E680, linker fill after the hand-written func_0045E630). */
#include "common.h"

typedef struct PfsPak {
    u8 pad[0x68];
} PfsPak;

/* One note of the cached Controller Pak directory (an OSPfsState). */
typedef struct PakNote {
    /* 0x00 */ u32 file_size;
    /* 0x04 */ u32 game_code;
    /* 0x08 */ u16 company_code;
    /* 0x0A */ u8 ext_name[4];
    /* 0x0E */ u8 game_name[16];
    /* 0x1E */ u8 pad1E[2];
} PakNote;

typedef struct PakDirectory {
    /* 0x000 */ s32 free;
    /* 0x004 */ PakNote notes[16];
} PakDirectory; /* 0x204 */

extern PfsPak D_80130C80[];
extern PakDirectory *D_800C2224;

extern s32 func_00427B40(PfsPak *pfs, s32 file_no, PakNote *state); /* osPfsFileState */
extern s32 func_00427D80(PfsPak *pfs, s32 *bytes);                  /* osPfsFreeBlocks */

/* Re-reads the directory of the pak on channel ch into the cache. */
void func_0045E680(s32 ch) {
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
                D_800C2224[ch].free = bytes;
            }
        }
    }
    func_00427D80(&D_80130C80[ch], &bytes);
}

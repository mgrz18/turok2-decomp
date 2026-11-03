#include "common.h"

/* Refreshes the cached state of the 16 notes of a controller pak (osPfsFileState, retried once; an invalid
 * note is cleared) and its free space. */

typedef struct {
    /* 0x00 */ u8 data[0x68];
} PfsHandle; /* OSPfs */

typedef struct {
    /* 0x00 */ u32 file_size;
    /* 0x04 */ u32 game_code;
    /* 0x08 */ u16 company_code;
    /* 0x0A */ char ext_name[4];
    /* 0x0E */ char game_name[16];
} PfsNote; /* OSPfsState, 0x20 */

typedef struct {
    /* 0x000 */ s32 freeBytes;
    /* 0x004 */ PfsNote notes[16];
} PakCache; /* 0x204 */

extern PakCache *D_800C2224;
extern PfsHandle D_80130C80[];
extern s32 func_00427C20(PfsHandle *pfs, s32 fileNo, PfsNote *state); /* osPfsFileState */
extern s32 osPfsFreeBlocks(PfsHandle *pfs, s32 *bytes);

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

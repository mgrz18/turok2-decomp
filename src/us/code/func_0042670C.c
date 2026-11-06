/* WRONG CUT: REAL Pak_CountEmptyNotes size 0x94 (splat cut it into Pak_CountEmptyNotes + func_00426734). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_8.CPP, func_804050CC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern void **gPakStateBuffer;
extern s32 gPakDirectoryAllocated;
extern u8 gPakQueueState;

extern void **CCache__Allocate(s32, s32, s32, char *);
extern void CCache__Flush(s32);
extern void CCache__Deallocate(s32, void **);
extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 Pak_Mount(s32 ch);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

/* Counts the empty notes of the pak on channel ch. */
s32 Pak_CountEmptyNotes(s32 ch, s32 *count) {
    s32 i;

    if (gPakState[ch] != 3) {
        return -2;
    }
    *count = 0;
    if (gPakError[ch] == 0) {
        for (i = 0; i < 16; i++) {
            if (gPakDirectories[ch].notes[i].file_size == 0) {
                (*count)++;
            }
        }
    }
    return gPakError[ch];
}

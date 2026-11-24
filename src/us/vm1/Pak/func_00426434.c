/* WRONG CUT: REAL Pak_FreeDirectory size 0x4C (splat cut its last word into func_0042647C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_7.CPP, func_80404DDC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"


extern void **CCache__Allocate(s32, s32, s32, char *);
extern void CCache__Flush(s32);
extern void CCache__Deallocate(sptr, void **);
extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 Pak_Mount(s32 ch);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

/* Releases the pak directory buffer and clears its global state. */
void Pak_FreeDirectory(void) {
    CCache__Flush(0);
    if (gPakStateBuffer) {
        CCache__Deallocate(0, gPakStateBuffer);
        gPakStateBuffer = 0;
        gPakDirectories = 0;
    }
    gPakDirectoryAllocated = 0;
}

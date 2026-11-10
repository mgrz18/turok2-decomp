/* Second-module copy of Pak_ReadNote (first module; this one at link address 0x004264D0), calling this module's own functions at their link addresses. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404958_read, with Turok 2's layouts. */
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
extern s32 osPfsReadWriteFile_2(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

extern char D_800C18B0[];

/* Reads note file_no of the pak on channel ch into dst. */
s32 Pak_ReadNote_2(s32 ch, s32 file_no, void *dst, s32 size) {
    s32 result;
    s32 blocks;
    void **handle;
    void *buf;

    blocks = (size + 0xFF) & ~0xFF;
    if (gPakState[ch] != 3) {
        return -2;
    }
    handle = CCache__Allocate(0, blocks, 0x23, D_800C18B0);
    buf = *handle;
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        if ((result = osPfsReadWriteFile_2(&gPakFileSystems[ch], file_no, 0, 0, blocks, buf)) != 0) {
            result = -1;
        }
        if (result == 0) {
            func_0029E460(dst, buf, size);
        }
    }
    Controller_Unlock();
    CCache__Flush(0);
    if (handle != 0) {
        CCache__Deallocate(0, handle);
    }
    return result;
}

/* Second-module copy of Pak_SaveNoteVerified (first module; this one at link address 0x00426084), calling this module's own functions at their link addresses. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404958_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern char D_800C18B0[];
extern char D_800C18C8[];
extern u8 D_800C18E0[];

extern void **CCache__Allocate(s32, s32, s32, char *);
extern void CCache__Flush(s32);
extern void CCache__Deallocate(s32, void **);
extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 memcmp(void *a, void *b, s32 size);
extern s32 Pak_WriteNote_2(s32 ch, s32 size, void *data, u8 *game_name, s32 *file_no, u8 *ext_name,
                         u8 *company, u8 *code);
extern s32 Pak_DeleteNote_2(s32 ch, s32 index);
extern s32 osPfsReadWriteFile_2(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);

/* Reads note file_no of the pak on channel ch into dst. */
static __inline__ s32 readNote(s32 ch, s32 file_no, void *dst, s32 size) {
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

/* Saves data as a note on the pak on channel ch and reads it back to verify it, up to four times. */
s32 Pak_SaveNoteVerified_2(s32 ch, s32 size, void *data, u8 *name, u8 *ext, u8 *code) {
    s32 result;
    s32 file_no;
    void **handle;
    void *buf;
    s32 blocks;
    s32 tries;

    blocks = (size + 0xFF) & ~0xFF;
    if (gPakState[ch] != 3) {
        return -2;
    }
    handle = CCache__Allocate(0, blocks, 0x23, D_800C18C8);
    buf = *handle;
    for (tries = 0; tries < 4; tries++) {
        result = Pak_WriteNote_2(ch, size, data, name, &file_no, ext, D_800C18E0, code);
        if (result != 0) {
            break;
        }
        if (readNote(ch, file_no, buf, blocks) == 0 && memcmp(buf, data, size) == 0) {
            break;
        }
        if (Pak_DeleteNote_2(ch, file_no) != 0) {
            break;
        }
    }
    CCache__Flush(0);
    if (handle != 0) {
        CCache__Deallocate(0, handle);
    }
    return result;
}

/* WRONG CUT: REAL Pak_FindFreeExtLetter size 0x144 (splat cut it into Pak_FindFreeExtLetter + func_00426AF8 + func_00426BA8). */
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

extern s32 Pak_FindNote(s32 ch, s32 index, s32 *exists, u8 *name, u8 *ext, s32 *size, u8 *company,
                         u8 *code);

/* Picks the first note extension letter ('A' + n) no note of the pak on channel ch uses. */
s32 Pak_FindFreeExtLetter(s32 ch, u8 *ext) {
    u8 company[8];
    u8 code[8];
    u8 name[16];
    s32 exists;
    s32 size;
    s32 used;
    s32 i;

    used = 0;
    if (gPakState[ch] != 3) {
        return -2;
    }
    for (i = 0; i < 16; i++) {
        if (Pak_FindNote(ch, i, &exists, name, ext, &size, company, code) != 0) {
            return -1;
        }
        if (exists) {
            used |= 1 << (ext[0] - 'A');
        }
    }
    ext[0] = 'A';
    ext[1] = 0;
    ext[2] = 0;
    ext[3] = 0;
    for (i = 0; i < 16; i++) {
        if (!((used & (1 << i)) & 0xFFFF)) {
            break;
        }
        ext[0]++;
    }
    if (i >= 16) {
        return -1;
    }
    return 0;
}

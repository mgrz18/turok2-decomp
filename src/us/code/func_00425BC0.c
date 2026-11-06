/* one function 0x00425BC0-0x00425E74 (size 0x2B4); splat cut its tail into func_00425D94. MATCHES with that size (twin of Pak_WriteNote_2). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC.CPP, func_804042F0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"


extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */
/* osPfsAllocateFile, osPfsReadWriteFile, osPfsDeleteFile and the directory refresh, in the first module */
extern s32 osPfsAllocateFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 size, s32 *file_no);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);
extern s32 osPfsDeleteFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);
extern s32 Pak_Mount(s32 ch);

static __inline__ s32 ToUpper(s32 c) {
    if (c - 'a' < 26U) {
        c -= 0x20;
    }
    return c;
}

/* Upper-cases src in place and converts it to pak font codes in dst (0 for a character it lacks). */
static __inline__ void EncodePakText(u8 *dst, u8 *src, s32 n) {
    s32 i;
    s32 j;

    for (i = 0; i < n; i++) {
        src[i] = ToUpper(src[i]);
        for (j = 0; j < 0x42; j++) {
            if (src[i] == gPakCharTable[j]) {
                dst[i] = j;
                break;
            }
        }
        if (j == 0x42) {
            dst[i] = 0;
        }
    }
}

/* Writes data as a note on the pak of channel ch; returns 0, -1 on failure or -2 if the pak is not ready. */
s32 Pak_WriteNote(s32 ch, s32 size, void *data, u8 *game_name, s32 *file_no, u8 *ext_name, u8 *company, u8 *code) {
    s32 result;
    u8 name[16];
    u8 ext[4];
    u16 company_code;
    u32 game_code;
    OSPfs *pfs;
    s32 offset;

    if (gPakState[ch] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        EncodePakText(ext, ext_name, 4);
        size = (size + 0xFF) & ~0xFF;
        func_0029E460(name, game_name, 16);
        EncodePakText(name, game_name, 16);
        pfs = &gPakFileSystems[ch];
        company_code = (company[0] << 8) | company[1];
        game_code = (code[0] << 24) | (code[1] << 16) | (code[2] << 8) | code[3];
        result = osPfsAllocateFile(pfs, company_code, game_code, name, ext, size, file_no);
        if (result != 0) {
            result = -1;
        }
        offset = 0;
        if (result == 0) {
            result = osPfsReadWriteFile(pfs, *file_no, 1, offset, size, data);
            if (result != 0) {
                osPfsDeleteFile(pfs, company_code, game_code, name, ext);
                result = -1;
            }
        }
        Pak_Mount(ch);
    }
    Controller_Unlock();
    return result;
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC.CPP, func_804042F0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"

extern u8 D_800C222C[];  /* pak font code -> character */
extern u8 D_800F6750;

extern void func_00285878(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
extern void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */
/* osPfsAllocateFile, osPfsReadWriteFile, osPfsDeleteFile and the directory refresh, at this module's link addresses */
extern s32 func_004270A0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 size, s32 *file_no);
extern s32 func_00428C10(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);
extern s32 func_004276A0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);
extern s32 func_00425808(s32 ch);

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
            if (src[i] == D_800C222C[j]) {
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
s32 func_0045EAE0(s32 ch, s32 size, void *data, u8 *game_name, s32 *file_no, u8 *ext_name, u8 *company, u8 *code) {
    s32 result;
    u8 name[16];
    u8 ext[4];
    u16 company_code;
    u32 game_code;
    OSPfs *pfs;
    s32 offset;

    if (D_80130C60[ch] != 3) {
        return -2;
    }
    func_00285878(1);
    func_002847E0();
    result = D_80130C70[ch];
    D_800F6750 = 2;
    if (result == 0) {
        EncodePakText(ext, ext_name, 4);
        size = (size + 0xFF) & ~0xFF;
        func_0029E460(name, game_name, 16);
        EncodePakText(name, game_name, 16);
        pfs = &D_80130C80[ch];
        company_code = (company[0] << 8) | company[1];
        game_code = (code[0] << 24) | (code[1] << 16) | (code[2] << 8) | code[3];
        result = func_004270A0(pfs, company_code, game_code, name, ext, size, file_no);
        if (result != 0) {
            result = -1;
        }
        offset = 0;
        if (result == 0) {
            result = func_00428C10(pfs, *file_no, 1, offset, size, data);
            if (result != 0) {
                func_004276A0(pfs, company_code, game_code, name, ext);
                result = -1;
            }
        }
        func_00425808(ch);
    }
    func_002858A4();
    return result;
}

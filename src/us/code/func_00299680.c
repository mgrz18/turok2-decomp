/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_22.c, func_802B84C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern u8 D_80130E20[];
extern void func_00299FE4(void);
extern s32 func_0029A080(s32, s32);
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern u32 func_00299E48(void *arg0);
extern void func_0029A050(void);
extern u8 D_80121420;
extern u8 D_80130E20[];
extern void func_002998B0(s32 arg0, s32 arg1);
extern s32 func_0029A080(s32, s32);
extern u32 func_00299E48(void *arg0);
extern u32 func_00427F80(s32 arg0, s32 arg1);
u32 func_00299680(s32 arg0, s32 arg1, u16 arg2, u8 *arg3) {
    Block40 saved;
    u8 *source;
    s32 i;
    s32 retries;
    u32 result;
    source = D_80130E20;
    retries = 2;
    func_00299FE4();
    D_80121420 = 2;
    func_002998B0(arg1, arg2 & 0xFFFF);
    func_0029A080(1, D_80130E20);
    osRecvMesg(arg0, 0, 1);
    do {
        func_0029A080(0, D_80130E20);
        osRecvMesg(arg0, 0, 1);
        source = D_80130E20;
        if (arg1 != 0) {
            i = 0;
            if (arg1 > 0) {
                do {
                    i++;
                    source++;
                } while (i < arg1);
            }
        }
        saved = *(Block40 *)source;
        result = (saved.bytes[2] & 0xC0) >> 4;
        if (result == 0) {
            if ((func_00299E48(&saved.bytes[6]) & 0xFF) != saved.bytes[0x26]) {
                result = func_00427F80(arg0, arg1);
                if (result != 0) {
                    break;
                }
                result = 4;
            } else {
                i = 0;
                do {
                    *arg3++ = ((Block40 *)((u8 *)&saved + i))->bytes[6];
                    i++;
                } while (i < 0x20);
            }
        } else {
            result = 1;
        }
        if (result != 4) {
            break;
        }
    } while (retries-- >= 0);
    func_0029A050();
    return result;
}

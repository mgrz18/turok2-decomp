/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_11.CPP, func_80254284_de, with Turok 2's layouts. */

#include "common.h"

typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

extern OSMesgQueue D_800D8DB0;      /* resource lock queue */
extern s32 D_800D8DCC;              /* resource lock depth */
extern char D_800A51E0[];

u32 func_0029DFF0(void);            /* __osDisableInt */
void func_0029E010(u32 mask);       /* __osRestoreInt */
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void **func_0020367C(s32, s32, s32, s32, s32, s32, void *, s32);
void func_002061B8(s32, void **);

/* Loads a four-byte probe to read a count, frees it under the resource lock, then loads count*4+15
 * rounded to eight bytes and stores its first word through arg1, returning the allocation. */
void **func_00205B94(s32 arg0, void **arg1, s32 arg2, void *arg3) {
    void **resource;
    s32 size;
    s32 counter;
    s32 counter2;
    u32 token;
    u32 token2;
    s32 one = 1;

    resource = func_0020367C(0, arg2, 4, 0x1B, 0, 0, D_800A51E0, one);
    size = **(s32 **)resource;
    token = func_0029DFF0();
    counter = D_800D8DCC + one;
    D_800D8DCC = counter;
    if (counter != one) {
        func_0029E010(token);
        osRecvMesg(&D_800D8DB0, 0, one);
    } else {
        func_0029E010(token);
    }
    func_002061B8(0, resource);
    token2 = func_0029DFF0();
    counter2 = D_800D8DCC - 1;
    D_800D8DCC = counter2;
    if (counter2 != 0) {
        func_0029E010(token2);
        osSendMesg(&D_800D8DB0, 0, 1);
    } else {
        func_0029E010(token2);
    }
    resource = func_0020367C(0, arg2, ((size * 4) + 0xF) & ~7, 0x1B, 0, 0, arg3, 1);
    *arg1 = *resource;
    return resource;
}

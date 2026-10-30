#include "common.h"

/* Probe the controller pak in a port: 2 = no pak/error, 1 = not a usable pak,
 * 4 = pak changed, otherwise the timer result. */

typedef struct {
    u16 type; /* 0x00 */
    union {
        u16 word;
        struct {
            u8 status; /* 0x02 */
            u8 errno;  /* 0x03 */
        } b;
    } s;
} PakStatus;

extern u8 D_80130E20[];

void func_00427F78(s32 channel);
s32 func_0029A080(s32 enable, void *timer);
s32 osRecvMesg(void *mq, void *msg, s32 flag);
void func_00428004(s32 channel, PakStatus *status);

s32 func_00460EA0(void *mq, s32 channel) {
    PakStatus status;
    void *msg;
    s32 ret;

    func_00427F78(channel);
    func_0029A080(1, D_80130E20);
    osRecvMesg(mq, &msg, 1);
    ret = func_0029A080(0, D_80130E20);
    osRecvMesg(mq, &msg, 1);
    func_00428004(channel, &status);
    if ((status.s.b.status & 1) && (status.s.b.status & 2)) {
        return 2;
    }
    if ((status.s.word & 0x1FF) != 0x100) {
        return 1;
    }
    if (status.s.b.status & 4) {
        return 4;
    }
    return ret;
}

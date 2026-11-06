#include "common.h"
#include "globals/libultra.h"

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

void __osPfsRequestOneChannel_2(s32 channel);
s32 __osSiRawStartDma(s32 enable, void *timer);
s32 osRecvMesg(void *mq, void *msg, s32 flag);
void __osPfsGetOneChannelData_2(s32 channel, PakStatus *status);

s32 __osPfsGetStatus_2(void *mq, s32 channel) {
    PakStatus status;
    void *msg;
    s32 ret;

    __osPfsRequestOneChannel_2(channel);
    __osSiRawStartDma(1, &__osPfsPifRam);
    osRecvMesg(mq, &msg, 1);
    ret = __osSiRawStartDma(0, &__osPfsPifRam);
    osRecvMesg(mq, &msg, 1);
    __osPfsGetOneChannelData_2(channel, &status);
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

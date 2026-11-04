#include "common.h"

/* One of 17 0xCC-byte channels, from +4 (see func_00271904.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ s32 state;
    /* 0x54 */ u8 pad54[0xCC - 0x54];
} Channel;

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ Channel channels[17];
} ChannelSet;

/* Clears every channel's state. */
void func_00273E78(ChannelSet *arg0) {
    s32 var_v0;
    Channel *var_a0;

    var_a0 = arg0->channels;
    var_v0 = 0x10;
    do {
        var_a0->state = 0;
        var_v0 -= 1;
        var_a0++;
    } while (var_v0 >= 0);
}

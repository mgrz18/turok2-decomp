/* Second VM module copy of func_0042E91C (same source, linked 0x0042E83C). */
#include "common.h"

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x510];
    /* 0x0510 */ s32 unk510;
    /* 0x0514 */ s32 index;
    /* 0x0518 */ void *options;
    /* 0x051C */ u8 pad51C[0x92C - 0x51C];
    /* 0x092C */ s32 unk92C;
    /* 0x0930 */ s32 unk930;
    /* 0x0934 */ u8 pad934[0xA40 - 0x934];
    /* 0x0A40 */ u8 unkA40[0xC00 - 0xA40];
    /* 0x0C00 */ s32 unkC00;
    /* 0x0C04 */ u8 padC04[0x1288 - 0xC04];
    /* 0x1288 */ s32 sound0;
    /* 0x128C */ s32 sound1;
} Player;

typedef struct Controller {
    u8 pad[0x224];
} Controller;

extern Controller D_800F5EC0[];

extern void func_002470E8(void *, Controller *);
extern void func_00248340(Player *p);

void func_0046783C(Player *p, s32 index, void *options) {
    p->unk510 = 0;
    func_002470E8(p->unkA40, &D_800F5EC0[index]);
    p->index = index;
    p->options = options;
    p->sound0 = 0;
    p->sound1 = 0;
    p->unk92C = 0;
    p->unk930 = 0;
    p->unkC00 = 0;
    func_00248340(p);
}

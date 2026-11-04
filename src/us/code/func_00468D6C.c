#include "common.h"
#include "rw/abu_protos.h"

void *func_00266C5C(void *destination, s32 value, u32 size);
s32 func_0027D160(s32);

s32 func_0042F8D0(void *);                      /* extern */

/* Manager block cleared and set up here (0x11F0 bytes): three lists and
 * an embedded sub-object at 0x40. */
typedef struct {
    /* 0x0000 */ s32 unk0;
    /* 0x0004 */ s32 unk4;
    /* 0x0008 */ s32 unk8;
    /* 0x000C */ u8 list0C[0x20 - 0xC];
    /* 0x0020 */ u8 list20[0x34 - 0x20];
    /* 0x0034 */ u8 pad34[0x38 - 0x34];
    /* 0x0038 */ s32 unk38;
    /* 0x003C */ u8 pad3C[0x40 - 0x3C];
    /* 0x0040 */ u8 sub40[0xF00 - 0x40];
    /* 0x0F00 */ s32 unkF00;
    /* 0x0F04 */ s32 unkF04;
    /* 0x0F08 */ s32 unkF08;
    /* 0x0F0C */ u8 listF0C[0x11E8 - 0xF0C];
    /* 0x11E8 */ s32 unk11E8;
    /* 0x11EC */ u8 pad11EC[0x11F0 - 0x11EC];
} Unk00468D6C; /* size 0x11F0 */

void func_00468D6C(Unk00468D6C *arg0) {
    void *temp_s1;

    func_00266C5C(arg0, 0, 0x11F0U);
    CList__Construct(arg0->list0C, 0, 4);
    CList__Construct(arg0->list20, 0, 4);
    CList__Construct(arg0->listF0C, 0, 4);
    temp_s1 = arg0->sub40;
    arg0->unk11E8 = 1;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkF00 = 0;
    arg0->unkF04 = 0;
    arg0->unkF08 = 0;
    arg0->unk38 = 2;
    func_0042F8D0(temp_s1);
    func_0027D160((s32) temp_s1);
}

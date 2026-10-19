#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void *func_00266C5C(void *destination, s32 value, u32 size);
s32 func_0027D160(s32);

M2C_UNK func_0042F8D0(void *);                      /* extern */

void func_00468D6C(void *arg0) {
    void *temp_s1;

    func_00266C5C(arg0, 0, 0x11F0U);
    CList__Construct(arg0 + 0xC, 0, 4);
    CList__Construct(arg0 + 0x20, 0, 4);
    CList__Construct(arg0 + 0xF0C, 0, 4);
    temp_s1 = arg0 + 0x40;
    M2C_FIELD(arg0, s32 *, 0x11E8) = 1;
    M2C_FIELD(arg0, s32 *, 0) = 0;
    M2C_FIELD(arg0, s32 *, 4) = 0;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0xF00) = 0;
    M2C_FIELD(arg0, s32 *, 0xF04) = 0;
    M2C_FIELD(arg0, s32 *, 0xF08) = 0;
    M2C_FIELD(arg0, s32 *, 0x38) = 2;
    func_0042F8D0(temp_s1);
    func_0027D160((s32) temp_s1);
}

#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0027588C(s32);                         /* extern */
M2C_UNK func_00275D34(s32, void *);                 /* extern */
s32 func_00275F7C();                                /* extern */

void func_00253E68(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x1288);
    if (temp_v0 != NULL) {
        func_0027588C(M2C_FIELD(temp_v0, s32 *, 0xC));
    }
    func_00275D34(func_00275F7C(), M2C_FIELD(arg0, void **, 0x1288));
    M2C_FIELD(arg0, void **, 0x1288) = NULL;
}

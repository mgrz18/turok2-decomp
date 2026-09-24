#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00286674(M2C_UNK *);                   /* extern */
s32 func_0029DFD0();                                /* extern */
M2C_UNK func_0029E220(s32);                         /* extern */
M2C_UNK func_0029E3E0(M2C_UNK *, M2C_UNK *, M2C_UNK *, M2C_UNK *); /* extern */
extern M2C_UNK D_800A9D3C;
extern M2C_UNK D_800A9D44;
extern M2C_UNK D_800A9D50;
extern M2C_UNK D_800F6CB0;
extern M2C_UNK D_8011F188;

void func_00288860(void) {
    func_0029E3E0(&D_8011F188, &D_800A9D3C, &D_800A9D44, &D_800A9D50);
    func_0029E220(func_0029DFD0() | 0x400);
    func_00286674(&D_800F6CB0);
}

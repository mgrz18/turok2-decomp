#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void func_0020EF2C(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2);
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);

void func_0023C1D8(Vec3 *arg0) {
    Vec3 sp10;
    Vec3 sp20;
    Vec3 *temp_s1;

    temp_s1 = ((__typeof__(arg0))((s8 *)arg0 + 0xC));
    func_0020EF2C(&sp10, temp_s1, arg0);
    func_0020EF2C(&sp20, ((__typeof__(arg0))((s8 *)arg0 + 0x18)), temp_s1);
    func_0020EFDC(((__typeof__(arg0))((s8 *)arg0 + 0x30)), &sp10, &sp20);
}

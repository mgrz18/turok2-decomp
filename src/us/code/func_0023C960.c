#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void func_0020EEF8(Vec3 *, Vec3 *, Vec3 *);

void func_0023C960(Vec3 *arg0, Vec3 *arg1) {
    Vec3 *temp_a0;
    Vec3 *temp_a0_2;
    Vec3 *temp_s0;

    func_0020EEF8(arg0, arg0, arg1);
    temp_a0 = ((__typeof__(arg0))((s8 *)arg0 + 0xC));
    func_0020EEF8(temp_a0, temp_a0, arg1);
    temp_a0_2 = ((__typeof__(arg0))((s8 *)arg0 + 0x18));
    func_0020EEF8(temp_a0_2, temp_a0_2, arg1);
    temp_s0 = ((__typeof__(arg0))((s8 *)arg0 + 0x24));
    func_0020EEF8(temp_s0, temp_s0, arg1);
}

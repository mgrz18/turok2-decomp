#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void func_0020EEF8(Vec3 *, Vec3 *, Vec3 *);
void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);

extern f32 D_800A8010;

void func_0026CB2C(void *arg0, Vec3 *arg1) {
    Vec3 sp10;
    Vec3 sp20;
    Vec3 sp30;
    f32 temp_f20;

    func_0020EEF8(&sp10, M2C_FIELD(arg0, Vec3 **, 4), M2C_FIELD(arg0, Vec3 **, 8));
    temp_f20 = D_800A8010;
    func_0020EF60(&sp10, &sp10, temp_f20);
    func_0020EEF8(&sp20, M2C_FIELD(arg0, Vec3 **, 8), M2C_FIELD(arg0, Vec3 **, 0xC));
    func_0020EF60(&sp20, &sp20, temp_f20);
    func_0020EEF8(&sp30, M2C_FIELD(arg0, Vec3 **, 0xC), M2C_FIELD(arg0, Vec3 **, 4));
    func_0020EF60(&sp30, &sp30, temp_f20);
    func_0020EEF8(arg1, &sp10, &sp20);
    func_0020EEF8(arg1, arg1, &sp30);
    func_0020EF60(arg1, arg1, 0.33333334f);
}

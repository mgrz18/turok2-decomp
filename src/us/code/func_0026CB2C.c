#include "common.h"
#include "rw/abu_protos.h"

void func_0020EEF8(Vec3 *, Vec3 *, Vec3 *);
void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);

extern f32 D_800A8010;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ Vec3 *a;
    /* 0x8 */ Vec3 *b;
    /* 0xC */ Vec3 *c;
} Triangle0026CB2C;

void func_0026CB2C(Triangle0026CB2C *arg0, Vec3 *arg1) {
    Vec3 sp10;
    Vec3 sp20;
    Vec3 sp30;
    f32 temp_f20;

    func_0020EEF8(&sp10, arg0->a, arg0->b);
    temp_f20 = D_800A8010;
    func_0020EF60(&sp10, &sp10, temp_f20);
    func_0020EEF8(&sp20, arg0->b, arg0->c);
    func_0020EF60(&sp20, &sp20, temp_f20);
    func_0020EEF8(&sp30, arg0->c, arg0->a);
    func_0020EF60(&sp30, &sp30, temp_f20);
    func_0020EEF8(arg1, &sp10, &sp20);
    func_0020EEF8(arg1, arg1, &sp30);
    func_0020EF60(arg1, arg1, 0.33333334f);
}

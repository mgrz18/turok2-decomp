/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_7.c, func_802480F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern SineTable D_800ACBF8;
extern s32 D_800B6D18;
extern void func_0020F040(Vec3 *);
extern void func_0020FF80(f32 *, f32 *);
extern void func_0020EF60(Vec3 *, Vec3 *, f32);
extern void func_00210318(f32 *, f32, f32, f32);
extern void func_0020FCC4(f32 *, f32, f32, f32, f32);
extern void func_0020C618(f32 *, f32 *, f32 *);
void func_0021B59C(s32 time, s32 flags, f32 *matrix) {
    f32 rotation[16];
    f32 inverse[16];
    Vec3 origin;
    Vec3 axis;
    u32 t;
    f32 angle;
    f32 flip;
    s32 mode;
    t = D_800B6D18;
    t += (flags & 0x38) * 50;
    t += (u32)(time & 0xFFFF) / 113;
    if (!(flags & 4)) {
        t += 40;
    }
    axis.x = 0.0f;
    axis.z = 0.0f;
    axis.y = 1.0f;
    mode = flags & 0xC3;
    if (mode == 2) {
        axis.z = 1.0f;
        axis.x = 0.0f;
        angle = D_800ACBF8.value[t % 120] - 0.5f;
        axis.y = D_800ACBF8.value[(t + 12) % 120];
    } else if (mode == 0x82) {
        axis.x = 1.0f;
        axis.z = 0.0f;
        angle = D_800ACBF8.value[t % 120] - 0.5f;
        axis.y = D_800ACBF8.value[(t + 12) % 120];
        angle *= 2.0f;
    } else if (mode == 0x40) {
        angle = D_800ACBF8.value[(t + 30) % 120] - 0.5f;
        axis.x = D_800ACBF8.value[(t + 12) % 120];
        axis.y = D_800ACBF8.value[(t + 42) % 120];
        axis.z = D_800ACBF8.value[(t + 110) % 120];
        angle *= 2.0f;
    } else if (mode == 1) {
        angle = D_800ACBF8.value[(t + 70) % 120] - 0.5f;
        angle *= 0.7f;
    } else {
        return;
    }
    func_0020F040(&axis);
    origin.x = matrix[12];
    origin.y = matrix[13];
    origin.z = matrix[14];
    func_0020FF80(inverse, matrix);
    flip = -1.0f;
    func_0020EF60(&origin, &origin, flip);
    func_00210318(inverse, origin.x, origin.y, origin.z);
    func_0020FCC4(rotation, angle, axis.x, axis.y, axis.z);
    func_0020C618(matrix, inverse, rotation);
    func_0020EF60(&origin, &origin, flip);
    func_00210318(matrix, origin.x, origin.y, origin.z);
}

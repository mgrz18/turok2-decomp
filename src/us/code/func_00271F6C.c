/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_13.c, func_80259C5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_002119FC(f32 arg0, f32 arg1);
extern f32 func_0026D210(s32);
extern float D_800A80FC;
extern float D_800A8100;
extern float D_800A8104;
extern float D_800A8108;
extern float D_800A810C;
extern float D_800B284C[];
extern struct Bytes12 *func_00271790(ContextRecAt2BC0 *, s32);
extern s16 func_0026D200(f32);
extern s32 func_002768A4(ControllerCode802591C0 *);
extern s32 func_002768B0(ControllerCode802591C0 *);
void func_00271F6C(ViewRecAtB0 *view, ControllerCode802591C0 *controller, s32 flags) {
    s16 x;
    s16 y;
    f32 fx;
    f32 fy;
    s32 turn;
    f32 diff;
    f32 position;
    s16 index;
    s16 next;
    f32 base;
    f32 result;
    struct Motion *motion;
    struct Angles *angles;
    motion = &view->motion;
    angles = &view->angles;
    motion->rate1 = motion->rate2 = 0.0f;
    if (controller->preset != -1) {
        *(struct Bytes12 *)angles = *func_00271790(view->presets, controller->preset);
    } else {
        angles->heading = func_0026D200(controller->yaw);
    }
    x = func_002768A4(controller);
    y = func_002768B0(controller);
    fy = y;
    fx = x;
    turn = (s16)(s32)(fx + ((func_002119FC(fy, fx)) < 0.0f ? -(func_002119FC(fy, fx)) : (func_002119FC(fy, fx))));
    angles->heading = func_0026D200(func_0026D210(angles->heading) + turn);
    if (flags & 0x40) {
        angles->heading = func_0026D200(func_0026D210(angles->heading) + D_800A80FC);
    }
    if (flags & 0x80) {
        angles->heading = func_0026D200(func_0026D210(angles->heading) - D_800A8100);
    }
    if (func_0026D210(angles->heading) >= D_800A8104) {
        angles->heading = func_0026D200(D_800A8104);
    }
    if (controller->flags & 2) {
        motion->heading = func_0026D210(angles->heading);
        diff = func_0026D210(angles->pitch);
        diff -= func_0026D210(angles->target);
        if (angles->steps == 0) {
            view->pitchStep = 0.0f;
        } else {
            view->pitchStep = diff / angles->steps;
        }
        view->pitch = func_0026D210(angles->pitch);
    }
    position = (func_0026D210(angles->heading) + D_800A8108) * D_800A810C;
    index = position;
    if (index != position) {
        next = index + 1;
        base = D_800B284C[index];
        result = base + (position - index) * (D_800B284C[next] - base);
    } else {
        result = D_800B284C[index];
    }
    motion->heading = result;
    angles->heading = func_0026D200(result);
}

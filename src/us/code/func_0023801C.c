#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Kind {
    s32 value;
} Kind;

typedef struct Object {
    u8 type;            /* 0x0 */
    u8 pad1[0x13];
    Kind *kind;         /* 0x14 */
    u8 pad18[0x80 - 0x18];
    s32 value;          /* 0x80 */
} Object;

typedef struct EventParams {
    s32 id;         /* 0x0 */
    s32 unk4;       /* 0x4 */
} EventParams;

extern s32 func_002755B0(s32 id, Vec3 pos, s32 arg4, s32 selected, f32 scale);
extern f32 D_800A65A4;
extern f32 D_800A65A8;
extern f32 D_800A65AC;
extern s32 D_8011ACA4;

void func_0023801C(s32 arg0, Object *obj, s32 arg2, Vec3 pos, EventParams params) {
    s32 *global = &D_8011ACA4;
    s32 selected;
    f32 scale;

    scale = D_800A65A4;
    selected = -1;
    if (global[0] != 0) {
        return;
    }
    if (obj != NULL) {
        switch (obj->type) {
            case 1:
                if (obj->kind->value == obj->type) {
                    if (global[-115] & 0x40) {
                        scale = D_800A65A8;
                    } else if (global[-115] & 0x20) {
                        scale = D_800A65AC;
                    }
                }
                selected = (s32) obj;
                break;
            case 2:
                selected = (s32) obj;
                break;
            case 0:
                selected = obj->value;
                break;
        }
    }
    if (params.id < 0x1CC) {
        if (params.id >= 0x1C2) {
            selected = -1;
        }
    }
    func_002755B0((s16) params.id, pos, 0, selected, scale);
}

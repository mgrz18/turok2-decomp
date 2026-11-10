#include "common.h"
#include "structs/vector.h"
#include "globals/game.h"

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

extern s32 Sound__PlayPitched(s32 id, CVector3 pos, s32 arg4, sptr selected, f32 scale);

void func_0023801C(s32 arg0, Object *obj, s32 arg2, CVector3 pos, EventParams params) {
    s32 *global = &gEngineApp_m_Settings.paused;
    sptr selected;
    f32 scale;

    scale = 1.0f;
    selected = -1;
    if (global[0] != 0) {
        return;
    }
    if (obj != NULL) {
        switch (obj->type) {
            case 1:
                if (obj->kind->value == obj->type) {
                    if (global[-115] & 0x40) {
                        scale = 2.5f;
                    } else if (global[-115] & 0x20) {
                        scale = 0.35f;
                    }
                }
                selected = (sptr) obj;
                break;
            case 2:
                selected = (sptr) obj;
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
    Sound__PlayPitched((s16) params.id, pos, 0, selected, scale);
}

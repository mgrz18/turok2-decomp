#include "common.h"

typedef struct ObjDef {
    /* 0x00 */ u32 type;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ f32 unk18;       /* u16 shape for type 2 */
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ f32 unk28;
} ObjDef;

typedef struct Owner {
    /* 0x000 */ u8 pad0[0xBB0];
    /* 0xBB0 */ s32 unkBB0;
} Owner;

typedef struct Obj {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x13];
    /* 0x014 */ ObjDef *def;
    /* 0x018 */ u8 pad18[0xD4 - 0x18];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ Owner *owner;
} Obj;

/* Radius of an object, by the kind of its definition. */
f32 func_002672C8(Obj *obj) {
    ObjDef *def;
    f32 r;
    f32 m;

    switch (obj->def->type) {
    case 11:
        if (obj->kind == 1 && (obj->flags & 0x300000) && obj->owner->unkBB0 != 0) {
            return 122.88f;
        }
        /* fallthrough */
    case 1:
    case 4:
        return obj->def->unk28;
    case 0:
    case 5:
    case 8:
    case 10:
    case 12:
        return obj->def->unk18;
    case 2:
        def = obj->def;
        if (*(u16 *)&def->unk18 == 2) {
            m = def->unk20;
            if (!(def->unk1C <= m)) {
                m = def->unk1C;
            }
            r = def->unk28;
            if (!(m <= r)) {
                r = m;
            }
            return r * 1.42f;
        }
        return def->unk1C;
    case 7:
        return obj->def->unk1C;
    default:
        return 0.0f;
    }
}

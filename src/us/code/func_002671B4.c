/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024D274.c, func_0024D274.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct ObjDef {
    s32 type;          /* 0x00 */
    u8 pad4[0x10];
    u32 flags14;       /* 0x14 */
    u8 pad18[4];
    f32 unk1C;         /* 0x1C */
    f32 unk20;         /* 0x20 */
    u8 pad24[0xC];
    f32 unk30;         /* 0x30 */
} ObjDef;

/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct Owner {
    u8 pad0[0xAC0];
    f32 unkAC0;        /* 0xAC0 */
    u8 padAC4[0xB24 - 0xAC4];
    f32 unkB24;        /* 0xB24 */
} Owner;

typedef struct Obj {
    u8 type;           /* 0x00 */
    u8 pad1[0x13];
    ObjDef *def;       /* 0x14 (0x18) */
    u8 pad18[0xD4 - 0x18];
    s32 flags;         /* 0xD4 (0x100) */
    u8 padD8[0x144 - 0xD8];
    s32 health;        /* 0x144 (0x174) */
    u8 pad148[0x1A8 - 0x148];
    Owner *owner;      /* 0x1A8 (0x1D8) */
} Obj;

extern f32 func_002683CC(Obj *a);
extern f32 D_800A7D9C;
extern f32 D_800A7DA0;

/* NOTE: Turok 2 differs from Rage Wars in case 11 (subtracts two owner values
 * and falls through to case 1/4) and has no cases 13/14. */
f32 func_002671B4(Obj *a) {
    ObjDef *d;
    f32 r;
    if (a->type == 1 && a->health <= 0) {
        return func_002683CC(a);
    }
    switch (a->def->type) {
    case 11:
        if (a->type == 1 && (a->flags & 0x300000)) {
            return a->def->unk30 - a->owner->unkB24 - a->owner->unkAC0;
        }
        /* fallthrough */
    case 1:
    case 4:
        return a->def->unk30;
    case 8:
        d = a->def;
        r = d->flags14 & 1; /* dead store, needed for scheduling */
        r = D_800A7D9C;
        if (!(d->flags14 & 1)) {
            r = d->unk1C;
        }
        return r;
    case 7:
        return a->def->unk20;
    case 6:
        return D_800A7DA0;
    case 0:
    case 5:
    case 10:
    case 12:
        return a->def->unk1C;
    default:
        return 0.0f;
    }
}

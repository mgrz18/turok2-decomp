/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024E2EC.c, func_0024E2EC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef union { u16 h; f32 f; } B11U32;
typedef struct ObjDef {
    s32 type;
    u8 pad4[0x10];
    u32 flags14;
    B11U32 u18;
    f32 unk1C;
    u8 pad20[0x8];
    f32 unk28;              /* 0x28 in Turok 2 (Rage Wars reads 0xEC) */
    f32 unk2C;
} ObjDef;
/* Turok 2 layout: def at 0x14 (0x18 in Rage Wars). */
typedef struct Obj {
    u8 pad0[0x14];
    ObjDef *def;            /* 0x14 */
} Obj;
extern f32 D_800A7EC4;
extern f32 D_800A7EC8;
extern f32 D_800A7ECC;
f32 func_00268304(Obj *a) {
    switch (a->def->type) {
    case 11:
        return a->def->unk28;
    case 1:
        return a->def->unk2C;
    case 2:
        if (a->def->u18.h == 0) {
            return D_800A7EC4;
        }
        return a->def->unk1C;
    case 5:
        return a->def->u18.f;
    case 8:
        if (a->def->flags14 & 1) {
            return D_800A7EC8;
        }
        return a->def->u18.f;
    default:
        return D_800A7ECC;
    }
}

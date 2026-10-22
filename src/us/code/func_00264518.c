/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00207B5C.c, func_00207B5C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct ActorInfo {
               u32 flags;
               s32 unk4;
               u8 pad8[4];
               s16 unkC;
               u8 padE[6];
               f32 unk14;
               u8 pad18[0xC];
               u32 flags24;
               u8 pad28[0x28];
               f32 unk50;
} ActorInfo;
typedef struct ActorDef {
               s32 type;
               u8 pad4[0x10];
               ActorInfo info;     /* 0x14 */
} ActorDef;
typedef struct Actor { /* Turok 2 layout */
                 u8 pad0[0x14];
                 ActorDef *def;    /* 0x14 (Rage Wars: 0x18) */
} Actor;
void func_0026E110(Actor *actor, s32 flags, Actor *actor2);
void func_00264518(Actor *actor, u32 *flags) {
    ActorInfo *info = &actor->def->info;
    func_0026E110(actor, 0x10000, actor);
    if (!(info->flags24 & 0x40)) {
        *flags &= 0xF7FFFFFF;
    }
}

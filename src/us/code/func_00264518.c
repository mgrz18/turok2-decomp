/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00207B5C.c, func_00207B5C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"

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
void func_0026E110(CGameObjectInstance *actor, s32 flags, CGameObjectInstance *actor2);
void func_00264518(CGameObjectInstance *actor, u32 *flags) {
    ActorInfo *info = &((ActorDef *)actor->header.m_pIntelligence)->info;
    func_0026E110(actor, 0x10000, actor);
    if (!(info->flags24 & 0x40)) {
        *flags &= 0xF7FFFFFF;
    }
}

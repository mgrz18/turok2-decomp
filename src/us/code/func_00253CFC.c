/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80225D10_10.CPP, func_8022ADBC_de, with Turok 2's layouts. */

#include "structs/game_object.h"


typedef struct ActorDef {
    char pad0[0x30];
    f32 height;                 /* 0x30 */
} ActorDef;


extern f32 D_800A70FC;          /* 82.944f: default height */
extern f32 D_800A7100;          /* 0.9f */

f32 func_00253CFC(CGameObjectInstance *actor) {
    ActorDef *def = ((ActorDef *)actor->header.m_pIntelligence);

    if (def != 0) {
        return def->height * D_800A7100;
    }
    return D_800A70FC;
}

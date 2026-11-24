#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"

typedef struct EventParams {
    s32 unk0;
    s32 unk4;
} EventParams;

extern void MantidQueen_SpawnMinion(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, EventParams params);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Primagen_HandleAnimEvent Primagen_HandleAnimEvent_ev
#endif
extern void Primagen_HandleAnimEvent(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, EventParams params);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define MotherBoss_AnimEvent MotherBoss_AnimEvent_ev
#endif
extern void MotherBoss_AnimEvent(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, EventParams params);

/* the actors pointer-wide (sptr: s32 on the N64, the same code) */
void ImpactEvent_LevelSpecial(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, EventParams params) {
    switch (gScene_m_LevelID) {
        case 0x1D4D:
            MantidQueen_SpawnMinion(arg0, arg1, arg2, pos, params);
            break;
        case 0x1DB2:
            Primagen_HandleAnimEvent(arg0, arg1, arg2, pos, params);
            break;
        case 0x1DB1:
            MotherBoss_AnimEvent(arg0, arg1, arg2, pos, params);
            break;
    }
}

#include "common.h"
#include "structs/vector.h"

typedef struct EventParams {
    s32 unk0;
    s32 unk4;
} EventParams;

extern s32 gScene_m_LevelID;
extern void MantidQueen_SpawnMinion(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, EventParams params);
extern void Primagen_HandleAnimEvent(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, EventParams params);
extern void MotherBoss_AnimEvent(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, EventParams params);

void ImpactEvent_LevelSpecial(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, EventParams params) {
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

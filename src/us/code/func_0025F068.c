#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x88 - 0xC];
} TargetInfo; /* size 0x88 */

void func_00246108(CGameObjectInstance *obj, CAI *ai, TargetInfo *info, s32 a, s32 b);
void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 angle, s32 unk);
void func_00284660(CPathTrack *track);
void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_0025F068(CGameObjectInstance *obj, CAI *ai) {
    TargetInfo info;
    f32 angle;

    func_00246108(obj, ai, &info, 0, 1);
    angle = 0.2617994249f; /* 15 deg */
    if ((u8) ai->m_ModeBefore == 1 || (u8) ai->m_ModeBefore == 2) {
        angle = 1.0471977f; /* 60 deg */
    }
    func_00245BAC(obj, ai, angle, info.unk8);
    if (ai->m_AnimRepeatedCount != 0 || ai->m_Action == 1) {
        func_00284660(&ai->m_PathTrack);
        func_00243414(obj, ai, 10);
    }
}

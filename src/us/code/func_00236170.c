#include "common.h"
#include "structs/game_object.h"

s32 CMtxF__FromMtxAffine(s32, s32);
s32 CMtxF__ClampEntries(s32);
s32 Swoosh_NewNode(s32, s32, s32);

s32 CMtxF__ScaleAxes(s32 *, f32, f32, f32);    /* extern */

/* A drawn instance; 0xF8 points at its render info, 0x1D0 links it in the
 * world's draw list at 0xEC14. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 unk14;
} CRenderInfo;

typedef struct CRenderInst {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 objType;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x40 - 0x10];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0x48 - 0x44];
    /* 0x048 */ u8 mtx[2][0x40];
    /* 0x0C8 */ u8 bounds[2][0x18];
    /* 0x0F8 */ CRenderInfo *info;
    /* 0x0FC */ u8 padFC[0x108 - 0xFC];
    /* 0x108 */ struct CRenderInst *link;
    /* 0x10C */ CGameObjectInstance *actor;
    /* 0x110 */ u8 pad110[0x1D0 - 0x110];
    /* 0x1D0 */ struct CRenderInst *next;
} CRenderInst;

typedef struct Node00236170 {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Node00236170 *next;
    /* 0x08 */ u8 pad8[0x1C - 0x8];
    /* 0x1C */ CRenderInst *inst;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ f32 strength;
} Node00236170;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7520];
    /* 0x7520 */ Node00236170 *head;
} World00236170;

void Swoosh_AddParticleNode(World00236170 *arg0, CRenderInst *arg1, s32 arg2) {
    u8 sp10[0x40];
    f32 temp_f0;
    f32 var_f0;
    Node00236170 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            if (var_s0->inst == arg1) {
                temp_f0 = var_s0->strength;
                if (temp_f0 > 0.0f) {
                    if (temp_f0 > 0.0f) {
                        CMtxF__FromMtxAffine((s32) sp10, arg2);
                        var_f0 = 0.01f;
                        if (arg1->info->unk14 != 0) {
                            var_f0 = 0.29257143f;
                        }
                        CMtxF__ScaleAxes(sp10, var_f0, var_f0, var_f0);
                        CMtxF__ClampEntries((s32) sp10);
                        Swoosh_NewNode((s32) arg0, (s32) var_s0, (s32) sp10);
                    }
                }
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80268150_de_5.c, func_80268A40_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CVector3__Normalize(f32 *);
extern void CVector3__Scale(void *, void *, f32);
extern void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);

void DLights_GetAtPoint(RangeNode **arg0, u32 arg1, u32 arg2, u32 arg3,
                   CVector3 *arg4, f32 *arg5) {
    CVector3 delta;
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f3;
    f32 var_f2;
    f32 var_f2_2;
    f32 var_f2_3;
    RangeNode *node;
    u16 *radius_ptr;
    node = *arg0;
    if (node != 0) {
        do {
            if (node->active != 0) {
                var_f2 = *(f32 *)&arg1 - (f32)node->x;
                radius_ptr = node->radius;
                if (var_f2 < 0.0f) {
                    var_f2 = -var_f2;
                }
                temp_f1 = *(f32 *)&arg2 - (f32)node->y;
                if (temp_f1 < 0.0f) {
                    var_f2_2 = var_f2 - temp_f1;
                } else {
                    var_f2_2 = var_f2 + temp_f1;
                }
                temp_f0 = *(f32 *)&arg3 - (f32)node->z;
                if (temp_f0 < 0.0f) {
                    var_f2_3 = var_f2_2 - temp_f0;
                } else {
                    var_f2_3 = var_f2_2 + temp_f0;
                }
                temp_f3 = (f32)*radius_ptr;
                if (var_f2_3 < temp_f3) {
                    delta.x = (f32)node->x - *(f32 *)&arg1;
                    delta.y = (f32)node->y - *(f32 *)&arg2;
                    delta.z = (f32)node->z - *(f32 *)&arg3;
                    temp_f20 = 1.0f - (var_f2_3 / temp_f3);
                    CVector3__Normalize(&delta);
                    temp_f20_2 = temp_f20 * 0.4f;
                    CVector3__Scale(&delta, &delta, temp_f20_2);
                    CVector3__Add(arg4, arg4, &delta);
                    *arg5 -= temp_f20_2;
                }
            }
            node = node->next;
        } while (node != 0);
    }
}

#include "common.h"
#include "globals/scene.h"
/* The port: the light index reaches CScene__GetLight (the N64 C left it in $a1 for it) and
 * the light comes back pointer-wide. */
#include "structs/particle.h"

void *CScene__GetLight(CScene *, s32);
void *CDLightSet__Alloc(CDLightSet *arg0, void *arg1);

void *DLight_Spawn(CDLightSet *arg0, s32 arg1) {
    return CDLightSet__Alloc(arg0, CScene__GetLight(&gScene, arg1));
}

#include "protos/list.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 owner;
    /* 0x0C */ u8 padC[0x16 - 0xC];
    /* 0x16 */ s16 unk16;
} PoolNode00228AEC;
/* Two CLists: the used list at 0x0 and the free list at 0x14. */
typedef struct {
    /* 0x00 */ PoolNode00228AEC *usedHead;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ PoolNode00228AEC *freeHead;
} Pool00228AEC;

void *CDLightSet__Alloc(CDLightSet *arg0, void *arg1) {
    CDLight *temp_s0;

    temp_s0 = arg0->m_FreeList.m_pHead;
    if (temp_s0 != NULL) {
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddHead(&arg0->m_UsedList, temp_s0);
        temp_s0->m_pROMLight = arg1;
        temp_s0->m_bEnabled = 0;
    }
    return temp_s0;
}

#include "structs/camera.h"

void CList__AddHead(CList *, void *);               /* extern */
void CList__Remove();                               /* extern */

void CDLightSet__Free(CDLightSet *arg0, CDLight *arg1) {
    arg1->m_bEnabled = 0;
    CList__Remove(&arg0->m_UsedList, arg1);
    CList__AddHead(&arg0->m_FreeList, arg1);
}

#include "m2c_macros.h"

CDLight *CDLightSet__GetFirst(CDLightSet *arg0) {
    return arg0->m_UsedList.m_pHead;
}

#include "structs/game_object.h"

CDLight *CDLightSet__GetNext(CDLightSet *arg0, CDLight *arg1) {
    return arg1->m_pNext;
}

#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00402BB0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402BB0 pos;
} Marker00402BB0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402BB0 pos;
    /* 0x10 */ s32 region;
} Obj00402BB0;

typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ Vec3_00402BB0 pos;
} Item00402BB0;

extern u8 D_800F7078[];
extern u8 D_801100F0[];
extern u8 D_800C0330[];
s32 Cinema_IsIdle(void);
s32 CScene__FindInstances(void *world, s32 a1, s32 a2, s32 a3, Marker00402BB0 **out, s32 max);
s32 CScene__FindRegionAt(void *world, Vec3_00402BB0 *pos);
void CAI__SetMode(Obj00402BB0 *obj, Item00402BB0 *item, s32 a2);
void COnScreen__SetBoss(void *a0, Obj00402BB0 *obj, void *a2);

void MantidQueen_UpdateIntro(Obj00402BB0 *obj, Item00402BB0 *item) {
    Marker00402BB0 *marker;

    if (Cinema_IsIdle()) {
        if (CScene__FindInstances(D_800F7078, 0x451, 9, 0x1D4D, &marker, 1)) {
            item->pos = marker->pos;
            obj->pos = marker->pos;
            obj->region = CScene__FindRegionAt(D_800F7078, &obj->pos);
        }
        CAI__SetMode(obj, item, 8);
        COnScreen__SetBoss(D_801100F0, obj, D_800C0330);
    }
}

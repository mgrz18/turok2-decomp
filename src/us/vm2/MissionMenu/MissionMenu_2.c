/* Second-module copy of Mission_LevelObjectivesDone (first module). The flag test is an inline helper, so &gScene is re-addressed by symbol at each call. */
#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

static __inline__ s32 IsSet(s32 flag) {
    return CScene__GetLinkGroupCount(&gScene, flag);
}

s32 Mission_LevelObjectivesDone_2(s32 level) {
    s32 open = 0;

    switch (level) {
        case 1:
            if (!IsSet(2) && !IsSet(3)) {
                open = 1;
            }
            break;
        case 2:
            if (!IsSet(4) && !IsSet(5)) {
                open = 1;
            }
            break;
        case 3:
            if (!IsSet(6) && !IsSet(7)) {
                open = 1;
            }
            break;
        case 4:
            if (!IsSet(8)) {
                open = 1;
            }
            break;
        case 5:
            if (!IsSet(9) && !IsSet(10)) {
                open = 1;
            }
            break;
        case 6:
            if (!IsSet(11) && !IsSet(12)) {
                open = 1;
            }
            break;
        default:
            open = 1;
            break;
    }
    return open;
}

#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 MenuItem_TextLength_2(void *item);

s32 MissionMenu_UpdateObjective1Count_2(MenuItem *item) {
    s32 rank = -1;

    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = sMissionMenuText_Unavailable;
            break;
        case 1:
            item->text = sMissionMenuText_ActivatedXOf3;
            rank = CScene__GetLinkGroupCount(&gScene, 2);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 2:
            item->text = sMissionMenuText_DestroyedXOf2;
            rank = CScene__GetLinkGroupCount(&gScene, 4);
            if (rank != -1) {
                rank = 2 - rank;
            }
            break;
        case 3:
            item->text = sMissionMenuText_RescuedXOf5;
            rank = CScene__GetLinkGroupCount(&gScene, 6);
            if (rank != -1) {
                rank = 5 - rank;
            }
            break;
        case 4:
            item->text = sMissionMenuText_SealedXOf3;
            rank = CScene__GetLinkGroupCount(&gScene, 8);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 5:
            item->text = sMissionMenuText_DestroyedXOf3_B44E8;
            rank = CScene__GetLinkGroupCount(&gScene, 9);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 6:
            item->text = sMissionMenuText_RecalibratedXOf4;
            rank = CScene__GetLinkGroupCount(&gScene, 11);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
    }
    if (rank != -1) {
        item->text[MenuItem_TextLength_2(item) - 6] = rank + '0';
    }
    return 0;
}

extern char D_800C1768[];

s32 MissionMenu_UpdateObjective2Count_2(MenuItem *item) {
    s32 rank = -1;

    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = sMissionMenuText_RescuedXOf4;
            rank = CScene__GetLinkGroupCount(&gScene, 3);
            if (rank != -1) {
                rank = 4 - rank;
            }
            break;
        case 2:
            item->text = sMissionMenuText_DestroyedXOf3_B441C;
            rank = CScene__GetLinkGroupCount(&gScene, 5);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 3:
            item->text = sMissionMenuText_DestroyedXOf3_B4470;
            rank = CScene__GetLinkGroupCount(&gScene, 7);
            if (rank != -1) {
                rank = 3 - rank;
            }
            break;
        case 4:
            item->text = D_800C1768;
            break;
        case 5:
            item->text = sMissionMenuText_DestroyedXOf1;
            rank = CScene__GetLinkGroupCount(&gScene, 10);
            if (rank != -1) {
                rank = 1 - rank;
            }
            break;
        case 6:
            item->text = sMissionMenuText_DestroyedXOf;
            rank = CScene__GetLinkGroupCount(&gScene, 12);
            if (rank != -1) {
                rank = 3 - rank;
                if (rank == 3) {
                    item->text = sMissionMenuText_DestroyedXOf3_B4574;
                }
            }
            break;
    }
    if (rank != -1) {
        item->text[MenuItem_TextLength_2(item) - 6] = rank + '0';
    }
    return 0;
}

void func_0045DE14(s32 arg0) {
    CScene__GetLinkGroupCount(&gScene, arg0);
}

#include "m2c_macros.h"

s32 Level_GetHub(s32);

void MissionMenu_InitStatus_2(void) {
    sMissionHub = Level_GetHub(gScene_m_LevelID);
    sMissionFailed = 0;
}

void MissionMenu_InitFailed_2(void) {
    sMissionHub = Level_GetHub(gScene_m_LevelID);
    sMissionFailed = 1;
}

typedef struct MissionTitleField {
    char pad[0x14];
    char *text;
} MissionTitleField;

s32 MissionMenu_UpdateTitle_2(MissionTitleField *field) {
    if (sMissionFailed != 0) {
        field->text = sMissionMenuText_MissionFailed;
    } else {
        field->text = sMissionMenuText_MissionStatus;
    }
    return 1;
}

s32 MissionMenu_UpdateLevelName_2(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = sMissionMenuText_PortOfAdia;
            break;
        case 2:
            item->text = sMissionMenuText_RiverOfSouls;
            break;
        case 3:
            item->text = sMissionMenuText_DeathMarshes;
            break;
        case 4:
            item->text = sMissionMenuText_LairOfTheBlindOnes;
            break;
        case 5:
            item->text = sMissionMenuText_HiveOfTheMantids;
            break;
        case 6:
            item->text = sMissionMenuText_PrimagensLightship;
            break;
    }
    return 0;
}

s32 MissionMenu_UpdateObjective1_2(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = sMissionMenuText_DistressBeacons;
            break;
        case 2:
            item->text = sMissionMenuText_SoulGates;
            break;
        case 3:
            item->text = sMissionMenuText_Prisoners;
            break;
        case 4:
            item->text = sMissionMenuText_ThermalVents;
            break;
        case 5:
            item->text = sMissionMenuText_QueenEmbryos;
            break;
        case 6:
            item->text = sMissionMenuText_PowerGenerators;
            break;
    }
    return 0;
}

s32 MissionMenu_UpdateObjective2_2(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = sMissionMenuText_Children;
            break;
        case 2:
            item->text = sMissionMenuText_SistersOfDespair;
            break;
        case 3:
            item->text = sMissionMenuText_AmmoStorageFacilities;
            break;
        case 4:
            item->text = D_800C1768;
            break;
        case 5:
            item->text = sMissionMenuText_MasterComputers;
            break;
        case 6:
            item->text = sMissionMenuText_AssemblyPlants;
            break;
    }
    return 0;
}

#include "protos/game.h"

void Music_RestoreVolume(void);
void LoopSounds_Resume(sptr);
void Controller_ResetAllRumble(void);
void Controller_SetRumbleOptionAll(s32);
void MenuItem_SelectSubmenu_2(sptr, sptr, sptr);

s32 MissionMenu_SelectOk_2(s32 arg0, s32 arg1, s32 arg2) {
    s32 ret;

    Music_RestoreVolume();
    LoopSounds_Resume(LoopSounds_Get());
    if (sMissionFailed == 0) {
        MenuItem_SelectSubmenu_2(arg0, arg1, arg2);
        ret = 1;
    } else {
        Controller_ResetAllRumble();
        Controller_SetRumbleOptionAll(1);
        ret = 1;
    }
    return ret;
}

#include "gfx.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/framebuffer.h"
#include "structs/frame.h"
#include "structs/cache.h"

/* Frees and reallocates the frame buffers, the depth buffer and the
 * display-list memory. A handle points at a struct whose first word is the
 * block address. */



extern CMemEntry *CCache__Allocate(s32 heap, s32 size, s32 flags, char *name); /* allocate */
extern CMemEntry *CCache__AllocateAt(s32 heap, sptr addr, s32 size, char *name);  /* claim a fixed block */
extern void CCache__Deallocate(sptr heap, CMemEntry *handle);                     /* free */
extern u32 GetMemSize(void);                                             /* osGetMemSize */
extern void memset(void *dst, s32 value, uptr size);                  /* memset */

void GfxPool_Alloc_2(CGfxPool *pool) {
    u32 mb = 0x100000;
    u32 i;
    s32 j;
    s32 n;
    CMemEntry *depth;
    CMemEntry *fb;
    CMemEntry *block;
    u32 size;
    u32 base;

    if (pool->gfx != 0) {
        CCache__Deallocate(0, pool->gfx);
    }
    if (pool->dynamic != 0) {
        CCache__Deallocate(0, pool->dynamic);
    }
    if (pool->depth != 0) {
        CCache__Deallocate(0, pool->depth);
    }
    if (pool->vtx != 0) {
        CCache__Deallocate(0, pool->vtx);
    }
    for (n = 0; n < 3; n++) {
        if (pool->fb[n] != 0) {
            CCache__Deallocate(0, pool->fb[n]);
        }
    }

    pool->ready = 0;
    pool->gfx = 0;
    pool->dynamic = 0;
    pool->depth = 0;
    pool->vtx = 0;
    for (j = 2; j >= 0; j--) {
        pool->fb[j] = 0;
    }

    if (GetMemSize() >= mb * 6) {
        size = 0x54600;
        base = GetMemSize() | 0x80000000;
        depth = CCache__AllocateAt(0, base - mb, size, "Z-Buffer");
        fb = CCache__AllocateAt(0, base - mb * 2, 0xFD200, "Display Buffer");
        memset((void *) (uptr)fb->data, 0, 0xFD200);
        memset((void *) (uptr)depth->data, 0, 0x54600);
        pool->depth = depth;
        pool->fb[0] = fb;
        gDepthBuffer = (uptr)pool->depth->data;
        for (i = 0; i < gTotalFramebuffers; i++) {
            gCFB[i] = (uptr)fb->data + i * size;
        }
    } else {
        size = 0x1ECC0;
        base = 0x80400000;
        block = CCache__AllocateAt(0, base - (gTotalFramebuffers + 1) * size, (gTotalFramebuffers + 1) * size, "color frame buffers + z buffer");
        pool->fb[0] = block;
        for (i = 0; i < gTotalFramebuffers; i++) {
            gCFB[i] = (uptr)block->data + i * size;
        }
        gDepthBuffer = (uptr)block->data + size * i;
    }
    for (i = 0; i < gTotalFramebuffers; i++) {
        memset((void *) gCFB[i], 0, size);
    }
    memset((void *) gDepthBuffer, 0, size);
    pool->gfx = CCache__Allocate(0, (gDisplayListSize + gLineListSize) * (2 * GFX_SIZE), 0x23, "display lists");
    pool->dynamic = CCache__Allocate(0, 0x8000, 0x23, "fifo");
    pool->vtx = CCache__Allocate(0, 0x22000, 0x23, sGfxPoolText_AudioHeap);
    gDisplayList1 = (uptr)pool->gfx->data;
    gLineList0 = gDisplayList1 + gDisplayListSize * GFX_SIZE;
    gDisplayList0 = gLineList0 + gLineListSize * GFX_SIZE;
    gLineList1 = gDisplayList0 + gDisplayListSize * GFX_SIZE;
    gRspFifoSize = 0x8000;
    gAudioHeapSize = 0x22000;
    gRspFifoBuffer = (u64 *) (uptr)pool->dynamic->data;
    gAudioHeapBase = (uptr)pool->vtx->data;
    for (i = 0; i < gTotalFramebuffers; i++)
        ;
    pool->ready = 1;
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 slots[3];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Unk0045E524;

void GfxPool_Clear_2(Unk0045E524 *arg0) {
    s32 var_v1;
    s32 *var_v0;

    var_v1 = 2;
    var_v0 = &arg0->unk8;   /* var_v0[3] walks slots[2..0] */
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = 0;
    do {
        var_v0[3] = 0;
        var_v1 -= 1;
        var_v0 -= 1;
    } while (var_v1 >= 0);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80405454_de_2.c, func_80405CDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void GfxPool_Free_2(struct RecordRecAt1C *arg0);

void GfxPool_Free_2(struct RecordRecAt1C *arg0) {
    sptr temp_a1;
    sptr temp_a1_2;
    sptr temp_a1_3;
    sptr temp_a1_4;
    sptr temp_a1_5;
    s32 var_s1;
    s32 var_v1;
    temp_a1 = arg0->b;
    if (temp_a1 != 0) {
        CCache__Deallocate(0, (CMemEntry *) temp_a1);
    }
    temp_a1_2 = arg0->c;
    if (temp_a1_2 != 0) {
        CCache__Deallocate(0, (CMemEntry *) temp_a1_2);
    }
    temp_a1_3 = arg0->d;
    if (temp_a1_3 != 0) {
        CCache__Deallocate(0, (CMemEntry *) temp_a1_3);
    }
    temp_a1_4 = arg0->e;
    if (temp_a1_4 != 0) {
        CCache__Deallocate(0, (CMemEntry *) temp_a1_4);
    }
    var_s1 = 0;
    do {
        temp_a1_5 = arg0->values[var_s1];
        if (temp_a1_5 != 0) {
            CCache__Deallocate(0, (CMemEntry *) temp_a1_5);
        }
        var_s1 += 1;
    } while (var_s1 < 3);
    var_v1 = 2;
    arg0->a = 0;
    arg0->b = 0;
    arg0->c = 0;
    arg0->d = 0;
    arg0->e = 0;
    do {
        arg0->values[var_v1] = 0;
        var_v1 -= 1;
    } while (var_v1 >= 0);
}

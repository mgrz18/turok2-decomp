/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044C7B8_de, with Turok 2's layouts. */
/* Second VM module copy of CScene__DecompressAnimatedInstances (same source); its calls go to the module's own copies, named by link address. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/player.h"

typedef struct LevelRecord {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x10];
    /* 0x20 */ u16 spawnId;
    /* 0x22 */ u16 defId;
    /* 0x24 */ s16 angle;
    /* 0x26 */ u8 pad26[2];
} LevelRecord; /* 0x28 */

typedef struct LevelNode {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ LevelRecord records[1];
} LevelNode;

typedef struct ActorBlock {
    /* 0x00 */ s32 size;
    /* 0x04 */ s32 count;
    /* 0x08 */ CGameObjectInstance objects[1];
} ActorBlock;

typedef struct Handle {
    void *data;
} Handle;

extern CVector3 D_80119928;
extern s32 D_80119948;

extern CPlayer *D_80119874;
extern s32 D_80119878;

extern s32 ProPackDecompress(Handle **handle, s32);
extern s32 CScene__GetRegion(CScene *world, s32);
extern Handle *CCache__AllocateMemoryForDecompression(s32, Handle **handle, s32 size);
extern void CUSet__Transpose(LevelNode *node, void *data, s32);
extern s32 func_00465480(CScene *world, CVector3 *pos, s32 group, void *info);
extern s32 func_004650B4(CScene *world, s32 group, s32 key, CWarpPoint *out, s32 max);
extern s32 *CScene__GetIntelligence(CScene *world, u16 id);
extern void CGameObjectInstance__ConstructFromRecord(CGameObjectInstance *obj, LevelRecord *rec, s32 mode, s32 index);
extern s32 CScene__FindRegionAt(CScene *world, CVector3 *pos);
extern void Angle_Wrap(f32 *angle);
extern void CPlayer__SetupActors_2(CPlayer *player, LevelRecord *rec, s32 mode, s32 index);
extern s32 func_00267DBC(CGameObjectInstance *obj);
extern s32 func_00267D94(CGameObjectInstance *obj);
extern f32 AI_DistSqToPointAtHeight(CGameObjectInstance *obj, CVector3 pos);
extern void CCache__DeallocAndReplaceCompressedData(s32, Handle **handle, Handle *block);

/* Builds the level's actor block from its records: places every player on the spawn points found for
   the level (one record per player start), then links each remaining actor to the nearest actor with
   the same definition. The spawn point is indexed afresh on each use and the position copied with
   memcpy (as in Rage Wars): both set loop.c's insn count and giv benefits, which decide the ROM's
   hoisting of the spawns base and of the angle constants. */
void CScene__DecompressAnimatedInstances_2(CScene *world, Handle **handle) {
    CWarpPoint spawns[4];
    CGameObjectInstance *objects;
    s32 mode;
    Handle *block;
    s32 nspawns;
    LevelNode *node;
    LevelRecord *records;
    ActorBlock *hdr;
    s32 n;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 spawnIndex;
    LevelRecord *rec;
    LevelRecord *recWalk;
    CGameObjectInstance *objWalk;
    CPlayer *player;
    CGameObjectInstance *obj;
    CGameObjectInstance *other;
    f32 angle;
    f32 best;
    f32 dist;
    s16 value;

    if (ProPackDecompress(handle, 1) != 0) {
        mode = CScene__GetRegion(world, 0);
        node = (*handle)->data;
        n = node->count;
        block = CCache__AllocateMemoryForDecompression(0, handle, n * sizeof(CGameObjectInstance) + 8);
        if (block != NULL) {
            CUSet__Transpose(node, block->data, 1);
            hdr = block->data;
            hdr->size = sizeof(CGameObjectInstance);
            hdr->count = n;
            records = node->records;
            objects = hdr->objects;
            if (world->m_bWarpSpawn == 0) {
                world->m_LevelID = func_00465480(world, &node->records[0].pos, world->m_nLevel, &world->m_WarpSpawn);
                if (world->m_LevelID != 0) {
                    world->m_bWarpSpawn = 1;
                }
            }
            spawnIndex = 0;
            nspawns = 0;
            if (world->m_bWarpSpawn != 0) {
                nspawns = func_004650B4(world, world->m_nLevel, world->m_LevelID, spawns, 4);
            }
            world->m_pActors = (CGameObjectInstance *)objects;
            world->m_nActors = n;
            i = 0;
            if (i < n) {
                objWalk = objects;
                recWalk = records;
                while (i < n) {
                    rec = recWalk;
                    obj = objWalk;
                    i++;
                    obj->header.m_pIntelligence = CScene__GetIntelligence(world, rec->defId);
                    objWalk = obj + 1;
                    recWalk = rec + 1;
                }
            }
            world->m_nFirstActor = 0;
            for (i = 0; i < n; i++) {
                rec = &records[i];
                obj = &objects[i];
                CGameObjectInstance__ConstructFromRecord(obj, rec, mode, i);
                if (*CScene__GetIntelligence(world, rec->defId) != 0xB && i != 0) {
                    continue;
                }
                world->m_nFirstActor++;
                memcpy(&D_80119928, &rec->pos, sizeof(CVector3));
                D_80119928.y += 204.79999f;
                D_80119948 = CScene__FindRegionAt(world, &rec->pos);
                for (playerIndex = 0; playerIndex < D_80119878; playerIndex++) {
                    player = &D_80119874[playerIndex];
                    if (nspawns != 0) {
                        rec->spawnId = spawns[spawnIndex].m_nID;
                        Angle_Wrap(&spawns[spawnIndex].m_RotY);
                        angle = spawns[spawnIndex].m_RotY;
                        if (!(angle > 3.1415927f)) {
                            if (!(angle < -3.1415927f)) {
                                if (!(angle > 3.1415927f)) {
                                    value = angle * 10430.06f;
                                } else {
                                    value = 0x7FFF;
                                }
                            } else {
                                value = -0x7FFF;
                            }
                        } else {
                            value = 0x7FFF;
                        }
                        rec->angle = value;
                        memcpy(&rec->pos, &spawns[spawnIndex].m_vPos, sizeof(CVector3));
                        spawnIndex++;
                        if (spawnIndex >= nspawns) {
                            spawnIndex = 0;
                        }
                    }
                    CPlayer__SetupActors_2(player, rec, mode, i);
                }
            }
            for (i = world->m_nFirstActor; i < n; i++) {
                obj = &objects[i];
                best = 3.4028235e38f;
                obj->m_AI.m_pParent = NULL;
                if (func_00267DBC(obj) == 0) {
                    continue;
                }
                for (j = world->m_nFirstActor; j < n; j++) {
                    other = &objects[j];
                    if (obj->header.m_pIntelligence->m_Type == other->header.m_pIntelligence->m_Type && func_00267D94(other) != 0 && obj != other) {
                        if (obj->m_AI.m_pParent != NULL) {
                            dist = AI_DistSqToPointAtHeight(obj, other->header.m_vPos);
                            if (dist < best) {
                                obj->m_AI.m_pParent = other;
                                best = dist;
                            }
                        } else {
                            obj->m_AI.m_pParent = other;
                            best = AI_DistSqToPointAtHeight(obj, other->header.m_vPos);
                        }
                    }
                }
            }
        }
        CCache__DeallocAndReplaceCompressedData(0, handle, block);
        world->m_Kind1Cursor = 0;
    }
}

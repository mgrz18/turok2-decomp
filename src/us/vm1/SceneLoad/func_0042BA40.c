/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044C7B8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "protos/iset.h"
#include "protos/cache.h"


typedef struct LevelNode {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ CObjectRecord records[1];
} LevelNode;

typedef struct ActorBlock {
    /* 0x00 */ s32 size;
    /* 0x04 */ s32 count;
    /* 0x08 */ CGameObjectInstance objects[1];
} ActorBlock;

typedef struct Handle {
    void *data;
} Handle;



extern s32 ProPackDecompress(Handle **handle, s32);
extern sptr CScene__GetRegion(CScene *world, s32);
extern s32 CScene__FindNearestWarpDest(CScene *world, CVector3 *pos, s32 group, void *info);
extern s32 CScene__FindWarpDests(CScene *world, s32 group, s32 key, CWarpPoint *out, s32 max);
extern s32 *CScene__GetIntelligence(CScene *world, u16 id);
extern void CGameObjectInstance__ConstructFromRecord(CGameObjectInstance *obj, CObjectRecord *rec, s32 mode, s32 index);
extern sptr CScene__FindRegionAt(CScene *world, CVector3 *pos);
extern void Angle_Wrap(f32 *angle);
extern void CPlayer__SetupActors(CPlayer *player, CObjectRecord *rec, s32 mode, s32 index);
extern s32 GameObject_IsFollower(CGameObjectInstance *obj);
extern s32 GameObject_IsLeader(CGameObjectInstance *obj);
extern f32 AI_DistSqToPointAtHeight(CGameObjectInstance *obj, CVector3 pos);

/* Builds the level's actor block from its records: places every player on the spawn points found for
   the level (one record per player start), then links each remaining actor to the nearest actor with
   the same definition. The spawn point is indexed afresh on each use and the position copied with
   memcpy (as in Rage Wars): both set loop.c's insn count and giv benefits, which decide the ROM's
   hoisting of the spawns base and of the angle constants. */
void CScene__DecompressAnimatedInstances(CScene *world, Handle **handle) {
    CWarpPoint spawns[4];
    CGameObjectInstance *objects;
    sptr mode;
    Handle *block;
    s32 nspawns;
    LevelNode *node;
    CObjectRecord *records;
    ActorBlock *hdr;
    s32 n;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 spawnIndex;
    CObjectRecord *rec;
    CObjectRecord *recWalk;
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
                world->m_LevelID = CScene__FindNearestWarpDest(world, &node->records[0].m_vPos, world->m_nLevel, &world->m_WarpSpawn);
                if (world->m_LevelID != 0) {
                    world->m_bWarpSpawn = 1;
                }
            }
            spawnIndex = 0;
            nspawns = 0;
            if (world->m_bWarpSpawn != 0) {
                nspawns = CScene__FindWarpDests(world, world->m_nLevel, world->m_LevelID, spawns, 4);
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
                    obj->header.m_pIntelligence = CScene__GetIntelligence(world, rec->m_nIntelligence);
                    objWalk = obj + 1;
                    recWalk = rec + 1;
                }
            }
            world->m_nFirstActor = 0;
            for (i = 0; i < n; i++) {
                rec = &records[i];
                obj = &objects[i];
                CGameObjectInstance__ConstructFromRecord(obj, rec, mode, i);
                if (*CScene__GetIntelligence(world, rec->m_nIntelligence) != 0xB && i != 0) {
                    continue;
                }
                world->m_nFirstActor++;
                memcpy(&gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_vPos, &rec->m_vPos, sizeof(CVector3));
                gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_vPos.y += 204.79999f;
                gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_pCurrentRegion = CScene__FindRegionAt(world, &rec->m_vPos);
                for (playerIndex = 0; playerIndex < gEngineApp_m_Game.m_PlayerList.m_nPlayers; playerIndex++) {
                    player = &gEngineApp_m_Game.m_PlayerList.m_Players[playerIndex];
                    if (nspawns != 0) {
                        rec->m_nRegion = spawns[spawnIndex].m_nID;
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
                        rec->m_Angle = value;
                        memcpy(&rec->m_vPos, &spawns[spawnIndex].m_vPos, sizeof(CVector3));
                        spawnIndex++;
                        if (spawnIndex >= nspawns) {
                            spawnIndex = 0;
                        }
                    }
                    CPlayer__SetupActors(player, rec, mode, i);
                }
            }
            for (i = world->m_nFirstActor; i < n; i++) {
                obj = &objects[i];
                best = 3.4028235e38f;
                obj->m_AI.m_pParent = NULL;
                if (GameObject_IsFollower(obj) == 0) {
                    continue;
                }
                for (j = world->m_nFirstActor; j < n; j++) {
                    other = &objects[j];
                    if (obj->header.m_pIntelligence->m_Type == other->header.m_pIntelligence->m_Type && GameObject_IsLeader(other) != 0 && obj != other) {
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

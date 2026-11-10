/* CScene, the level world.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_SCENE_H
#define PROTOS_SCENE_H

#include "common.h"

#include "structs/vector.h"
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;

void CScene__AdvanceInstances(void *);
void CScene__AdvanceListedInstances(void *);
s32 CScene__AreEventsDone(char *, s32);
void CScene__Blast(struct CScene *, void *);
void CScene__BuildActorLists(struct CScene *);
void CScene__BuildGeometryDisplayList(struct CScene *);
void CScene__CheckAutoActivation(struct CScene *, struct CGameObjectInstance *, void *, void *);
void CScene__ClearListCounts(void *);
void CScene__CollectSectorObjects(struct CScene *);
void CScene__ComputeLights(void *, s32, f32 *, u8 *);
void CScene__Construct(struct CScene *, s32);
void CScene__ConstructMap(void *, void *, s32);
void CScene__ConstructWarp(struct CScene *);
void CScene__DamageAllEnemies(struct CScene *, s32, s32);
void CScene__DecLinkGroupCount(struct CScene *, s32);
void CScene__DecompressAnimatedInstances(struct CScene *, void *);
void CScene__DecompressCollision(void *, void *);
void CScene__DecompressGridSection(struct CScene *, s32 ***);
void CScene__DecompressIntelligence(sptr, void ***);
void CScene__DecompressLinks(void *, void ***);
void CScene__DecompressParticles(sptr, void ***);
void CScene__DecompressPaths(void *, s32 ***);
void CScene__DetachChildren(void *, s32);
void CScene__DrawGeometry(struct CScene *, struct CCamera *);
void CScene__DrawParticles(u8 *, sptr);
void CScene__DrawRegisteredInstances(struct CScene *, sptr);
void CScene__DrawSortedList(void *, void *);
void CScene__DrawWarpFade(struct CScene *);
void CScene__FileInstance(struct CScene *, struct CGameObjectInstance *);
s32 CScene__FindInstances(void *, s32, s32, s32, void *, s32);
void *CScene__FindIntelligence(struct CScene *, s32, s32);
void CScene__FindLightsAt(struct CScene *, f32 *, void *, s32 *);
struct CGameObjectInstance *CScene__FindNearestActor(struct CScene *, struct CGameObjectInstance *, s32, s32, s32);
s32 CScene__FindNearestWarpDest(void *, void *, s32, void *);
s32 CScene__FindObjectType(struct CScene *, s32);
void *CScene__FindRegionAt(struct CScene *, f32 *);
s32 CScene__FindWarpDests(void *, s32, s32, void *, s32);
void CScene__FireEventsByKey(void *, s32);
void CScene__FireEventsByKeyOnce(void *, s32);
void *CScene__GetCurve(struct CScene *, s32);
void CScene__GetEventRecords(struct CScene *, s32, s32, struct CSceneRecord **, s32 *);
void CScene__GetInstanceEventRecords(struct CScene *, struct CGameObjectInstance *, struct CSceneRecord **, s32 *);
s32 CScene__GetInstanceIndex(void *, void *);
s32 CScene__GetInstanceSavedBit(void *, void *);
s32 CScene__GetInstanceSavedBitInLevel(struct CScene *, s32, s32);
sptr CScene__GetIntelligence(void *, s32);
s32 CScene__GetLevelBit(struct CScene *, s32);
s32 CScene__GetLevelBitInLevel(void *, s32, s32);
void CScene__GetLevelName(struct CScene *, s32, void *, s32);
void *CScene__GetLight(struct CScene *, s32);
void CScene__GetLightsAt(struct CScene *, void *, void *);
s32 CScene__GetLinkGroupCount(struct CScene *, s32);
sptr CScene__GetLinkGroupCountPtr(struct CScene *, s32);
s32 CScene__GetObject(struct CScene *, s32);
u16 CScene__GetObjectTypeID(struct CScene *, s32);
sptr CScene__GetRegion(void *, s32);
void CScene__GetRegionEventRecords(struct CScene *, u16 *, struct CSceneRecord **, s32 *);
u32 CScene__GetRegionIndex(struct CScene *, sptr);
void *CScene__GetRegionInfo(struct CScene *, u16 *);
void *CScene__GetRumble(struct CScene *, s32);
s32 CScene__GetSavedBit(struct CScene *, s32, s32, s32, s32);
#ifdef TARGET_PORT
void *CScene__GetSwoosh(struct CScene *, s32); /* host: returns the block (docs/port/ABI.md) */
#else
void CScene__GetSwoosh(struct CScene *);
#endif
void CScene__GetTextureSet(struct CScene *, s32, s32 *, sptr);
void CScene__InitPersistentData(void *);
s32 CScene__IsEventFired(struct CScene *, void *);
s32 CScene__IsEventFiredInLevel(struct CScene *, s32, s32);
s32 CScene__IsLevelActorKilled(struct CScene *, void *);
s32 CScene__IsLevelActorKilledInLevel(void *, s32, s32);
s32 CScene__IsLinkGroupDone(void *, s32);
sptr CScene__LoadBinaryBlock(void *, s32, s32, s32);
void CScene__LoadBitSections(void *);
void CScene__LoadBitSections2(void *);
sptr CScene__LoadGraphicSet(void *, s32, s32, s32);
void CScene__LoadLevel(struct CScene *, s32, s32);
void CScene__LoadLightSettings(void *);
void CScene__LoadVisibleGridSections(struct CScene *);
void CScene__MarkGeneratorsSaved(void *);
void CScene__QueueDelayedEvent(void *, struct LevelEvent *);
void CScene__ReleaseSectors(void *);
void CScene__RequestWarp(struct CScene *, s32, s32);
void CScene__RestoreFiredEvent(void *, void *);
void CScene__SetEventFired(struct CScene *, void *, s32);
void CScene__SetEventFiredInLevel(void *, s32, s32, s32);
void CScene__SetInstanceObjectType(struct CScene *, struct CGameObjectInstance *, s32, sptr);
void CScene__SetInstanceSavedBit(struct CScene *, void *, s32);
void CScene__SetInstanceSavedBitInLevel(void *, s32, s32, s32);
void CScene__SetLevelActorKilled(void *, void *, s32);
void CScene__SetLevelActorKilledInLevel(void *, s32, s32, s32);
void CScene__SetLevelBit(void *, s32, s32);
void CScene__SetLevelBitInLevel(void *, s32, s32, s32);
void CScene__SetSavedBit(void *, s32, s32, s32, s32, s32);
void CScene__SetupLighting(void *);
void CScene__SpawnPoolActor(struct CScene *, s32, CVector3, s32, f32);
void CScene__StartRumbles(struct CScene *, sptr, s32, CVector3, f32, f32);
void CScene__StreamAllSections(struct CScene *);
void CScene__UnloadLevel(void *);
void CScene__UnmarkGeneratorsSaved(void *);
void CScene__Update(struct CScene *);
void CScene__UpdateDelayedEvents(void *);
void CScene__UpdateWarp(struct CScene *);
void CScene__WarpToLevel(struct CScene *, s32);

#endif

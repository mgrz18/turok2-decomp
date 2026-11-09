/* Second-module twin of CScene__Construct (same source; VM callees 0xE0 lower). func_0042CB04/func_0042C9D4 need symbols (the asm has CScene__DecompressParticles + 0x50 and D_0042C9D4). */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/iset.h"

/* Level start: resets the world, opens the cartridge index and the level resource tables (levels, texture
 * sets, object types, objects, intelligence, particle effects, binaries, swooshes, lights, curves, rumbles,
 * graphic sets, cinemas, link groups), then loads five geometry headers (objects 0xDAC-0xDB0). */

extern s32 ProPack__Decompress();
extern s32 Model_DecompressCallback();
extern s32 CScene__DecompressIntelligence_2();
extern s32 CScene__DecompressParticles_2();

extern void **CCache__Request(s32, sptr, s32, s32, s32, s32 (*)(), char *, s32);
extern void CCache__Deallocate(s32, void **);
extern void CCache__Flush(s32);
extern void CCache__LoadPersistantIndex(s32, sptr *, sptr, char *);
extern void **CCache__LoadPersistantSubBlock(s32, sptr, s32, s32, CScene *, s32 (*)(), char *);
extern s32 CScene__FindObjectType(CScene *w, s32 id);
extern s32 CScene__GetObject(CScene *w, s32 index);
extern void CScene__ClearListCounts(void);
extern void CScene__LoadLevel_2(CScene *w, s32, s32);
extern void CScene__InitPersistentData_2(CScene *w, s32 file);
extern void CScene__ConstructWarp_2(CScene *w);
extern void CPlayerPool__LevelStart_2(CPlayerPool *);

/* Copies the header of object id's geometry (or clears it when the object is missing). */
static __inline__ void loadGeomHeader(CScene *w, CGeomHeader *out, s32 id) {
    s32 size;
    s32 index;
    s32 file;
    void **objIndex;
    void **geometry;

    index = CScene__FindObjectType(w, id);
    if (index == -1) {
        out->m_Data[2] = 0;
        return;
    }
    file = CScene__GetObject(w, index);
    objIndex = CCache__Request(0, file, 0x18, 0, 0, NULL, "object index", 1);
    if (objIndex != NULL) {
        geometry = CCache__Request(0, CISet__GetROMAddressAndSize((sptr)*objIndex, file, 1, &size), size, 0, 0, Model_DecompressCallback,
                                 "geometry", 1);
        if (geometry != NULL) {
            *out = *(CGeomHeader *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*geometry, 2), 0), 0), 0);
            CCache__Deallocate(0, geometry);
        }
        CCache__Deallocate(0, objIndex);
    }
}

void CScene__Construct_2(CScene *w, s32 file) {
    sptr cart;
    sptr res;
    f32 *vec;

    CScene__ClearListCounts();
    w->m_Mode = 0;
    CScene__ConstructWarp_2(w);
    w->unk_04 = 0;
    w->unk_08 = 0;
    w->m_nLevel = -1;
    w->unk_18EB8 = 0;
    w->m_SkyAngle = 0;
    CPlayerPool__LevelStart_2(&gEngineApp_m_Game.m_PlayerList);
    vec = &w->m_qSky.x;
    w->m_qSky.x = vec[1] = vec[2] = 0.0f;
    w->m_qSky.w = 1.0f;
    CCache__Flush(0);
    CCache__LoadPersistantIndex(0, &cart, file, "cartridge index");
    res = CISet__GetROMAddress(cart, file, 5);
    w->m_LevelsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pLevels, res, "levels index");
    res = CISet__GetROMAddress(cart, file, 3);
    w->m_TextureSetsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pTextureSets, res, "texture sets index");
    w->m_pObjectTypes = *CCache__LoadPersistantSubBlock(0, cart, 2, file, w, NULL, "object types");
    res = CISet__GetROMAddress(cart, file, 0);
    w->m_ObjectsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pObjects, res, "objects index");
    w->m_pIntelligence = *CCache__LoadPersistantSubBlock(0, cart, 1, file, w, CScene__DecompressIntelligence_2, "intelligence");
    w->m_pParticleEffects = (s32 *)CCache__LoadPersistantSubBlock(0, cart, 4, file, w, CScene__DecompressParticles_2, "particle effects");
    w->m_pObjectTypes2 = *CCache__LoadPersistantSubBlock(0, cart, 0xA, file, w, NULL, "object types");
    res = CISet__GetROMAddress(cart, file, 9);
    w->m_BinariesKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pBinaries, res, "binaries index");
    w->m_pSwooshes = *CCache__LoadPersistantSubBlock(0, cart, 0xB, file, w, NULL, "particle swooshes");
    w->m_pLights = *CCache__LoadPersistantSubBlock(0, cart, 0xC, file, w, NULL, "dynamic lights");
    w->m_pBitSections = gLoadSavePersistentData;
    w->unk_28 = CISet__GetROMAddressAndSize(cart, file, 6, &w->unk_30);
    w->m_pBitSections2 = gGamePersistentData;
    w->unk_2C = CISet__GetROMAddressAndSize(cart, file, 7, &w->unk_34);
    w->m_pCurves = *CCache__LoadPersistantSubBlock(0, cart, 0xD, file, w, ProPack__Decompress, "curves");
    w->m_pRumbles = *CCache__LoadPersistantSubBlock(0, cart, 0xE, file, w, ProPack__Decompress, "rumbles");
    w->m_pRumbleTypes = *CCache__LoadPersistantSubBlock(0, cart, 0xF, file, w, ProPack__Decompress, "rumble types");
    w->m_pGraphicSetTypes = *CCache__LoadPersistantSubBlock(0, cart, 0x11, file, w, NULL, "graphicset type");
    res = CISet__GetROMAddress(cart, file, 0x10);
    w->m_GraphicSetsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pGraphicSets, res, "graphicset index");
    res = CISet__GetROMAddress(cart, file, 0x12);
    w->m_CinemasKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pCinemas, res, "Cinemas index");
    w->m_pLinkGroupTypes = *CCache__LoadPersistantSubBlock(0, cart, 0x13, file, w, NULL, "link group types");
    CScene__InitPersistentData_2(w, file);
    gPersistentDataLoaded = 1;
    loadGeomHeader(w, &w->m_Geom[0], 0xDAC);
    loadGeomHeader(w, &w->m_Geom[1], 0xDAD);
    loadGeomHeader(w, &w->m_Geom[2], 0xDAE);
    loadGeomHeader(w, &w->m_Geom[3], 0xDAF);
    loadGeomHeader(w, &w->m_Geom[4], 0xDB0);
    CScene__LoadLevel_2(w, 0, 0);
}

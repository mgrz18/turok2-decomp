/* Second-module twin of CScene__Construct (same source; VM callees 0xE0 lower). func_0042CB04/func_0042C9D4 need symbols (the asm has CScene__DecompressParticles + 0x50 and D_0042C9D4). */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"

/* Level start: resets the world, opens the cartridge index and the level resource tables (levels, texture
 * sets, object types, objects, intelligence, particle effects, binaries, swooshes, lights, curves, rumbles,
 * graphic sets, cinemas, link groups), then loads five geometry headers (objects 0xDAC-0xDB0). */

extern char D_800A5D70[]; /* "object index" */
extern char D_800A5D80[]; /* "geometry" */
extern char D_800A5D8C[]; /* "cartridge index" */
extern char D_800A5D9C[]; /* "levels index" */
extern char D_800A5DAC[]; /* "texture sets index" */
extern char D_800A5DC0[]; /* "object types" */
extern char D_800A5DD0[]; /* "objects index" */
extern char D_800A5DE0[]; /* "intelligence" */
extern char D_800A5DF0[]; /* "particle effects" */
extern char D_800A5E04[]; /* "binaries index" */
extern char D_800A5E14[]; /* "particle swooshes" */
extern char D_800A5E28[]; /* "dynamic lights" */
extern char D_800A5E38[]; /* "curves" */
extern char D_800A5E40[]; /* "rumbles" */
extern char D_800A5E48[]; /* "rumble types" */
extern char D_800A5E58[]; /* "graphicset type" */
extern char D_800A5E68[]; /* "graphicset index" */
extern char D_800A5E7C[]; /* "Cinemas index" */
extern char D_800A5E8C[]; /* "link group types" */
extern s32 D_800B6D54;
extern u8 D_800E6AC0[];
extern u8 D_800EBAC0[];

extern s32 ProPack__Decompress();
extern s32 Model_DecompressCallback();
extern s32 func_0042CB04();
extern s32 func_0042C9D4();

extern void *CISet__GetBlock(void *table, s32 index);
extern s32 CISet__GetROMAddress(s32 res, s32 file, s32 index);
extern s32 CISet__GetROMAddressAndSize(s32 res, s32 file, s32 index, s32 *size);
extern void **CCache__Request(s32, s32, s32, s32, s32, s32 (*)(), char *, s32);
extern void CCache__Deallocate(s32, void **);
extern void CCache__Flush(s32);
extern void CCache__LoadPersistantIndex(s32, s32 *, s32, char *);
extern void **CCache__LoadPersistantSubBlock(s32, s32, s32, s32, CScene *, s32 (*)(), char *);
extern s32 CScene__FindObjectType(CScene *w, s32 id);
extern s32 CScene__GetObject(CScene *w, s32 index);
extern void CScene__ClearListCounts(void);
extern void func_0042AB2C(CScene *w, s32, s32);
extern void func_0042C2DC(CScene *w, s32 file);
extern void func_0042C414(CScene *w);
extern void func_0042E7B4(CPlayerPool *);

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
    objIndex = CCache__Request(0, file, 0x18, 0, 0, NULL, D_800A5D70, 1);
    if (objIndex != NULL) {
        geometry = CCache__Request(0, CISet__GetROMAddressAndSize((s32)*objIndex, file, 1, &size), size, 0, 0, Model_DecompressCallback,
                                 D_800A5D80, 1);
        if (geometry != NULL) {
            *out = *(CGeomHeader *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*geometry, 2), 0), 0), 0);
            CCache__Deallocate(0, geometry);
        }
        CCache__Deallocate(0, objIndex);
    }
}

void CScene__Construct_2(CScene *w, s32 file) {
    s32 cart;
    s32 res;
    f32 *vec;

    CScene__ClearListCounts();
    w->m_Mode = 0;
    func_0042C414(w);
    w->unk_04 = 0;
    w->unk_08 = 0;
    w->m_nLevel = -1;
    w->unk_18EB8 = 0;
    w->m_SkyAngle = 0;
    func_0042E7B4(&D_80119870.m_PlayerList);
    vec = &w->m_qSky.x;
    w->m_qSky.x = vec[1] = vec[2] = 0.0f;
    w->m_qSky.w = 1.0f;
    CCache__Flush(0);
    CCache__LoadPersistantIndex(0, &cart, file, D_800A5D8C);
    res = CISet__GetROMAddress(cart, file, 5);
    w->m_LevelsKey = res;
    CCache__LoadPersistantIndex(0, (s32 *)&w->m_pLevels, res, D_800A5D9C);
    res = CISet__GetROMAddress(cart, file, 3);
    w->m_TextureSetsKey = res;
    CCache__LoadPersistantIndex(0, (s32 *)&w->m_pTextureSets, res, D_800A5DAC);
    w->m_pObjectTypes = *CCache__LoadPersistantSubBlock(0, cart, 2, file, w, NULL, D_800A5DC0);
    res = CISet__GetROMAddress(cart, file, 0);
    w->m_ObjectsKey = res;
    CCache__LoadPersistantIndex(0, (s32 *)&w->m_pObjects, res, D_800A5DD0);
    w->m_pIntelligence = *CCache__LoadPersistantSubBlock(0, cart, 1, file, w, func_0042CB04, D_800A5DE0);
    w->m_pParticleEffects = (s32 *)CCache__LoadPersistantSubBlock(0, cart, 4, file, w, func_0042C9D4, D_800A5DF0);
    w->m_pObjectTypes2 = *CCache__LoadPersistantSubBlock(0, cart, 0xA, file, w, NULL, D_800A5DC0);
    res = CISet__GetROMAddress(cart, file, 9);
    w->m_BinariesKey = res;
    CCache__LoadPersistantIndex(0, (s32 *)&w->m_pBinaries, res, D_800A5E04);
    w->m_pSwooshes = *CCache__LoadPersistantSubBlock(0, cart, 0xB, file, w, NULL, D_800A5E14);
    w->m_pLights = *CCache__LoadPersistantSubBlock(0, cart, 0xC, file, w, NULL, D_800A5E28);
    w->m_pBitSections = D_800EBAC0;
    w->unk_28 = CISet__GetROMAddressAndSize(cart, file, 6, &w->unk_30);
    w->m_pBitSections2 = D_800E6AC0;
    w->unk_2C = CISet__GetROMAddressAndSize(cart, file, 7, &w->unk_34);
    w->m_pCurves = *CCache__LoadPersistantSubBlock(0, cart, 0xD, file, w, ProPack__Decompress, D_800A5E38);
    w->m_pRumbles = *CCache__LoadPersistantSubBlock(0, cart, 0xE, file, w, ProPack__Decompress, D_800A5E40);
    w->m_pRumbleTypes = *CCache__LoadPersistantSubBlock(0, cart, 0xF, file, w, ProPack__Decompress, D_800A5E48);
    w->m_pGraphicSetTypes = *CCache__LoadPersistantSubBlock(0, cart, 0x11, file, w, NULL, D_800A5E58);
    res = CISet__GetROMAddress(cart, file, 0x10);
    w->m_GraphicSetsKey = res;
    CCache__LoadPersistantIndex(0, (s32 *)&w->m_pGraphicSets, res, D_800A5E68);
    res = CISet__GetROMAddress(cart, file, 0x12);
    w->m_CinemasKey = res;
    CCache__LoadPersistantIndex(0, (s32 *)&w->m_pCinemas, res, D_800A5E7C);
    w->m_pLinkGroupTypes = *CCache__LoadPersistantSubBlock(0, cart, 0x13, file, w, NULL, D_800A5E8C);
    func_0042C2DC(w, file);
    D_800B6D54 = 1;
    loadGeomHeader(w, &w->m_Geom[0], 0xDAC);
    loadGeomHeader(w, &w->m_Geom[1], 0xDAD);
    loadGeomHeader(w, &w->m_Geom[2], 0xDAE);
    loadGeomHeader(w, &w->m_Geom[3], 0xDAF);
    loadGeomHeader(w, &w->m_Geom[4], 0xDB0);
    func_0042AB2C(w, 0, 0);
}

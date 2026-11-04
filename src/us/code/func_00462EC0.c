/* Second-module twin of func_00429FA0 (same source; VM callees 0xE0 lower). func_0042CB04/func_0042C9D4 need symbols (the asm has func_0042CAB4 + 0x50 and D_0042C9D4). */
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

extern s32 func_00200AE0();
extern s32 func_0020B490();
extern s32 func_0042CB04();
extern s32 func_0042C9D4();

extern void *func_002017D4(void *table, s32 index);
extern s32 func_00201848(s32 res, s32 file, s32 index);
extern s32 func_0020185C(s32 res, s32 file, s32 index, s32 *size);
extern void **func_0020367C(s32, s32, s32, s32, s32, s32 (*)(), char *, s32);
extern void func_002052D8(s32, void **);
extern void func_002053A8(s32);
extern void func_00205B94(s32, s32 *, s32, char *);
extern void **func_00205D0C(s32, s32, s32, s32, CWorld *, s32 (*)(), char *);
extern s32 func_00224180(CWorld *w, s32 id);
extern s32 func_00224F84(CWorld *w, s32 index);
extern void func_00226390(void);
extern void func_0042AB2C(CWorld *w, s32, s32);
extern void func_0042C2DC(CWorld *w, s32 file);
extern void func_0042C414(CWorld *w);
extern void func_0042E7B4(CPlayerList *);

/* Copies the header of object id's geometry (or clears it when the object is missing). */
static __inline__ void loadGeomHeader(CWorld *w, CGeomHeader *out, s32 id) {
    s32 size;
    s32 index;
    s32 file;
    void **objIndex;
    void **geometry;

    index = func_00224180(w, id);
    if (index == -1) {
        out->m_Data[2] = 0;
        return;
    }
    file = func_00224F84(w, index);
    objIndex = func_0020367C(0, file, 0x18, 0, 0, NULL, D_800A5D70, 1);
    if (objIndex != NULL) {
        geometry = func_0020367C(0, func_0020185C((s32)*objIndex, file, 1, &size), size, 0, 0, func_0020B490,
                                 D_800A5D80, 1);
        if (geometry != NULL) {
            *out = *(CGeomHeader *)func_002017D4(func_002017D4(func_002017D4(func_002017D4(*geometry, 2), 0), 0), 0);
            func_002052D8(0, geometry);
        }
        func_002052D8(0, objIndex);
    }
}

void func_00462EC0(CWorld *w, s32 file) {
    s32 cart;
    s32 res;
    f32 *vec;

    func_00226390();
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
    func_002053A8(0);
    func_00205B94(0, &cart, file, D_800A5D8C);
    res = func_00201848(cart, file, 5);
    w->m_LevelsKey = res;
    func_00205B94(0, (s32 *)&w->m_pLevels, res, D_800A5D9C);
    res = func_00201848(cart, file, 3);
    w->m_TextureSetsKey = res;
    func_00205B94(0, (s32 *)&w->m_pTextureSets, res, D_800A5DAC);
    w->m_pObjectTypes = *func_00205D0C(0, cart, 2, file, w, NULL, D_800A5DC0);
    res = func_00201848(cart, file, 0);
    w->m_ObjectsKey = res;
    func_00205B94(0, (s32 *)&w->m_pObjects, res, D_800A5DD0);
    w->m_pIntelligence = *func_00205D0C(0, cart, 1, file, w, func_0042CB04, D_800A5DE0);
    w->m_pParticleEffects = (s32 *)func_00205D0C(0, cart, 4, file, w, func_0042C9D4, D_800A5DF0);
    w->m_pObjectTypes2 = *func_00205D0C(0, cart, 0xA, file, w, NULL, D_800A5DC0);
    res = func_00201848(cart, file, 9);
    w->m_BinariesKey = res;
    func_00205B94(0, (s32 *)&w->m_pBinaries, res, D_800A5E04);
    w->m_pSwooshes = *func_00205D0C(0, cart, 0xB, file, w, NULL, D_800A5E14);
    w->m_pLights = *func_00205D0C(0, cart, 0xC, file, w, NULL, D_800A5E28);
    w->m_pBitSections = D_800EBAC0;
    w->unk_28 = func_0020185C(cart, file, 6, &w->unk_30);
    w->m_pBitSections2 = D_800E6AC0;
    w->unk_2C = func_0020185C(cart, file, 7, &w->unk_34);
    w->m_pCurves = *func_00205D0C(0, cart, 0xD, file, w, func_00200AE0, D_800A5E38);
    w->m_pRumbles = *func_00205D0C(0, cart, 0xE, file, w, func_00200AE0, D_800A5E40);
    w->m_pRumbleTypes = *func_00205D0C(0, cart, 0xF, file, w, func_00200AE0, D_800A5E48);
    w->m_pGraphicSetTypes = *func_00205D0C(0, cart, 0x11, file, w, NULL, D_800A5E58);
    res = func_00201848(cart, file, 0x10);
    w->m_GraphicSetsKey = res;
    func_00205B94(0, (s32 *)&w->m_pGraphicSets, res, D_800A5E68);
    res = func_00201848(cart, file, 0x12);
    w->m_CinemasKey = res;
    func_00205B94(0, (s32 *)&w->m_pCinemas, res, D_800A5E7C);
    w->m_pLinkGroupTypes = *func_00205D0C(0, cart, 0x13, file, w, NULL, D_800A5E8C);
    func_0042C2DC(w, file);
    D_800B6D54 = 1;
    loadGeomHeader(w, &w->m_Geom[0], 0xDAC);
    loadGeomHeader(w, &w->m_Geom[1], 0xDAD);
    loadGeomHeader(w, &w->m_Geom[2], 0xDAE);
    loadGeomHeader(w, &w->m_Geom[3], 0xDAF);
    loadGeomHeader(w, &w->m_Geom[4], 0xDB0);
    func_0042AB2C(w, 0, 0);
}

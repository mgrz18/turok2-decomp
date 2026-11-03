#include "common.h"

/* Level start: resets the world, opens the cartridge index and the level resource tables (levels, texture
 * sets, object types, objects, intelligence, particle effects, binaries, swooshes, lights, curves, rumbles,
 * graphic sets, cinemas, link groups), then loads five geometry headers (objects 0xDAC-0xDB0). */

typedef struct {
    s32 data[7];
} GeomHeader; /* 0x1C */

typedef struct {
    /* 0x00000 */ s32 unk0;
    /* 0x00004 */ s32 unk4;
    /* 0x00008 */ s32 unk8;
    /* 0x0000C */ s32 levels;
    /* 0x00010 */ s32 textureSets;
    /* 0x00014 */ s32 objects;
    /* 0x00018 */ s32 binaries;
    /* 0x0001C */ s32 graphicSets;
    /* 0x00020 */ s32 unk20;
    /* 0x00024 */ s32 cinemas;
    /* 0x00028 */ s32 unk28;
    /* 0x0002C */ s32 unk2C;
    /* 0x00030 */ s32 unk30;
    /* 0x00034 */ s32 unk34;
    /* 0x00038 */ s32 levelsRes;
    /* 0x0003C */ s32 textureSetsRes;
    /* 0x00040 */ s32 objectsRes;
    /* 0x00044 */ s32 binariesRes;
    /* 0x00048 */ s32 graphicSetsRes;
    /* 0x0004C */ u8 pad4C[0x5C - 0x4C];
    /* 0x0005C */ s32 swooshes;
    /* 0x00060 */ s32 cinemasRes;
    /* 0x00064 */ s32 intelligence;
    /* 0x00068 */ s32 unk68;
    /* 0x0006C */ void *unk6C;
    /* 0x00070 */ void *unk70;
    /* 0x00074 */ s32 objectTypes;
    /* 0x00078 */ s32 objectTypes2;
    /* 0x0007C */ s32 graphicSetTypes;
    /* 0x00080 */ s32 lights;
    /* 0x00084 */ s32 curves;
    /* 0x00088 */ s32 rumbleTypes;
    /* 0x0008C */ s32 rumbles;
    /* 0x00090 */ s32 linkGroupTypes;
    /* 0x00094 */ s32 *particleEffects;
    /* 0x00098 */ u8 pad98[0x18EA4 - 0x98];
    /* 0x18EA4 */ s32 unk18EA4;
    /* 0x18EA8 */ f32 unk18EA8[3];
    /* 0x18EB4 */ f32 unk18EB4;
    /* 0x18EB8 */ s32 unk18EB8;
    /* 0x18EBC */ u8 pad18EBC[0x18FA8 - 0x18EBC];
    /* 0x18FA8 */ s32 level;
    /* 0x18FAC */ u8 pad18FAC[0x18FEC - 0x18FAC];
    /* 0x18FEC */ GeomHeader geom[5];
} World;

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
extern u8 D_80119870[];

extern s32 func_00200AE0();
extern s32 func_0020B490();
extern s32 func_0042CBE4();
extern s32 func_0042CAB4();

extern void *func_002017D4(void *table, s32 index);
extern s32 func_00201848(s32 res, s32 file, s32 index);
extern s32 func_0020185C(s32 res, s32 file, s32 index, s32 *size);
extern void **func_0020367C(s32, s32, s32, s32, s32, s32 (*)(), char *, s32);
extern void func_002052D8(s32, void **);
extern void func_002053A8(s32);
extern void func_00205B94(s32, s32 *, s32, char *);
extern s32 *func_00205D0C(s32, s32, s32, s32, World *, s32 (*)(), char *);
extern s32 func_00224180(World *w, s32 id);
extern s32 func_00224F84(World *w, s32 index);
extern void func_00226390(void);
extern void func_0042AC0C(World *w, s32, s32);
extern void func_0042C3BC(World *w, s32 file);
extern void func_0042C4F4(World *w);
extern void func_0042E894(u8 *);

/* Copies the header of object id's geometry (or clears it when the object is missing). */
static __inline__ void loadGeomHeader(World *w, GeomHeader *out, s32 id) {
    s32 size;
    s32 index;
    s32 file;
    void **objIndex;
    void **geometry;

    index = func_00224180(w, id);
    if (index == -1) {
        out->data[2] = 0;
        return;
    }
    file = func_00224F84(w, index);
    objIndex = func_0020367C(0, file, 0x18, 0, 0, NULL, D_800A5D70, 1);
    if (objIndex != NULL) {
        geometry = func_0020367C(0, func_0020185C((s32)*objIndex, file, 1, &size), size, 0, 0, func_0020B490,
                                 D_800A5D80, 1);
        if (geometry != NULL) {
            *out = *(GeomHeader *)func_002017D4(func_002017D4(func_002017D4(func_002017D4(*geometry, 2), 0), 0), 0);
            func_002052D8(0, geometry);
        }
        func_002052D8(0, objIndex);
    }
}

void func_00429FA0(World *w, s32 file) {
    s32 cart;
    s32 res;
    f32 *vec;

    func_00226390();
    w->unk0 = 0;
    func_0042C4F4(w);
    w->unk4 = 0;
    w->unk8 = 0;
    w->level = -1;
    w->unk18EB8 = 0;
    w->unk18EA4 = 0;
    func_0042E894(D_80119870);
    vec = w->unk18EA8;
    w->unk18EA8[0] = vec[1] = vec[2] = 0.0f;
    w->unk18EB4 = 1.0f;
    func_002053A8(0);
    func_00205B94(0, &cart, file, D_800A5D8C);
    res = func_00201848(cart, file, 5);
    w->levels = res;
    func_00205B94(0, &w->levelsRes, res, D_800A5D9C);
    res = func_00201848(cart, file, 3);
    w->textureSets = res;
    func_00205B94(0, &w->textureSetsRes, res, D_800A5DAC);
    w->objectTypes = *func_00205D0C(0, cart, 2, file, w, NULL, D_800A5DC0);
    res = func_00201848(cart, file, 0);
    w->objects = res;
    func_00205B94(0, &w->objectsRes, res, D_800A5DD0);
    w->intelligence = *func_00205D0C(0, cart, 1, file, w, func_0042CBE4, D_800A5DE0);
    w->particleEffects = func_00205D0C(0, cart, 4, file, w, func_0042CAB4, D_800A5DF0);
    w->objectTypes2 = *func_00205D0C(0, cart, 0xA, file, w, NULL, D_800A5DC0);
    res = func_00201848(cart, file, 9);
    w->binaries = res;
    func_00205B94(0, &w->binariesRes, res, D_800A5E04);
    w->swooshes = *func_00205D0C(0, cart, 0xB, file, w, NULL, D_800A5E14);
    w->lights = *func_00205D0C(0, cart, 0xC, file, w, NULL, D_800A5E28);
    w->unk6C = D_800EBAC0;
    w->unk28 = func_0020185C(cart, file, 6, &w->unk30);
    w->unk70 = D_800E6AC0;
    w->unk2C = func_0020185C(cart, file, 7, &w->unk34);
    w->curves = *func_00205D0C(0, cart, 0xD, file, w, func_00200AE0, D_800A5E38);
    w->rumbles = *func_00205D0C(0, cart, 0xE, file, w, func_00200AE0, D_800A5E40);
    w->rumbleTypes = *func_00205D0C(0, cart, 0xF, file, w, func_00200AE0, D_800A5E48);
    w->graphicSetTypes = *func_00205D0C(0, cart, 0x11, file, w, NULL, D_800A5E58);
    res = func_00201848(cart, file, 0x10);
    w->graphicSets = res;
    func_00205B94(0, &w->graphicSetsRes, res, D_800A5E68);
    res = func_00201848(cart, file, 0x12);
    w->cinemas = res;
    func_00205B94(0, &w->cinemasRes, res, D_800A5E7C);
    w->linkGroupTypes = *func_00205D0C(0, cart, 0x13, file, w, NULL, D_800A5E8C);
    func_0042C3BC(w, file);
    D_800B6D54 = 1;
    loadGeomHeader(w, &w->geom[0], 0xDAC);
    loadGeomHeader(w, &w->geom[1], 0xDAD);
    loadGeomHeader(w, &w->geom[2], 0xDAE);
    loadGeomHeader(w, &w->geom[3], 0xDAF);
    loadGeomHeader(w, &w->geom[4], 0xDB0);
    func_0042AC0C(w, 0, 0);
}

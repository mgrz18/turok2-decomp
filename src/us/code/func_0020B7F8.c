/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_3.CPP, func_8026C484_de, with Turok 2's layouts. */

#include "common.h"

/* One queued draw of the batched model. */
typedef struct ModelInstance {
    /* 0x00 */ void *matrix;
    /* 0x04 */ void *lights;
    /* 0x08 */ void *textures;
    /* 0x0C */ s32 segment;
} ModelInstance;

extern void **D_800D8E28;           /* batched model */
extern s32 D_800D8E14;              /* its level of detail */
extern s32 D_800D8E10;              /* its pass */
extern s32 D_800D8E34;              /* queued instance count */
extern ModelInstance D_800DD578[32];
extern s32 D_800DD170;              /* registered model count */
extern void **D_800DD178[0x100];
extern char D_800A53D8[];
extern char D_800A53EC[];

void *func_002017D4(void *resource, s32 index);
void **func_0020367C(s32, s32 key, s32 size, s32 tag, s32, void *callback, void *name, s32 flag);
s32 func_0020185C(void *header, s32 key, s32 index, s32 *size);
void func_002051F4(s32 unused, void **resource);
void func_0020A568(void);
void func_00205110(s32 unused, void **resource);
void func_0020B4B0();

/* Loads the model resource for key through its 0x18-byte header, then queues an instance of one
 * level of detail of it with the model's own textures, as a loaded matrix (see func_0020B6A0), and
 * releases both. */
void func_0020B7F8(s32 key, void *matrix, void *lights, s32 lod, s32 pass) {
    void **header;
    void **resource;
    s32 *parts;
    s32 size;
    s32 dataKey;
    ModelInstance *inst;
    s32 n;

    header = func_0020367C(0, key, 0x18, 0, 0, 0, D_800A53D8, 1);
    if (header == 0) {
        return;
    }
    dataKey = func_0020185C(*header, key, 1, &size);
    resource = func_0020367C(0, dataKey, size, 0, 0, func_0020B4B0, D_800A53EC, 1);
    if (resource != 0) {
        parts = func_002017D4(*resource, 2);
        if (lod >= 0 && lod < *parts) {
            if (D_800D8E28 != resource || D_800D8E14 != lod || D_800D8E10 != pass || D_800D8E34 == 0x20) {
                func_0020A568();
                if (D_800DD170 == 0x100) {
                    goto done;
                }
                func_00205110(0, resource);
                D_800DD178[D_800DD170] = resource;
                D_800DD170 += 1;
            }
            D_800D8E28 = resource;
            D_800D8E14 = lod;
            D_800D8E10 = pass;
            n = D_800D8E34;
            inst = &D_800DD578[n++];
            D_800D8E34 = n;
            inst->matrix = matrix;
            inst->segment = 0;
            inst->lights = lights;
            inst->textures = 0;
        }
    done:
        func_002051F4(0, resource);
    }
    func_002051F4(0, header);
}

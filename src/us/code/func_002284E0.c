/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_3/80265370.CPP, func_802659E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "gfx.h"
struct LightData;
struct LightData {
    u16 falloff;
    u16 near;
    u8 color[4];
};
struct LightData;
typedef struct LightData LightData;
struct LightNode;
struct LightNode {
    char pad0[8];
    LightData *data;
    f32 intensity;
    s16 position[3];
    s16 active;
};
struct UnitLight;
typedef struct UnitLight UnitLight;
struct UnitLight;
struct UnitLight {
    s16 dir[3];
    u8 color[4];
    s16 falloff;
    u16 near;
    u16 range;
};
struct LightNode;
typedef struct LightNode LightNode;
extern void CMtxF__TransformPoint(void *arg0, void *arg1, void *arg2);
extern Gfx *D_800D8E24;
extern s32 D_800F1800;
extern UnitLight D_800F1400[];
extern char D_8010FF34;
extern LightNode *CDLightSet__GetFirst(void *);
extern LightNode *CDLightSet__GetNext(void *, LightNode *);
void DLights_Load(char *view) {
    LightNode *node;
    LightData *data;
    f32 position[4];
    f32 out[4];
    s32 i;
    s32 x;
    node = CDLightSet__GetFirst(&D_8010FF34);
    if (node == 0) {
        Gfx *g = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xD9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(~(0x80)) & (0xFFFFFFFFU >> (32 - (24)))) << (0))); _gbi->w1 = ((unsigned int)(0)); };
        D_800F1800 += 8;
        return;
    }
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xD9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(~(0)) & (0xFFFFFFFFU >> (32 - (24)))) << (0))); _gbi->w1 = ((unsigned int)(0x80)); };
    for (i = 0; i < 8; i++) {
        if (node != 0 && node->active != 0) {
            data = node->data;
            position[0] = node->position[0];
            position[1] = node->position[1];
            position[2] = node->position[2];
            CMtxF__TransformPoint(view + 0x20C, position, out);
            x = out[0];
            D_800F1400[i + D_800F1800].dir[0] = -x;
            D_800F1400[i + D_800F1800].dir[1] = out[1];
            D_800F1400[i + D_800F1800].dir[2] = -(s32)out[2];
            D_800F1400[i + D_800F1800].color[3] = data->color[3] * node->intensity;
            D_800F1400[i + D_800F1800].color[2] = data->color[0] * node->intensity;
            D_800F1400[i + D_800F1800].color[1] = data->color[1] * node->intensity;
            D_800F1400[i + D_800F1800].color[0] = data->color[2] * node->intensity;
            D_800F1400[i + D_800F1800].falloff = data->falloff;
            D_800F1400[i + D_800F1800].near = data->near;
            D_800F1400[i + D_800F1800].range = 0x7F80;
        } else {
            D_800F1400[i + D_800F1800].falloff = -0x8000;
        }
        if (node != 0) {
            node = CDLightSet__GetNext(&D_8010FF34, node);
        }
        {
            Gfx *g = D_800D8E24++;
            { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xDC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((16) - 1) / 8) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)((((i + 1) * 2 + 10) * 8) / 8) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)(&D_800F1400[i + D_800F1800])); };
        }
    }
    D_800F1800 += 8;
}

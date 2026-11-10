/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_3/80265370.CPP, func_802659E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/render.h"
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
#ifdef TARGET_PORT
/* sized for gendata (docs/port/DATA.md): 0x800F1400..0x800F1800, two halves of 0x20 lights
 * (DLights_SwapBuffers); splat's D_800F1402.. labels inside it made it 16 bytes on the host */
extern UnitLight gDLightBuffer[0x40];
#else
extern UnitLight gDLightBuffer[];
#endif
#ifdef TARGET_PORT
#include "structs/particle.h"
#include "structs/camera.h"
#include "globals/scene.h"
#define D_8010FF34 (*(char *) &gScene.m_DLightSet)
#else
extern char D_8010FF34;
#endif
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
        Gfx *g = gDList++;
        { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xD9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(~(0x80)) & (0xFFFFFFFFU >> (32 - (24)))) << (0))); _gbi->w1 = ((unsigned int)(0)); };
        gDLightBufferIndex += 8;
        return;
    }
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xD9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(~(0)) & (0xFFFFFFFFU >> (32 - (24)))) << (0))); _gbi->w1 = ((unsigned int)(0x80)); };
    for (i = 0; i < 8; i++) {
#ifdef TARGET_PORT
        if (node != 0 && ((CDLight *) node)->m_bEnabled != 0) {
#else
        if (node != 0 && node->active != 0) {
#endif
#ifdef TARGET_PORT
            /* the host CDLight and the camera's view matrix by member */
            data = ((CDLight *) node)->m_pROMLight;
            position[0] = ((CDLight *) node)->x;
            position[1] = ((CDLight *) node)->y;
            position[2] = ((CDLight *) node)->z;
            CMtxF__TransformPoint(&((CCamera *) view)->m_mfView, position, out);
#else
            data = node->data;
            position[0] = node->position[0];
            position[1] = node->position[1];
            position[2] = node->position[2];
            CMtxF__TransformPoint(view + 0x20C, position, out);
#endif
            x = out[0];
            gDLightBuffer[i + gDLightBufferIndex].dir[0] = -x;
            gDLightBuffer[i + gDLightBufferIndex].dir[1] = out[1];
            gDLightBuffer[i + gDLightBufferIndex].dir[2] = -(s32)out[2];
#ifdef TARGET_PORT
            gDLightBuffer[i + gDLightBufferIndex].color[3] = data->color[3] * ((CDLight *) node)->m_ColorFade;
            gDLightBuffer[i + gDLightBufferIndex].color[2] = data->color[0] * ((CDLight *) node)->m_ColorFade;
            gDLightBuffer[i + gDLightBufferIndex].color[1] = data->color[1] * ((CDLight *) node)->m_ColorFade;
            gDLightBuffer[i + gDLightBufferIndex].color[0] = data->color[2] * ((CDLight *) node)->m_ColorFade;
#else
            gDLightBuffer[i + gDLightBufferIndex].color[3] = data->color[3] * node->intensity;
            gDLightBuffer[i + gDLightBufferIndex].color[2] = data->color[0] * node->intensity;
            gDLightBuffer[i + gDLightBufferIndex].color[1] = data->color[1] * node->intensity;
            gDLightBuffer[i + gDLightBufferIndex].color[0] = data->color[2] * node->intensity;
#endif
            gDLightBuffer[i + gDLightBufferIndex].falloff = data->falloff;
            gDLightBuffer[i + gDLightBufferIndex].near = data->near;
            gDLightBuffer[i + gDLightBufferIndex].range = 0x7F80;
        } else {
            gDLightBuffer[i + gDLightBufferIndex].falloff = -0x8000;
        }
        if (node != 0) {
            node = CDLightSet__GetNext(&D_8010FF34, node);
        }
        {
            Gfx *g = gDList++;
            { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xDC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((16) - 1) / 8) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)((((i + 1) * 2 + 10) * 8) / 8) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = (GBI_ADDR(&gDLightBuffer[i + gDLightBufferIndex])); };
        }
    }
    gDLightBufferIndex += 8;
}

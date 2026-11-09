/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ACTORS/actors_volume_lighting.CPP, Actors_AccumulateVertexLighting, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "protos/iset.h"

/* A light volume (built by LightVolume_Construct). */
typedef struct LightVolume {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 inverse[16];
    /* 0x60 */ f32 mtx[16];
    /* 0xA0 */ CVector3 min;
    /* 0xAC */ CVector3 max;
    /* 0xB8 */ u8 padB8[2];
    /* 0xBA */ u8 animatedLight;
    /* 0xBB */ u8 padBB;
    /* 0xBC */ u8 animationSeed;
} LightVolume;

/* A packed model vertex. */
typedef struct PackedVertex {
    /* 0x00 */ s16 px;
    /* 0x02 */ s16 py;
    /* 0x04 */ s16 pz;
    /* 0x06 */ u16 flags;
    /* 0x08 */ s16 tc[2];
    /* 0x0C */ s8 nx;
    /* 0x0D */ s8 ny;
    /* 0x0E */ s8 nz;
    /* 0x0F */ u8 a;
} PackedVertex;

typedef struct VertexList {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ PackedVertex vertices[1];
} VertexList;

void CMtxF__FromMtxAffine(f32 *out, void *packed);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
void CMtxF__TransformPoint(f32 *mtx, CVector3 *in, CVector3 *out);
void CMtxF__RotateVector(f32 *mtx, CVector3 *in, CVector3 *out);
s32 Actors_EvaluateLightVolume(LightVolume *light, CVector3 pos, CVector3 normal, f32 *inverse, CVector3 *out, f32 *amount);

/* Adds the light of the volumes to each drawn vertex of the vertex lists: the vertex (moved by its
 * own packed matrix when multiple) is lit by each volume holding it. The last animated volume
 * found sets the vertex's animation bits: pattern 4..6 from the volume's seed (mode 1) or from
 * the vertex position (mode 2), and a level from the volume's strength there. */
void Actors_AccumulateVertexLighting(LightVolume **lights, s32 lightCount, s32 *lists, void *matrices, CVector3 *output, s32 multiple) {
    f32 matrix[16];
    CVector3 sum;
    CVector3 position;
    CVector3 normal;
    CVector3 worldPosition;
    CVector3 worldNormal;
    CVector3 contribution;
    f32 amount;
    f32 selectedAmount;
    s32 n;
    s32 i;
    s32 j;
    s32 mode;
    s32 colour;
    s32 level;
    s32 drawn;
    CVector3 *dest;
    u32 pattern;
    PackedVertex *vertex;
    PackedVertex *base;
    VertexList *list;
    LightVolume *light;

    CMtxF__FromMtxAffine(matrix, matrices);
    drawn = 0;
    for (n = 0; n < *lists; n++) {
        list = CISet__GetBlock(lists, n);
        base = list->vertices;
        for (i = 0; i < list->count; i++) {
            vertex = &base[i];
            dest = &output[drawn];
            mode = 0;
            selectedAmount = 0.0f;
            colour = 0;
            if (vertex->flags & 0x80) {
                continue;
            }
            if (multiple) {
                CMtxF__FromMtxAffine(matrix, (u8 *)matrices + ((vertex->flags >> 8) << 6));
            }
            position.x = vertex->px;
            position.y = vertex->py;
            position.z = vertex->pz;
            CMtxF__TransformPoint(matrix, &position, &worldPosition);
            normal.x = vertex->nx;
            normal.y = vertex->ny;
            normal.z = vertex->nz;
            CMtxF__RotateVector(matrix, &normal, &worldNormal);
            sum.x = 0.0f;
            sum.y = 0.0f;
            sum.z = 0.0f;
            for (j = 0; j < lightCount; j++) {
                light = lights[j];
                contribution.x = 0.0f;
                contribution.y = 0.0f;
                contribution.z = 0.0f;
                if (light->max.x > worldPosition.x &&
                    light->max.y > worldPosition.y &&
                    light->max.z > worldPosition.z &&
                    light->min.x <= worldPosition.x &&
                    light->min.y <= worldPosition.y &&
                    light->min.z <= worldPosition.z &&
                    Actors_EvaluateLightVolume(light, worldPosition, worldNormal, light->inverse, &contribution, &amount)) {
                    if (light->animatedLight) {
                        mode = light->animatedLight;
                        colour = light->animationSeed;
                        selectedAmount = amount;
                    }
                    CVector3__Add(&sum, &sum, &contribution);
                }
            }
            if (mode) {
                level = 0;
                if (selectedAmount < 0.6f) {
                    level = 1;
                }
                if (selectedAmount < 0.3f) {
                    level = 2;
                }
                if (mode == 1) {
                    vertex->flags = (vertex->flags & 0xFF8F) | (((colour % 7 + 1) << 4) & 0x70);
                }
                if (mode == 2) {
                    pattern = 0;
                    pattern += (u32)worldPosition.x / 10;
                    pattern += (u32)worldPosition.y / 10;
                    pattern += (u32)worldPosition.z / 10;
                    pattern = pattern % 7 + 1;
                    vertex->flags = (vertex->flags & 0xFF8F) | ((pattern << 4) & 0x70);
                }
                vertex->flags = (vertex->flags & 0xFFF3) | (level << 2);
                if (selectedAmount < 0.1f) {
                    vertex->flags &= 0xFF83;
                }
            }
            dest->x += sum.x;
            dest->y += sum.y;
            dest->z += sum.z;
            drawn++;
        }
    }
}

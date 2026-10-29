/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ACTORS/actors_volume_lighting.CPP, Actors_AccumulateVertexLighting, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* A light volume (built by func_00211F64). */
typedef struct LightVolume {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 inverse[16];
    /* 0x60 */ f32 mtx[16];
    /* 0xA0 */ Vec3f min;
    /* 0xAC */ Vec3f max;
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

void *func_002017D4(void *table, s32 index);
void func_0020D908(f32 *out, void *packed);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
void func_0020F85C(f32 *mtx, Vec3f *in, Vec3f *out);
void func_0020FAFC(f32 *mtx, Vec3f *in, Vec3f *out);
s32 func_00212680(LightVolume *light, Vec3f pos, Vec3f normal, f32 *inverse, Vec3f *out, f32 *amount);

/* Adds the light of the volumes to each drawn vertex of the vertex lists: the vertex (moved by its
 * own packed matrix when multiple) is lit by each volume holding it. The last animated volume
 * found sets the vertex's animation bits: pattern 4..6 from the volume's seed (mode 1) or from
 * the vertex position (mode 2), and a level from the volume's strength there. */
void func_0021285C(LightVolume **lights, s32 lightCount, s32 *lists, void *matrices, Vec3f *output, s32 multiple) {
    f32 matrix[16];
    Vec3f sum;
    Vec3f position;
    Vec3f normal;
    Vec3f worldPosition;
    Vec3f worldNormal;
    Vec3f contribution;
    f32 amount;
    f32 selectedAmount;
    s32 n;
    s32 i;
    s32 j;
    s32 mode;
    s32 colour;
    s32 level;
    s32 drawn;
    Vec3f *dest;
    u32 pattern;
    PackedVertex *vertex;
    PackedVertex *base;
    VertexList *list;
    LightVolume *light;

    func_0020D908(matrix, matrices);
    drawn = 0;
    for (n = 0; n < *lists; n++) {
        list = func_002017D4(lists, n);
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
                func_0020D908(matrix, (u8 *)matrices + ((vertex->flags >> 8) << 6));
            }
            position.x = vertex->px;
            position.y = vertex->py;
            position.z = vertex->pz;
            func_0020F85C(matrix, &position, &worldPosition);
            normal.x = vertex->nx;
            normal.y = vertex->ny;
            normal.z = vertex->nz;
            func_0020FAFC(matrix, &normal, &worldNormal);
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
                    func_00212680(light, worldPosition, worldNormal, light->inverse, &contribution, &amount)) {
                    if (light->animatedLight) {
                        mode = light->animatedLight;
                        colour = light->animationSeed;
                        selectedAmount = amount;
                    }
                    func_0020EEF8(&sum, &sum, &contribution);
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

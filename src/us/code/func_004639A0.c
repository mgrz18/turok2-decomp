/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_26.CPP, func_8044BE04_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct LightSettings;
struct LightSettings {
    char pad0[0xA];
    u8 color[3];
    u8 ambient[3];
    char pad10;
    signed char dir[3];
};
struct LightSettings;
struct SceneRecAt1B2B4;
struct SceneRecAt1B2B4 {
    char pad0[0x18E50]; /* Turok 2 offset (Rage Wars: 0x1B2B0) */
    struct LightSettings *light;
    s16 dir[3];
};
struct SceneRecAt1B2B4;
typedef struct SceneRecAt1B2B4 SceneRecAt1B2B4;
extern float D_800A5EB0;
extern u8 D_800AB0D0[];
extern u8 D_800AB0D4[];
extern u8 D_800AB0DC[];
extern u8 D_800AB0E0[];
extern f32 func_00298470(f32);
extern void func_0020EF60(f32 *, f32 *, f32);
void func_004639A0(SceneRecAt1B2B4 *scene) {
    s32 i;
    u8 c;
    f32 v[3];
    f32 length;
    u8 *light;
    for (i = 0; i < 3; i++) {
        c = scene->light->ambient[i];
        light = D_800AB0D0;
        light[i] = c;
        D_800AB0D4[i] = c;
        c = scene->light->color[i];
        light[i + 8] = c;
        D_800AB0DC[i] = c;
        D_800AB0E0[i] = scene->light->dir[i];
    }
    v[0] = scene->light->dir[0];
    v[1] = scene->light->dir[1];
    v[2] = scene->light->dir[2];
    length = func_00298470(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length != 0.0f) {
        func_0020EF60(v, v, D_800A5EB0 / length);
        scene->dir[0] = v[0];
        scene->dir[1] = v[1];
        scene->dir[2] = v[2];
    }
}

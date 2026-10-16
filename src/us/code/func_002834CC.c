/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_6.c, func_8023B694_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct Effect;
struct Effect {
    char pad00[0xC];
    s16 *definition;
    s16 mask;
    char pad12[2];
    s32 resource;
    char pad18[4];
    s32 angle0;
    s32 angle1;
    char pad24[0x1E4];
    s32 timer;
    s32 velocity;
    char pad210[4];
    f32 scale;
    f32 offset0;
    f32 offset1;
    f32 offset2;
};
struct Effect;
typedef struct Effect Effect;
extern void func_00207914(s32 *arg0, s32 arg1);
void func_002834CC(Effect *effect, s16 *definition, s16 mask, f32 scale,
                   f32 offset0, f32 offset1, f32 offset2) {
    effect->definition = definition;
    effect->mask = mask;
    func_00207914(&effect->resource, *definition);
    effect->angle0 = 0;
    effect->angle1 = 0;
    effect->timer = 0;
    effect->velocity = 0;
    effect->scale = scale;
    effect->offset0 = offset0;
    effect->offset1 = offset1;
    effect->offset2 = offset2;
}

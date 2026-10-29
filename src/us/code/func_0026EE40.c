/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80263760_8.CPP, func_80264DF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
extern f32 func_002119FC(f32 arg0, f32 arg1);
struct func_8020CA10_G1;
typedef struct func_8020CA10_G1 func_8020CA10_G1;
struct func_8020CA10_G1;
struct func_8020CA10_G1 {
    f32 unk0;
};
struct SampledCurve;
struct SampledCurve {
    int type;
    unsigned char samples[0x10];
    float length;
};
struct SampledCurve;
typedef struct RandomCurve {
    s32 type;
    f32 period;
    u8 minimum;
    u8 maximum;
} RandomCurve;
struct PlayerRecAt8;
struct PlayerRecAt8 {
    struct SampledCurve *curve;
    float time;
    float value;
};
struct PlayerRecAt8;
typedef struct PlayerRecAt8 PlayerRecAt8;
extern s32 func_0026EE40(void *arg0);
struct SampledCurve;
typedef struct SampledCurve SampledCurve;
extern func_8020CA10_G1 D_800A8074;
extern func_8020CA10_G1 D_800A8070;
extern func_8020CA10_G1 D_800A8078;
extern f32 D_8011B0A0[2];
s32 func_0026EE40(void *arg0) {
    PlayerRecAt8 *player = arg0;
    f32 temp_f1;
    f32 temp_f1_2;
    s32 temp_v1;
    SampledCurve *temp_a0;
    temp_a0 = player->curve;
    temp_v1 = temp_a0->type;
    switch (temp_v1) {
    case 0: {
        f32 divisor = D_8011B0A0[1];
        temp_f1 = player->time + (D_800A8070.unk0 / divisor);
        player->time = temp_f1;
        if (temp_a0->length <= temp_f1) {
            return 1;
        }
        goto block_7;
    }
    case 1: {
        RandomCurve *random = (void *)temp_a0;
        f32 divisor = D_8011B0A0[1];
        temp_f1_2 = player->time - (D_800A8074.unk0 / divisor);
        player->time = temp_f1_2;
        if (temp_f1_2 <= 0.0f) {
            player->time =
                temp_f1_2 + random->period;
            player->value = func_002119FC(
                (f32)random->minimum * D_800A8078.unk0,
                (f32)random->maximum * D_800A8078.unk0);
        }
        goto block_7;
    }
    default:
block_7:
        return 0;
    }
}

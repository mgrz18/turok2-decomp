/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8024E130_3.CPP, func_8024E610_de, with Turok 2's layouts. */
typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ char pad4[0x1C];
    /* 0x20 */ f32 value;
} ItemSource;

typedef struct {
    /* 0x00 */ unsigned char kind;
    /* 0x01 */ char pad1[0x13];
    /* 0x14 */ ItemSource *source;
    /* 0x18 */ char pad18[0x3C];
    /* 0x54 */ f32 value;
} ItemCode;

f32 func_0026841C(ItemCode *item) {
    ItemSource *src;

    if (item->kind == 1) {
        return item->value;
    }
    src = item->source;
    if (src->active == 0) {
        return src->value;
    }
    return 0.0f;
}

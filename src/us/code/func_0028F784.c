typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ char pad0[0x40];
    /* 0x40 */ u32 rate;
} TrackInfo;

typedef struct {
    /* 0x00 */ TrackInfo *info;
} Track;

u32 func_0028F784(Track *t, f32 scale, u32 div) {
    return (scale * ((f32) t->info->rate * 1000000.0f)) / (f32) div;
}

typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef float f32;

#define IO_WRITE(addr, data) (*(volatile u32 *)(0xA0000000 | (addr)) = (u32)(data))
#define AI_DACRATE_REG 0x04500010
#define AI_BITRATE_REG 0x04500014
#define AI_CONTROL_REG 0x04500008
#define AI_MIN_DAC_RATE 132
#define AI_MAX_BIT_RATE 16

extern s32 D_800B8900;          /* osViClock */

/* libultra osAiSetFrequency(). */
s32 func_002984A0(u32 frequency) {
    register u32 dacRate;
    register u8 bitRate;
    register f32 f;

    f = D_800B8900 / (f32)frequency + .5f;
    dacRate = f;
    if (dacRate < AI_MIN_DAC_RATE) {
        return -1;
    }
    bitRate = dacRate / 66;
    if (bitRate > AI_MAX_BIT_RATE) {
        bitRate = AI_MAX_BIT_RATE;
    }
    IO_WRITE(AI_DACRATE_REG, dacRate - 1);
    IO_WRITE(AI_BITRATE_REG, bitRate - 1);
    IO_WRITE(AI_CONTROL_REG, 1);
    return D_800B8900 / (s32)dacRate;
}

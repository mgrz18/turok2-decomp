/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/LISTS/80276620.CPP, func_802765B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * The three 1/85 weights are literals, so the object's pool is the ROM's whole 0x800A60D0-0x800A60E8
 * (three 1/85, then the three 2^31 of the u32 conversions) rather than its last 12 bytes. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
void func_00228C40(u8 hue, u8 red_arg, u8 green_arg, u8 blue_arg,
    void *red_out, void *green_out, void *blue_out) {
    s32 phase;
    s32 distance0, distance1, distance2;
    s32 phase2;
    s32 w0, w1, w2;
    s32 red, green, blue;
    s32 p0, p1, p2, p3, p4, p5, p6, p7, p8;
    s32 mixed_red, mixed_green, mixed_blue, largest;
    f32 original_max, mixed_max;
    phase = (s8)hue;
    if (phase != 0) {
        if (phase < 0) {
            distance0 = phase + 85;
            if (distance0 >= 0) w0 = (s32)((f32)(distance0 * 100) * (1.0f / 85.0f));
            else w0 = 0;
        } else {
            distance0 = 85 - phase;
            if (distance0 >= 0) w0 = (s32)((f32)(distance0 * 100) * (1.0f / 85.0f));
            else w0 = 0;
        }
        distance1 = (s8)hue;
        if (distance1 < -85) {
            distance1 += 170;
            w1 = (s32)((f32)(distance1 * 100) * (1.0f / 85.0f));
        } else {
            distance1 = -distance1;
            if (distance1 >= 0) w1 = (s32)((f32)(distance1 * 100) * (1.0f / 85.0f));
            else w1 = 0;
        }
        phase2 = (s8)hue;
        if (phase2 > 85) {
            distance2 = 170 - phase2;
            w2 = (s32)((f32)(distance2 * 100) * (1.0f / 85.0f));
        } else {
            if (phase2 >= 0) w2 = (s32)((f32)(phase2 * 100) * (1.0f / 85.0f));
            else w2 = 0;
        }
        blue = (u8)blue_arg;
        red = (u8)red_arg;
        green = (u8)green_arg;
        p0 = w1 * blue;
        p1 = w0 * red;
        p2 = w2 * green;
        p3 = w1 * red;
        p4 = w0 * green;
        p5 = w2 * blue;
        p6 = w1 * green;
        p7 = w0 * blue;
        p8 = w2 * red;
        mixed_red = p0 + p1 + p2;
        mixed_green = p3 + p4 + p5;
        mixed_blue = p6 + p7 + p8;
        original_max = (((u8)red_arg) > ((((u8)green_arg) > ((u8)blue_arg) ? ((u8)green_arg) : ((u8)blue_arg))) ? ((u8)red_arg) : ((((u8)green_arg) > ((u8)blue_arg) ? ((u8)green_arg) : ((u8)blue_arg))));
        largest = mixed_blue;
        if (largest < mixed_green) largest = mixed_green;
        if (largest < mixed_red) largest = mixed_red;
        mixed_max = (f32)largest;
        if (mixed_max != 0.0f) {
            *(u8 *)red_out = (u32)((f32)mixed_red * original_max / mixed_max);
            *(u8 *)green_out = (u32)((f32)mixed_green * original_max / mixed_max);
            *(u8 *)blue_out = (u32)((f32)mixed_blue * original_max / mixed_max);
        } else {
            *(u8 *)red_out = red_arg;
            *(u8 *)green_out = green_arg;
            *(u8 *)blue_out = blue_arg;
        }
    } else {
        *(u8 *)red_out = red_arg;
        *(u8 *)green_out = green_arg;
        *(u8 *)blue_out = blue_arg;
    }
}

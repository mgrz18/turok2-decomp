/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de_2.c, func_80272944_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef float f32;
void func_0020F908(f32 *m, f32 *in, f32 *out, u32 count) {
    f32 vx, vy, vz;
    while (count-- != 0) {
        vx = in[0];
        vy = in[1];
        vz = in[2];
        out[0] = (m[0] * vx) + (m[4] * vy) + (m[8] * vz) + m[12];
        out[1] = (m[1] * vx) + (m[5] * vy) + (m[9] * vz) + m[13];
        out[2] = (m[2] * vx) + (m[6] * vy) + (m[10] * vz) + m[14];
        in += 3;
        out += 3;
    }
}

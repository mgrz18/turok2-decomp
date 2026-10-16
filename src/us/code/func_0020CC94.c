/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_8026FC9C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern float D_800A547C;
extern float D_800A5434;
extern float D_800A5454;
extern float D_800A545C;
extern float D_800A542C;
extern float D_800A5424;
extern float D_800A5444;
extern float D_800A5464;
extern float D_800A543C;
extern float D_800A544C;
extern float D_800A5474;
extern float D_800A548C;
extern float D_800A541C;
extern float D_800A5414;
extern float D_800A5484;
extern float D_800A546C;
struct PackedMatrixWords;
struct PackedMatrixWords {
    u32 upper[8];
    u32 lower[8];
};
struct PackedMatrixWords;
typedef struct PackedMatrixWords PackedMatrixWords;
void func_0020CC94(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    f32 value;
    s32 a;
    s32 b;
    value = (src[(0) * 2]) * (D_800A5414); if (!(*(&(D_800A5414) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5414) + 1)); (a) |= 0x80000000; }; value = (src[(0) * 2 + 1]) * (D_800A541C); if (!(*(&(D_800A541C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A541C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(1) * 2]) * (D_800A5424); if (!(*(&(D_800A5424) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5424) + 1)); (a) |= 0x80000000; }; value = (src[(1) * 2 + 1]) * (D_800A542C); if (!(*(&(D_800A542C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A542C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(2) * 2]) * (D_800A5434); if (!(*(&(D_800A5434) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5434) + 1)); (a) |= 0x80000000; }; value = (src[(2) * 2 + 1]) * (D_800A543C); if (!(*(&(D_800A543C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A543C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(3) * 2]) * (D_800A5444); if (!(*(&(D_800A5444) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5444) + 1)); (a) |= 0x80000000; }; value = (src[(3) * 2 + 1]) * (D_800A544C); if (!(*(&(D_800A544C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A544C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(4) * 2]) * (D_800A5454); if (!(*(&(D_800A5454) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5454) + 1)); (a) |= 0x80000000; }; value = (src[(4) * 2 + 1]) * (D_800A545C); if (!(*(&(D_800A545C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A545C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(5) * 2]) * (D_800A5464); if (!(*(&(D_800A5464) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5464) + 1)); (a) |= 0x80000000; }; value = (src[(5) * 2 + 1]) * (D_800A546C); if (!(*(&(D_800A546C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A546C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(6) * 2]) * (D_800A5474); if (!(*(&(D_800A5474) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5474) + 1)); (a) |= 0x80000000; }; value = (src[(6) * 2 + 1]) * (D_800A547C); if (!(*(&(D_800A547C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A547C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(7) * 2]) * (D_800A5484); if (!(*(&(D_800A5484) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5484) + 1)); (a) |= 0x80000000; }; value = (src[(7) * 2 + 1]) * (D_800A548C); if (!(*(&(D_800A548C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A548C) + 1)); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
}

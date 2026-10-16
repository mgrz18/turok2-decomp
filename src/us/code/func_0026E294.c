/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_15.c, func_80278F00_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
struct Record14;
typedef struct Record14 Record14;
struct Record14;
struct Record14 {
    u32 coordinates[3];
    u16 lifetime;
    u8 flags;
    u8 kind;
    u8 state;
    u8 active;
    u8 mode;
    u8 reserved13;
};
struct Record14;
void func_0026E294(Record14 *arg0) {
    extern void func_0026DFB0(void *);
    Record14 copy;
    u8 *bytes = (u8 *)arg0;
    bytes[0xE] |= 0x20;
    if (bytes[0x11] == 1) {
        copy = *arg0;
        ((u8 *)&copy)[0x12] = 2;
        ((u8 *)&copy)[0x10] = 0;
        func_0026DFB0(&copy);
    }
}

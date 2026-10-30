/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_8.CPP, func_804053E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern int D_80130C60[];
struct NoteState;
struct NoteState {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
    char pad[2];
};
struct PakDirectory;
typedef struct PakDirectory PakDirectory;
struct NoteState;
typedef struct NoteState NoteState;
struct PakDirectory;
struct PakDirectory {
    s32 free;
    NoteState notes[16];
};
extern s32 func_0045F940(s32 index, s32 entry, s32 *out);
extern s32 func_00426D8C(s32);
extern PakDirectory *D_800C2224;
s32 func_0045F940(s32 index, s32 entry, s32 *out) {
    if (D_80130C60[index] != 3) {
        return -2;
    }
    *out = func_00426D8C(D_800C2224[index].notes[entry].file_size);
    return 0;
}

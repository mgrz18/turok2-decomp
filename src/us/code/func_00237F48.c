/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267198_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct ResourceManagerState;
struct ResourceManagerState {
    s32 active;
    s32 count;
};
struct ResourceManagerState;
typedef struct ResourceManagerState ResourceManagerState;
struct ResourceManagerState;
struct ResourceManagerState;
typedef void ( *func_80267198_de_Callback)(void *, void *, signed int, Vec3, struct ResourceManagerState);
extern void func_00237F48(void *actor, void *source, s32 event, Vec3 position, struct ResourceManagerState state);
struct CallbackEntryRecAt4V1;
struct CallbackEntryRecAt4V1 {
    func_80267198_de_Callback callback;
    s32 unused;
};
struct CallbackEntryRecAt4V1;
typedef struct CallbackEntryRecAt4V1 CallbackEntryRecAt4V1;
void func_00237F48(void *arg0, void *arg1, s32 arg2, Vec3 arg3, ResourceManagerState arg6) {
    extern CallbackEntryRecAt4V1 D_800AD080[];
    if (D_800AD080[arg2].callback != 0) {
        D_800AD080[arg2].callback(arg0, arg1, arg2, arg3, arg6);
    }
}

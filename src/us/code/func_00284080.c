/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802456A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern void func_00284468(s32 unused);
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern void func_00284080();
typedef void (*Shared_ContextCallback)(void *);
typedef void (*Shared_MenuVoidCallback)(void);
struct SharedTrackObject;
struct EntryRecAt48;
typedef struct EntryRecAt48 EntryRecAt48;
struct EntryRecAt48;
struct EntryRecAt48 {
    s32 id0;
    s32 id4;
    s32 id8;
    s32 flags;
    char pad10[0x28];
    f32 scaleA0;
    f32 aspectRatio;
    u32 settings[2];
    u32 unknown48;
};
typedef struct Shared_MenuContext Shared_MenuContext;
struct Shared_MenuContext {
    void *owner;
    void *resource;
    u32 unknown08;
    Shared_MenuVoidCallback onClose;
    Shared_MenuVoidCallback callback10;
    u32 unknown14;
    struct SharedTrackObject *target;
    f32 time;
    f32 time20;
    f32 time24;
    f32 clipStart;
    f32 clipEnd;
    f32 start;
    f32 end;
    s32 active;
    s32 state3C;
    u32 unknown40;
    s32 initialized44;
    s32 closed;
    s32 field4C;
    s32 field50;
    s32 pending;
    s32 field58;
    u32 unknown5C;
    u32 unknown60;
    u32 unknown64[1];
    EntryRecAt48 spec;
    s32 fieldB4;
    s32 mode;
    u32 unknownBC[9];
    s32 requestE0;
    Shared_ContextCallback callbackE4;
    Shared_MenuVoidCallback callbackE8;
    u32 unknownEC;
    Vec3 position;
    f32 yaw;
    f32 response;
    u32 unknown104[5];
    char messages[5][0x28];
    s32 messageCount;
};
extern int func_00284174(void);
extern void func_002844F0(void);
extern void func_002052D8(void *arg0, void *arg1);
void func_00284080(void) {
    extern void *D_800C1BB0;
    if (func_00284174() != 0) {
        s32 temp_a1;
        Shared_MenuContext *record;
        func_00284468(1);
        func_002844F0();
        temp_a1 = (s32)((Shared_MenuContext *)D_800C1BB0)->owner;
        if (temp_a1 != 0) {
            func_002052D8(0, temp_a1);
        }
        record = D_800C1BB0;
        record->owner = 0;
        record->resource = 0;
        record->active = 0;
        record->state3C = 0;
        record->unknown60 = 0;
    }
}
int func_00284174(void);

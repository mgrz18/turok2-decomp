/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_10.c, func_80245BC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
typedef void (*FuncPtr)(void);
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
struct SharedTrackObject;
typedef struct SharedTrackObjectPrefix {
    u32 unknown0[2];
    Vec3 position;
    u8 unknown14[0x3C];
} SharedTrackObjectPrefix;
typedef struct SharedTrackObject {
    SharedTrackObjectPrefix prefix;
    u8 unknown50[0x1C];
    f32 yaw;
} SharedTrackObject;
extern void func_002844F0();
struct SharedTrackObject;
extern Shared_MenuContext *D_800C1BB0;
struct SharedTrackObject;
extern void func_002052D8(void *arg0, void *arg1);
void func_002844F0(void) {
    if (D_800C1BB0->closed == 0) {
        {
            void *temp_a1 = D_800C1BB0->owner;
            if (temp_a1 != 0) {
                func_002052D8(0, temp_a1);
            }
        }
        {
            void *record = D_800C1BB0;
            FuncPtr fn = ((Shared_MenuContext *)(record))->onClose;
            ((Shared_MenuContext *)(record))->owner = 0;
            ((Shared_MenuContext *)(record))->resource = 0;
            ((Shared_MenuContext *)(record))->active = 0;
            ((Shared_MenuContext *)(record))->state3C = 0;
            ((Shared_MenuContext *)(record))->unknown60 = 0;
            if (fn != 0) {
                ((Shared_MenuContext *)(record))->onClose = 0;
                fn();
            }
        }
        D_800C1BB0->closed = 1;
    }
}

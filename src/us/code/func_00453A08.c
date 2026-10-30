/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_7.CPP, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ControllerProfile;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct IntrusiveList IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
struct IntrusiveList;
extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
struct CVector3_t;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
typedef struct OSPfs OSPfs;
struct OSPfs {
    int status;
    struct OSMesgQueue *queue;
    int channel;
    u8 id[32];
    u8 label[32];
    int version;
    int dir_size;
    int inode_table;
    int minode_table;
    int dir_table;
    int inode_start_page;
    u8 banks;
    u8 activebank;
};
struct OSPfs;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Shared_Entry;
typedef struct Shared_Entry Shared_Entry;
struct MenuRecAt20V4;
struct MenuRecAt20V4 {
    s16 state;
    char pad2[0x1E];
    s32 input;
};
struct Shared_Entry;
struct Shared_Entry {
    s32 value;
    f32 timer;
    s32 count;
    u8 text[24];
};
struct MenuRecAt20V4;
typedef struct MenuRecAt20V4 MenuRecAt20V4;
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
struct NodeCode8028469C;
struct NodeCode8028469C {
    struct NodeCode8028469C *previous;
    struct NodeCode8028469C *next;
    Clip *key;
    CVector3 position;
    f32 value0;
    f32 value1;
    u32 amplitudeHandle;
    char pad24[8];
    u32 lifetimeHandle;
    char pad30[8];
    struct NodeCode8028469C **owner;
};
struct PoolRecAt28V1 {
    IntrusiveList free;
    IntrusiveList active;
    s32 baseLevel;
};
struct OSMesgQueue;
struct OSMesgQueue;
typedef struct ControllerButtonHistory {
    s8 taps;
    s8 age;
    s8 repeat;
    s8 held;
} ControllerButtonHistory;
struct ControllerProfile;
struct ControllerProfile {
    s32 connected;
    s8 port;
    u8 padding005[3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 inputPreviousButtons;
    s32 inputButtons;
    u32 inputPressedButtons;
    s32 inputHistory[3];
    ControllerButtonHistory buttonHistory[32];
    s32 previousButtons;
    s32 buttons;
    u32 pressedButtons;
    s32 history[2];
    u32 buttonState;
    s8 stickX;
    s8 stickY;
    s8 previousStickX;
    s8 previousStickY;
    s32 pakActive;
    s32 mode;
    f32 pakAmount;
    f32 pakTimer;
    OSPfs pak;
    struct PoolRecAt28V1 effectPool;
    struct NodeCode8028469C effectNodes[3];
    s32 reset;
};
struct ControllerProfile;
typedef struct ControllerProfile ControllerProfile;
struct NodeCode8028469C;
typedef struct NodeCode8028469C NodeCode8028469C;
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
extern f32 D_800C1274[];
extern Shared_Entry D_800C1F80;
extern f32 D_800B6D28;
extern f32 D_800C1274[];
s32 func_00453A08(void *unused, MenuRecAt20V4 *menu) {
    f32 t;
    t = D_800C1F80.timer + D_800B6D28;
    D_800C1F80.timer = t;
    if (t >= D_800C1274[1]) {
        D_800C1F80.timer = t - D_800C1274[1];
    }
    if (menu->state == 2) {
        if (((s32 (*)(s32))func_00285628)(menu->input) != 0 && D_800C1F80.value > 0) {
            D_800C1F80.value--;
            D_800C1F80.timer = 0.0f;
        } else if (((s32 (*)(s32))func_00285670)(menu->input) != 0) {
            if (D_800C1F80.value < D_800C1F80.count) {
                D_800C1F80.value++;
                D_800C1F80.timer = 0.0f;
            }
        }
    }
    return 0;
}

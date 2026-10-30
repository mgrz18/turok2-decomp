/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80400000_15.CPP, func_8040170C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
void *func_002017D4(void *table, s32 index);
typedef void (*Shared_ContextCallback)(void *);
typedef void (*Shared_MenuVoidCallback)(void);
struct SharedTrackObject;
typedef struct Shared_MenuContext Shared_MenuContext;
struct Shared_MenuContext {
    void *owner;
    s32 resource;
    u32 unknown08;
    Shared_MenuVoidCallback onClose;
    Shared_MenuVoidCallback callback10;
    u32 unknown14;
    struct SharedTrackObject *target;
    f32 time;
    u32 unknown20[4];
    f32 start;
    f32 end;
    s32 active;
    s32 state3C;
    u32 unknown40;
    s32 initialized44;
    s32 closed;
    u32 unknown4C[5];
    u32 unknown60;
    u32 unknown64[4];
    u32 flags;
    u32 unknown78[26];
    s32 requestE0;
    Shared_ContextCallback callbackE4;
    Shared_MenuVoidCallback callbackE8;
    u32 unknownEC;
    CVector3 position;
    f32 yaw;
    u32 unknown100[6];
    char messages[5][0x28];
    s32 messageCount;
};
typedef struct SharedTrackObjectPrefix {
    u32 unknown0[2];
    CVector3 position;
    u8 unknown14[0x3C];
} SharedTrackObjectPrefix;
typedef struct SharedTrackObject {
    SharedTrackObjectPrefix prefix;
    u8 unknown50[0x1C];
    f32 yaw;
} SharedTrackObject;
struct SharedTrackConstants { f32 amplitude; f32 unit; };
struct KeyRecAt10;
typedef struct KeyRecAt10 KeyRecAt10;
struct KeyRecAt10;
struct KeyRecAt10 {
    char pad0[0xC];
    f32 value;
    f32 time;
};
extern f32 D_800C0990[];
extern f32 func_00449A74(f32);
extern Shared_MenuContext *D_800C1BB0;
extern f32 D_800C1BC8;
extern struct SharedTrackConstants D_800C0988;
f32 func_00449A74(f32 t) {
    s32 *track;
    KeyRecAt10 *key;
    s32 count;
    f32 u;
    f32 v;
    track = func_002017D4(D_800C1BB0->resource, 1);
    count = track[1];
    key = (KeyRecAt10 *)(track + 2);
    if (count == 0) {
        v = 0.0f;
    } else if (t <= key[0].time) {
        v = key[0].value;
    } else if (key[count - 1].time <= t) {
        v = key[count - 1].value;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        v = key[-1].value * (D_800C0988.unit - u) + key->value * u;
    }
    v *= 2.0f * D_800C1BC8 + D_800C0990[0];
    v -= D_800C1BC8;
    if (D_800C0990[0] < v) {
        v = D_800C0990[0];
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    return v;
}

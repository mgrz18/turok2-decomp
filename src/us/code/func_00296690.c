/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B5730_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct CallbackStateC;
struct CallbackStateC {
    unsigned char padding_0[8];
    void (*callback)(void *, s32, s32);
};
struct IntegerState34;
typedef struct IntegerState34 IntegerState34;
struct IntegerState34;
struct IntegerState34 {
    char pad0[0x18];
    s32 unk_18;
    char pad18[0x1C - 0x18 - sizeof(s32)];
    s32 unk_1C;
    char pad1C[0x20 - 0x1C - sizeof(s32)];
    s32 unk_20;
    char pad20[0x24 - 0x20 - sizeof(s32)];
    s32 unk_24;
    char pad24[0x30 - 0x24 - sizeof(s32)];
    s32 unk_30;
};
struct CallbackStateC;
typedef struct CallbackStateC CallbackStateC;
s32 func_00296690(void *arg0, s32 arg1, s32 arg2) {
    void *node = arg0;
    void *target;
    void (*callback)(void *, s32, s32);
    {
        s32 sw_message_value = arg1;
        sw_message_value -= (1);
        if ((unsigned int)sw_message_value > 8) {
            goto sw_message_default;
        }
        switch (sw_message_value) {
        case 0: goto sw_message_1;
        case 1: goto sw_message_default;
        case 2: goto sw_message_default;
        case 3: goto sw_message_4;
        case 4: goto sw_message_default;
        case 5: goto sw_message_default;
        case 6: goto sw_message_7;
        case 7: goto sw_message_8;
        case 8: goto sw_message_9;
        }
    }
    do {
    sw_message_1:
        *(s32 *)arg0 = arg2;
        break;
    sw_message_4:
        ((IntegerState34 *)(node))->unk_20 = 0;
        ((IntegerState34 *)(node))->unk_24 = 1;
        ((IntegerState34 *)(node))->unk_30 = 0;
        ((IntegerState34 *)(node))->unk_1C = 0;
        target = *(void **)arg0;
        if (target != 0) {
            callback = ((struct CallbackStateC *) ((char *) target))->callback;
            callback(target, 4, 0);
        }
        break;
    sw_message_9:
        ((IntegerState34 *)(node))->unk_30 = 1;
        target = *(void **)arg0;
        if (target != 0) {
            callback = ((struct CallbackStateC *) ((char *) target))->callback;
            callback(target, 9, 0);
        }
        break;
    sw_message_7:
        ((IntegerState34 *)(node))->unk_18 = arg2;
        break;
    sw_message_8:
        ((IntegerState34 *)(node))->unk_1C = 1;
        break;
    sw_message_default:
        target = *(void **)arg0;
        if (target != 0) {
            callback = ((struct CallbackStateC *) ((char *) target))->callback;
            callback(target, arg1, arg2);
        }
        break;
    } while (0);
    return 0;
}

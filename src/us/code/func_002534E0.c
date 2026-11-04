#include "common.h"
#include "structs/game.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_00285410(s32);


typedef struct {
    /* 0x00 */ u8 pad0[0xD0];
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ u8 padD4[0x140 - 0xD4];
    /* 0x140 */ u8 sub140[1];
} Attached002534E0;

/* 0x1328-byte list entries (see func_0025340C) linked through 0x1320. */
typedef struct Node002534E0 {
    /* 0x0000 */ u8 pad0[0xA50];
    /* 0x0A50 */ Attached002534E0 *attached;
    /* 0x0A54 */ u8 padA54[0x1320 - 0xA54];
    /* 0x1320 */ struct Node002534E0 *next;
} Node002534E0;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Node002534E0 *head;
} List002534E0;

void func_002534E0(List002534E0 *arg0) {
    Node002534E0 *var_a0;
    Node002534E0 *var_a0_2;
    Node002534E0 *var_s0;
    List002534E0 *var_s1;

    var_s0 = (Node002534E0 *) &D_8011ACB0;
    var_s0 = (Node002534E0 *) &D_8011ACB0;
    var_s1 = arg0;
    if ((D_8011ACB0.over == 0) && (D_8011ACB0.done == 0)) {
        func_00285410(0);
        if (D_8011ACB0.teams != 0) {
            if (D_8011ACB0.teamOver == 0) {
                D_8011ACB0.teamOver = 1;
                var_s0 = var_s1->head;
                var_a0 = var_s0;
                if (var_s0 != NULL) {
                    do {
                        func_0024E5F0((s32) var_a0, (s32) var_s0, 0x12);
                        var_s0 = var_s0->next;
                        var_a0 = var_s0;
                    } while (var_s0 != NULL);
                }
            }
        } else {
            D_8011ACB0.over = 1;
            var_s0 = var_s1->head;
            var_a0_2 = var_s0;
            if (var_s0 != NULL) {
                do {
                    func_0024E5F0((s32) var_a0_2, (s32) var_s0, 0x12);
                    var_s0 = var_s0->next;
                    var_a0_2 = var_s0;
                } while (var_s0 != NULL);
            }
        }
    }
}

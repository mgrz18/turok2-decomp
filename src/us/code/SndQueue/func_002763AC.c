#include "common.h"
#include "structs/sound.h"

void SndVoice_Construct(SndVoice *voice, CAudioWorld *owner, s32 index);

void SndQueue_InitNode(SndNode *node, CAudioWorld *owner) {
    node->link.next = &node->link;
    node->link.prev = &node->link;
    SndVoice_Construct(&node->body, owner, -1);
}

#ifndef CARD_H
#define CARD_H

// TODO cards of both the player and opponent will be stored in cardS.c
// board_and_pieces.c will poll cards.c for effects and stuff

#include "render.h"

typedef struct {

    Texture texture;

} CardType;

typedef struct {

    CardType *type;
    Transform transform;

} CardInstance;

#endif
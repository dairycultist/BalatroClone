#ifndef CARD_H
#define CARD_H

#include "render.h"

typedef struct {

    Texture texture;

} CardType;

typedef struct {

    CardType type;
    Transform transform;

} CardInstance;

#endif
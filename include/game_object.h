

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include <queue>
#include <vector>

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.

enum {
    TYPE_LITE, TYPE_ROOK, TYPE_KNIGHT, TYPE_BISHOP, TYPE_KING, TYPE_QUEEN, TYPE_PLAYER, TYPE_COINS
};

struct inst{
    int target_x, target_y;
    char dir; 
    /**
        Top right: E
        Top left: Q
        Bottom right: C
        Bottom left: Z
    **/
    std::vector<std::pair<int, int>> path;
};

class GameObject
{
public:
    // object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    unsigned int Type;
    bool        IsSolid;
    bool        Destroyed;
    int         chessColor;
    // movement queue
    std::queue<inst> q;
    // render state
    Texture2D   Sprite;	
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), int type = TYPE_LITE);
    // draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif


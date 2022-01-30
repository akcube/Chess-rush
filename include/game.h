/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};


const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

class Game
{
public:
    // game state
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    unsigned int            tilesize;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

const std::string texturesList[][2] = {
    { "textures/black_queen.png", "bqueen" },
    { "textures/black_rook.png", "brook" },
    { "textures/black_bishop.png", "bbishop" },
    { "textures/black_knight.png", "bknight" },
    { "textures/black_king.png", "bking" },
    { "textures/black_pawn.png", "bpawn" },
    { "textures/white_queen.png", "wqueen" },
    { "textures/white_rook.png", "wrook" },
    { "textures/white_bishop.png", "wbishop" },
    { "textures/white_knight.png", "wknight" },
    { "textures/white_king.png", "wking" },
    { "textures/white_pawn.png", "wpawn" },
    { "textures/brown_dark_square.png", "brown_dark_square" },
    { "textures/brown_light_square.png", "brown_light_square" },
    { "textures/gray_dark_square.png", "gray_dark_square" },
    { "textures/gray_light_square.png", "gray_light_square" },
    { "textures/wall.png", "wall" },
    { "textures/penguin.png", "penguin"},
    { "textures/gem.png", "gem"},
    {}
};

#endif
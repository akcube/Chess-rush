#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
private:
    void loadTextures();

public:
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
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
};

#endif
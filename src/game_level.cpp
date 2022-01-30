/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>


void GameLevel::Load(float tilesize, unsigned int levelWidth, unsigned int levelHeight, int enemyCount)
{
    this->Grid.clear();
    this->Pieces.clear();
    this->init(tilesize, levelWidth, levelHeight, enemyCount);
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (auto &row : this->Grid)
        for(auto &tile : row)
            if (!tile.Destroyed) tile.Draw(renderer);
    for (auto &piece : this->Pieces)
            if (!piece.Destroyed) piece.Draw(renderer);
    Player.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    // for (GameObject &tile : this->Bricks)
    //     if (!tile.IsSolid && !tile.Destroyed)
    //         return false;
    return false;
}

void GameLevel::init(float tilesize, unsigned int levelWidth, unsigned int levelHeight, int enemyCount)
{
    score = 0;
    wonGame = false;
    float unit_width = tilesize, unit_height = tilesize; 
    // calculate dimensions
    unsigned int height = levelHeight/static_cast<float>(unit_height);
    unsigned int width = levelWidth/static_cast<float>(unit_width); // note we can index vector at [0] since this function is only called if height > 0
    this->height = height;
    this->width = width;
    int p = 0;
    this->filled.assign(height, std::vector<bool>(width, false));
    // Draw checkerboard background
    for (unsigned int y = 0; y < height; ++y, p^=1)
    {
        std::vector<GameObject> row;
        for (unsigned int x = 0; x < width; ++x, p^=1)
        {
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width * 0.9, unit_height * 0.9);

            int MAGIC = rand()%10;

            if(x == 0 or y == 0 or x == width - 1 or y == height - 1 or MAGIC == 5){
                GameObject obj(pos, size, ResourceManager::GetTexture("wall"));
                obj.IsSolid = true; this->filled[y][x] = true;
                row.push_back(obj);
                continue;
            }
            if(p){
                GameObject obj(pos, size, ResourceManager::GetTexture("gray_dark_square"));
                obj.IsSolid = false;
                row.push_back(obj);
            }
            else{
                GameObject obj(pos, size, ResourceManager::GetTexture("gray_light_square"));
                obj.IsSolid = false;
                row.push_back(obj);
            }
        }
        this->Grid.push_back(row);
    }
    glm::vec2 posxx(unit_width * (width-1), unit_height * (height-2));
    glm::vec2 sizexx(unit_width, unit_height);
    Exit = GameObject(posxx, sizexx, ResourceManager::GetTexture("gray_light_square"));
    Exit.IsSolid = false;
    this->Grid[height-2][width-1] = Exit;
    // Draw person
    glm::vec2 posp(unit_width * 1, unit_height * 1);
    glm::vec2 sizep(unit_width * 0.7 , unit_height * 0.7);
    Player = GameObject(posp, sizep, ResourceManager::GetTexture("penguin"));

    int gemCt = rand()%7 + 10;

    for(int i=0; i<gemCt; i++){
        int x = rand()%(width-1)+1;
        int y = rand()%(height-1)+1;
        glm::vec2 pos(unit_width * x, unit_height * y);
        glm::vec2 size(unit_width * 0.9, unit_height * 0.9);
        if(filled[y][x]){
            i--;
            continue;
        }
        GameObject obj = GameObject(pos, size, ResourceManager::GetTexture("gem"));
        obj.Type = TYPE_COINS;
        obj.IsSolid = false;
        Pieces.push_back(obj);
    }

    // Draw pieces
    for(int i=0; i<enemyCount; i++){
        int x = rand()%(width-1)+1;
        int y = rand()%(height-1)+1;

        int maxdist = abs(x-1) + abs(y-1);

        if(filled[y][x] or maxdist<8){
            i--;
            continue;
        }
        filled[y][x] = true;
        glm::vec2 pos(unit_width * x, unit_height * y);
        glm::vec2 size(unit_width * 0.8, unit_height * 0.8);
        int randtype = rand()%5;
        GameObject obj;
        int colorr = rand()%2;
        if(randtype == 0){
            if(colorr)
                obj =  GameObject(pos, size, ResourceManager::GetTexture("bqueen"));
            else
                obj =  GameObject(pos, size, ResourceManager::GetTexture("wqueen"));
            obj.Type = TYPE_QUEEN;
            obj.IsSolid = true;
        }
        else if(randtype == 1){
            if(colorr)
                obj =  GameObject(pos, size, ResourceManager::GetTexture("bking"));
            else
                obj =  GameObject(pos, size, ResourceManager::GetTexture("wking"));
            obj.Type = TYPE_KING;
            obj.IsSolid = true;
        }
        else if(randtype == 2){
            if(colorr)
                obj =  GameObject(pos, size, ResourceManager::GetTexture("brook"));
            else
                obj =  GameObject(pos, size, ResourceManager::GetTexture("wrook"));
            obj.Type = TYPE_ROOK;
            obj.IsSolid = true;
        }
        else if(randtype == 3){
            if(colorr)
                obj =  GameObject(pos, size, ResourceManager::GetTexture("bknight"));
            else
                obj =  GameObject(pos, size, ResourceManager::GetTexture("wknight"));
            obj.Type = TYPE_KNIGHT;
            obj.IsSolid = true;
        }
        else if(randtype == 4){
            if(colorr)
                obj =  GameObject(pos, size, ResourceManager::GetTexture("bbishop"));
            else
                obj =  GameObject(pos, size, ResourceManager::GetTexture("wbishop"));
            obj.Type = TYPE_BISHOP;
            obj.IsSolid = true;
        }  
        obj.chessColor = colorr;
        Pieces.push_back(obj);
    }
}


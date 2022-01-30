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


void GameLevel::Load(float tilesize, unsigned int levelWidth, unsigned int levelHeight)
{
    this->Grid.clear();
    this->Pieces.clear();
    this->init(tilesize, levelWidth, levelHeight);
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (auto &row : this->Grid)
        for(auto &tile : row)
            if (!tile.Destroyed) tile.Draw(renderer);
    for (auto &piece : this->Pieces)
            if (!piece.Destroyed) piece.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    // for (GameObject &tile : this->Bricks)
    //     if (!tile.IsSolid && !tile.Destroyed)
    //         return false;
    return false;
}

void GameLevel::init(float tilesize, unsigned int levelWidth, unsigned int levelHeight)
{
    float unit_width = tilesize, unit_height = tilesize; 
    // calculate dimensions
    unsigned int height = 2 * levelHeight/static_cast<float>(unit_height);
    unsigned int width = levelWidth/static_cast<float>(unit_width); // note we can index vector at [0] since this function is only called if height > 0
    height++, width++;
    int p = 0;
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y)
    {
        std::vector<GameObject> row;
        for (unsigned int x = 0; x < width; ++x, p^=1)
        {
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);
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
}


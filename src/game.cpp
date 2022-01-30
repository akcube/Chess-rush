#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

// Game-related State data
SpriteRenderer  *Renderer;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    int num_textures = sizeof(texturesList)/sizeof(texturesList[0]);
    for(int i=0; i<num_textures; i++)
        ResourceManager::LoadTexture(texturesList[i][0].c_str(), true, texturesList[i][1].c_str());
    // load levels
    GameLevel one; one.Load(50, this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Level = 0;    
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
   
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        // draw background
        // Renderer->DrawSprite(ResourceManager::GetTexture("background"), 
        //     glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
        // );
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
    }
}
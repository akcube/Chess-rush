#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

#include <iostream>

// Game-related State data
SpriteRenderer  *Renderer;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), tilesize(50)
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
    GameLevel one; one.Load(this->tilesize, this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;    
}

glm::vec2 nxtPos(glm::vec2 curpos, char dir){
    if(dir == 'L') curpos.x--;
    else if(dir == 'R') curpos.x++;
    else if(dir == 'U') curpos.y--;
    else if(dir == 'D') curpos.y++;
    else if(dir == 'Q') curpos.x--, curpos.y--;
    else if(dir == 'E') curpos.x++, curpos.y--;
    else if(dir == 'C') curpos.x++, curpos.y++;
    else if(dir == 'Z') curpos.x--, curpos.y++;
    return curpos;
}

inst randomDiagonal(GameLevel &level, glm::vec2 curpos, int tsz=50){
    std::vector<std::vector<bool>> &filled = level.filled;
    int dy[4] = {-1, -1, +1, +1};
    int dx[4] = {-1, +1, +1, -1};
    char dirC[4] = {'Q', 'E', 'C', 'Z'};
    int dir = rand()%4;
    int len = rand()%17+1;
    inst retval;
    retval.dir = dirC[dir];
    int newx = curpos.x; int newy = curpos.y;
    for(int l=0; l<=len; l++){
        retval.path.push_back({newy/tsz, newx/tsz});
        retval.target_x = newx;
        retval.target_y = newy;
        newx += dx[dir]*tsz;
        newy += dy[dir]*tsz;
        if(filled[newy/tsz][newx/tsz]) break;
        filled[newy/tsz][newx/tsz] = true;
    }
    if(retval.target_x == curpos.x && retval.target_y == curpos.y) retval.dir = 'X';
    return retval;
}

using namespace std;

inst randomLine(GameLevel &level, glm::vec2 curpos, int maxlen = 1000, char fDir = 'S', int tsz=50){
    std::vector<std::vector<bool>> &filled = level.filled;
    int dy[4] = {-1, 0, +1, 0};
    int dx[4] = {0, +1, 0, -1};
    char dirC[4] = {'U', 'R', 'D', 'L'};
    int dir, len;
    if(fDir == 'S') dir = rand()%4;
    else{
        switch(fDir){
            case 'U' : dir = 0; break;
            case 'R' : dir = 1; break;
            case 'D' : dir = 2; break;
            case 'L' : dir = 3; break;
            default  : dir = 0; break;
        }
    }
    if(maxlen == 1000) len = rand()%17+1;
    else len = maxlen;
    inst retval;
    retval.dir = dirC[dir];
    int newx = curpos.x; int newy = curpos.y;
    retval.target_x = newx;
    retval.target_y = newy;
    for(int l=0; l<=len; l++){
        retval.path.push_back({newy/tsz, newx/tsz});
        newx += dx[dir]*tsz;
        newy += dy[dir]*tsz;
        if(filled[newy/tsz][newx/tsz]) break;
        filled[newy/tsz][newx/tsz] = true;
        retval.path.push_back({newy/tsz, newx/tsz});
        retval.target_x = newx;
        retval.target_y = newy;
    }
    if(retval.target_x == curpos.x && retval.target_y == curpos.y) retval.dir = 'X';
    return retval;
}

bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
} 

bool isEnemyPiece(GameObject &x){
    if(x.Type == TYPE_QUEEN) return true;
    if(x.Type == TYPE_KNIGHT) return true;
    if(x.Type == TYPE_KING) return true;
    if(x.Type == TYPE_BISHOP) return true;
    if(x.Type == TYPE_ROOK) return true;
    else return false;
}

void Game::Update(float dt)
{
    auto &level = this->Levels[this->Level];
    if(this->State == GAME_ACTIVE){
        for(auto &p : level.Pieces){
            glm::vec2 curpos = p.Position;

            if(!p.q.empty()){
                inst &top = p.q.front();
                if(top.target_x == curpos.x && top.target_y == curpos.y){
                    for(auto &p:top.path)
                        level.filled[p.first][p.second] = false;
                    p.q.pop();
                }
            }
            if(p.q.empty()){
                inst nxt, nxt2;
                switch(p.Type){
                    case TYPE_QUEEN:
                        nxt = (rand()%2) ? randomLine(level, curpos) : randomDiagonal(level, curpos);                        
                        p.q.push(nxt);
                    break;
                    case TYPE_ROOK:
                        nxt = randomLine(level, curpos);
                        p.q.push(nxt);
                    break;
                    case TYPE_BISHOP:
                        nxt = randomDiagonal(level, curpos);
                        p.q.push(nxt);
                    break;
                    case TYPE_KING:
                        nxt = randomLine(level, curpos, 1);
                        p.q.push(nxt);
                    break;
                    case TYPE_KNIGHT:
                        nxt = randomLine(level, curpos, 1);
                        if(nxt.dir == 'L' or nxt.dir == 'R') 
                            nxt2 = (rand()%2) ? randomLine(level, glm::vec2(nxt.target_x, nxt.target_y), 1, 'U') : randomLine(level, glm::vec2(nxt.target_x, nxt.target_y), 1, 'D');
                        else if(nxt.dir == 'U' or nxt.dir == 'D') 
                            nxt2 = (rand()%2) ? randomLine(level, glm::vec2(nxt.target_x, nxt.target_y), 1, 'L') : randomLine(level, glm::vec2(nxt.target_x, nxt.target_y), 1, 'R');
                        if(nxt.dir != 'X' and nxt2.dir != 'X'){
                            p.q.push(nxt);
                            p.q.push(nxt2);
                        }
                        else p.q.push(nxt);
                    break;
                }
            }
            inst &current = p.q.front();
            p.Position = nxtPos(curpos, current.dir);

            for(int i=0; i<level.Pieces.size(); i++){
                for(int j=0; j<level.Pieces.size(); j++){
                    if(i == j) continue;
                    if(!isEnemyPiece(level.Pieces[i])) continue;
                    if(!isEnemyPiece(level.Pieces[j])) continue;
                    if(level.Pieces[i].Destroyed or !level.Pieces[i].IsSolid) continue;
                    if(level.Pieces[j].Destroyed or !level.Pieces[j].IsSolid) continue;
                    if(CheckCollision(level.Pieces[i], level.Pieces[j]) && level.Pieces[i].chessColor != level.Pieces[j].chessColor){
                        if(rand()%2)
                            level.Pieces[i].Destroyed = true;
                        else 
                            level.Pieces[j].Destroyed = true;
                    }
                }
            }

            for(int i=0; i<level.Pieces.size(); i++){
                if(level.Pieces[i].Destroyed or !level.Pieces[i].IsSolid) continue;
                if(CheckCollision(level.Pieces[i], level.Player)){
                    cout<<"Object: "<<level.Pieces[i].Position.x<<" "<<level.Pieces[i].Position.y<<endl;
                    this->State = GAME_MENU;
                }
            }

        }
    }
}

void Game::ProcessInput(float dt)
{
    auto &level = this->Levels[this->Level];
    GameObject shitcode = level.Player;
    if(Keys[GLFW_KEY_W]) shitcode.Position.y--;
    if(Keys[GLFW_KEY_A]) shitcode.Position.x--;
    if(Keys[GLFW_KEY_S]) shitcode.Position.y++;
    if(Keys[GLFW_KEY_D]) shitcode.Position.x++;
    bool valid = true;
    for(auto &row:level.Grid)
        for(auto &cell:row){
            if(cell.IsSolid && CheckCollision(shitcode, cell)){
                valid = false;
            }
        }
    if(valid) level.Player.Position = shitcode.Position;
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        this->Levels[this->Level].Draw(*Renderer);
    }
}
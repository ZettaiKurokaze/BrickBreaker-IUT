//#if       _WIN32_WINNT < 0x0500
//  #undef  _WIN32_WINNT
//  #define _WIN32_WINNT   0x0500
//#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
//#include<windows.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#define MainMenu  1
#define NewGame 2
#define LoadGame 3
#define Options 4
#define Credits 5
#define Quit 6
#define GameOver 7
#define Loading 8
#define Confirmquit 9
#define Countdown 10
#define LevelCreator 11
typedef struct
{
    double x, y, vx, vy, w, h, spf, vlim, r;
} Ball;
typedef struct
{
    double x, y;
} Ballpoints;
typedef struct
{
    double x, y;
    int b, p;
} Brick;
typedef struct
{
    double x, y, w, h;
} Load;
typedef struct
{
    double x, y, w, h;
} Pad;
typedef struct
{
    double w, h, f, l, r;
} Res;
typedef struct
{
    double x, y, w, h;
    int c;
} Menu;
typedef struct
{
    double x, y, w, h;
} Score;
typedef struct
{
    double x, y, w, h;
} Counts;
typedef struct
{
    double w, h;
} Block;
typedef struct
{
    double x1, x2;
} Mouse;
time_t time1, time2;
typedef struct
{
    int State, prevState, secs, music[3], prevScore, HighScore, lives, stateload[12];
    char PlayerName[100];
    Res res;
    Mouse mouse;
    Ball balls;
    Pad pads;
    Load load;
    Menu menu[8];
    Score score[8];
    Block blocks;
    Counts counts;
    Brick brick[20][20];
    Ballpoints ballpoints;
    SDL_Texture *pad;
    SDL_Texture *ball;
    SDL_Texture *gameover;
    SDL_Texture *confirm;
    SDL_Texture *Count[3];
    SDL_Texture *block[7];
    SDL_Texture *background[10];
    SDL_Texture *number[10];
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *label[16];
    TTF_Font *font;
    Mix_Music *mIngame;
    Mix_Music *mMenu;
    Mix_Music *mGameOver;
    Mix_Chunk *cPadbounce;
    Mix_Chunk *cBrickbounce;
    Mix_Chunk *cWallbounce;
    Mix_Chunk *cSelect;

} GameState;
SDL_Texture* loadTexture(SDL_Texture *loadedTexture, char *ch, GameState *game)
{
    SDL_Surface *Surface = NULL;
    Surface = IMG_Load(ch);
    loadedTexture = SDL_CreateTextureFromSurface(game->renderer, Surface);
    SDL_FreeSurface(Surface);
    return loadedTexture;
}
void mainmenu(GameState *game)
{
    SDL_Color lightBlue = { 128, 192, 255, 200 };
    SDL_Color Blue = { 0, 0, 255, 255 };
    SDL_Surface *Surface = NULL;
    game->background[1]=loadTexture(game->background[1], "Images/Background1.png", game);
    game->menu[0].w = 435*game->res.f;
    game->menu[0].h = 86*game->res.f;
    game->label[0] = loadTexture(game->label[0], "Images/Newgame0.png", game);
    game->label[4] = loadTexture(game->label[4], "Images/Newgame1.png", game);
    game->menu[1].w = 484*game->res.f;
    game->menu[1].h = game->menu[0].h;
    game->label[1] = loadTexture(game->label[1], "Images/Loadgame0.png", game);
    game->label[5] = loadTexture(game->label[5], "Images/Loadgame1.png", game);
    game->menu[2].w = 370*game->res.f;
    game->menu[2].h = game->menu[0].h;
    game->label[2] = loadTexture(game->label[2], "Images/Options0.png", game);
    game->label[6] = loadTexture(game->label[6], "Images/Options1.png", game);
    game->menu[3].w = 196*game->res.f;
    game->menu[3].h = game->menu[0].h;
    game->label[3] = loadTexture(game->label[3], "Images/Exit0.png", game);
    game->label[7] = loadTexture(game->label[7], "Images/Exit1.png", game);
    int y=(game->res.h-4*game->menu[0].h-3*20*game->res.f)/2;
    for (int i=0; i<4; i++)
    {
        game->menu[i].x = 40*game->res.f;
        game->menu[i].y = y+(i*(game->menu[i].h+20*game->res.f));
    }
    game->mMenu = Mix_LoadMUS( "Sounds/Menu.mod" );
    game->cSelect = Mix_LoadWAV( "Sounds/Select.wav" );
}
void gameover(GameState *game)
{
    game->gameover= loadTexture(game->gameover, "Images/GameOver.png", game);
}
void confirmquit(GameState *game)
{
    double f=1.5*game->res.f;
    game->font = TTF_OpenFont("Fonts/BigSpace.otf", 48);
    game->background[2]= loadTexture( game->background[2], "Images/Background9.png", game);
    game->confirm = loadTexture(game->confirm, "Images/Confirm.png", game);
    SDL_Color lightBlue = { 128, 192, 255, 255 };
    SDL_Color Blue = { 0, 0, 255, 255 };
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "Yes", lightBlue);
    game->menu[4].w = 113*game->res.f;
    game->menu[4].h = 57*game->res.f;
    game->label[8] = loadTexture( game->label[8], "Images/Yes0.png", game);
    game->label[10] = loadTexture( game->label[10], "Images/Yes1.png", game);
    game->menu[5].w = game->menu[4].w;
    game->menu[5].h = game->menu[4].h;
    game->label[9] = loadTexture( game->label[9], "Images/No0.png", game);
    game->label[11] = loadTexture( game->label[11], "Images/No1.png", game);
    game->menu[6].w = 380*game->res.f;
    game->menu[6].h = 93*game->res.f;
    game->label[12] = loadTexture( game->label[12], "Images/quit.png", game);
    game->menu[4].x=867*game->res.f-game->menu[4].w/2;
    game->menu[4].y=620*game->res.f-game->menu[4].h/2;
    game->menu[5].x=game->menu[4].x+243*game->res.f-game->menu[5].w/2;
    game->menu[5].y=game->menu[4].y;
    game->menu[6].x=(game->res.w-game->menu[6].w)/2;
    game->menu[6].y=450*game->res.f;
    game->menu[7].x=(game->res.w-game->menu[7].w)/2;
    game->menu[7].y=game->menu[6].y+game->menu[6].h;
}
void loading(GameState *game)
{
    SDL_Surface *Surface = NULL;
    Surface = IMG_Load("Images/Loading.png");
    game->background[8]= loadTexture(game->background[8], "Images/Loading.png", game);
    game->load.w=18.5*game->res.f;
    game->load.h=60*game->res.f;
    game->load.x=170*game->res.f-game->load.w/2;
    game->load.y=550*game->res.f-game->load.h/2;

}
void countdown(GameState *game)
{
    char ch[30]="Images/count (0).png";
    for(int i=0; i<3; i++)
    {
        ch[14]++;
        game->Count[i] = loadTexture(game->Count[i], ch, game);
    }
    game->counts.w=600*game->res.f;
    game->counts.h=game->counts.w;
    game->counts.x=(game->res.w-game->counts.w)/2;
    game->counts.y=(game->res.h-game->counts.h)/2;
}
void newgame(GameState *game)
{
    game->ball = loadTexture(game->ball, "Images/Ball.png", game);
    game->pad = loadTexture(game->pad, "Images/pad.png", game);
    game->background[0]= loadTexture(game->background[0], "Images/Background.jpg", game);
    game->label[13]= loadTexture(game->label[13], "Images/edge2.png", game);
    char ch[25]="Images/block (0).png";
    for(int i=0; i<7; i++)
    {
        ch[14]++;
        game->block[i]=loadTexture(game->block[i], ch, game);
    }
    strcpy(ch,"Images/num (0).png");
    for(int i=0; i<10; i++)
    {
        if(i<8)
        {
            game->score[i].w=120*game->res.f;
            game->score[i].h=game->score[i].w*.78;
            game->score[i].x=game->res.r+game->res.l/2-game->score[i].w/2;
            game->score[i].y=i*game->score[i].h;
        }
        game->number[i]=loadTexture(game->number[i], ch, game);
        ch[12]++;
    }
    game->pads.w = 200*game->res.f;
    game->pads.h = 20*game->res.f;
    game->pads.x = (game->res.w-(game->pads.w))/2;
    game->pads.y = game->res.h-20*game->res.f-(game->pads.h);
    game->balls.spf = .01;
    game->balls.vlim = 8;
    game->balls.vx = 0;
    game->balls.vy = 0;
    game->balls.w = 20*game->res.f;
    game->balls.h = game->balls.w;
    game->balls.r=game->balls.w/2;
    game->balls.x = (game->res.w-(game->balls.w))/2;
    game->balls.y = game->pads.y-(game->balls.h)-1;
    game->blocks.w=72*game->res.f;
    game->blocks.h=36*game->res.f;
    for(int i=0; i<20; i++)
    {
        for(int j=0; j<20; j++)
        {
            game->brick[i][j].x=240*game->res.f+j*game->blocks.w;
            game->brick[i][j].y=(i+1)*game->blocks.h;
            game->brick[i][j].b=rand()%8;
            //if(j>10) game->brick[i][j].b=0;
            game->brick[i][j].p=0;
        }
    }
}
void collisionDetect(GameState *game)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT] && game->pads.x>game->res.l  && game->State == NewGame)
    {
        game->pads.x -= 10;
        if(game->pads.x<=game->res.l)
            game->pads.x = game->res.l;
        if(game->balls.vx==0)
            game->balls.x=game->pads.x+game->pads.w/2-game->balls.w/2;

    }
    if(state[SDL_SCANCODE_RIGHT] && game->pads.x<(game->res.r-game->pads.w)  && game->State == NewGame)
    {
        game->pads.x += 10;
        if(game->pads.x>=game->res.r-game->pads.w)
            game->pads.x = game->res.r-game->pads.w;
        if(game->balls.vx==0)
            game->balls.x=game->pads.x+game->pads.w/2-game->balls.w/2;
    }
    game->balls.x +=game->balls.vx;
    game->balls.y +=game->balls.vy;
    if(game->balls.x<=game->res.l || game->balls.x>=(game->res.r-game->balls.w))
    {
        game->balls.vx=(game->balls.vx*-1);
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        if(game->balls.vy<0)
            game->balls.vy-=game->balls.spf;
        else if(game->balls.vy>0)
            game->balls.vy+=game->balls.spf;
        Mix_PlayChannel( -1, game->cWallbounce, 0 );
    }
    if( game->balls.vy<0 && game->balls.y<=0 )
    {
        game->balls.vy=(game->balls.vy*-1);
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->balls.vy+=game->balls.spf;
        Mix_PlayChannel( -1, game->cWallbounce, 0 );
    }
    if(game->balls.y>=(game->pads.y-game->balls.h) && (game->pads.x+game->pads.w)>(game->balls.x) && (game->pads.x)<(game->balls.x+game->balls.w) && game->balls.vy>0)
    {
        game->balls.vy=(game->balls.vy*-1);
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->balls.vy-=game->balls.spf;
        Mix_PlayChannel( -1, game->cPadbounce, 0 );
    }
    if(game->balls.vx<-game->balls.vlim)
        game->balls.vx=-game->balls.vlim;
    if(game->balls.vx>game->balls.vlim)
        game->balls.vx=game->balls.vlim;
    if(game->balls.vy<-game->balls.vlim)
        game->balls.vy=-game->balls.vlim;
    if(game->balls.vy>game->balls.vlim)
        game->balls.vy=game->balls.vlim;
    if(game->balls.y>=(game->res.h-game->balls.h))
    {
        game->pads.x = (game->res.w-(game->pads.w))/2;
        game->pads.y = game->res.h-(game->pads.h)-10;
        game->balls.vx = 0;
        game->balls.vy = 0;
        game->balls.x = (game->res.w-(game->balls.w))/2;
        game->balls.y = game->pads.y-(game->balls.h)-1;
        if(game->lives==0)
        {
            game->prevState=game->State;
            game->State = GameOver;
            game->lives=3;
        }
        else
            game->lives--;
    }
    int i,j,r, bx, by;
    r=game->balls.r;
    bx=game->balls.x+r;
    by=game->balls.y+r;
    j=(bx-game->res.l)/game->blocks.w;
    i=(by/game->blocks.h);
    if(i<20 && game->balls.vy>0 && bx<=(game->brick[i][j].x+game->blocks.w) && bx>=game->brick[i][j].x && by>=game->brick[i][j].y-r && game->brick[i][j].b!=0)
    {
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy-=game->balls.spf;
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->HighScore+=game->brick[i][j].b*5;
        game->brick[i][j].b=0;
        Mix_PlayChannel( -1, game->cBrickbounce, 0 );
    }
    i-=2;
    if(i<20 && i>=0 && game->balls.vy<0 && bx<=(game->brick[i][j].x+game->blocks.w) && bx>=game->brick[i][j].x && by<=(game->brick[i][j].y+game->blocks.h+r) && game->brick[i][j].b!=0)
    {
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy+=game->balls.spf;
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->HighScore+=game->brick[i][j].b*5;
        game->brick[i][j].b=0;
        Mix_PlayChannel( -1, game->cBrickbounce, 0 );
    }
    i++;
    if(j<19 && i<20 && game->balls.vx>0 && by>=game->brick[i][j].y && by<=(game->brick[i][j].y+game->blocks.h) && bx>=game->brick[i][j+1].x-r && bx<=(game->brick[i][j+1].x+game->blocks.w) && game->brick[i][j+1].b!=0)
    {
        game->balls.vx=(game->balls.vx*-1);
        game->balls.vx-=game->balls.spf;
        if(game->balls.vy<0)
            game->balls.vy-=game->balls.spf;
        else if(game->balls.vy>0)
            game->balls.vy+=game->balls.spf;
        game->HighScore+=game->brick[i][j+1].b*5;
        game->brick[i][j+1].b=0;
        Mix_PlayChannel( -1, game->cBrickbounce, 0 );
    }
    if(i<20 && game->balls.vx<0 && by>=game->brick[i][j].y && by<=(game->brick[i][j].y+game->blocks.h) && bx>=game->brick[i][j-1].x && bx<=(game->brick[i][j-1].x+game->blocks.w+r) && game->brick[i][j-1].b!=0)
    {
        game->balls.vx=(game->balls.vx*-1);
        game->balls.vx+=game->balls.spf;
        if(game->balls.vy<0)
            game->balls.vy-=game->balls.spf;
        else if(game->balls.vy>0)
            game->balls.vy+=game->balls.spf;
        game->HighScore+=game->brick[i][j-1].b*5;
        game->brick[i][j-1].b=0;
        Mix_PlayChannel( -1, game->cBrickbounce, 0 );
    }
}
void loadMusic (GameState *game)
{
    game->mIngame = Mix_LoadMUS( "Sounds/Ingame.mod" );
    game->mGameOver = Mix_LoadMUS( "Sounds/GameOver.mod" );
    game->cPadbounce = Mix_LoadWAV( "Sounds/PadBounce.wav" );
    game->cBrickbounce = Mix_LoadWAV( "Sounds/BrickBounce.wav" );
    game->cWallbounce = Mix_LoadWAV( "Sounds/WallBounce.wav" );
}

//#if       _WIN32_WINNT < 0x0500
//  #undef  _WIN32_WINNT
//  #define _WIN32_WINNT   0x0500
//#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include<windows.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
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

typedef struct
{
    double x, y, vx, vy, w, h, spf, vlim;
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
    int State, prevState, secs;
    Res res;
    Mouse mouse;
    Ball balls;
    Pad pads;
    Load load;
    Menu menu[8];
    Block blocks;
    Counts counts;
    Brick brick[20][20];
    Ballpoints ballpoints[8];
    SDL_Texture *pad;
    SDL_Texture *ball;
    SDL_Texture *gameover;
    SDL_Texture *confirm;
    SDL_Texture *Count[3];
    SDL_Texture *block[7];
    SDL_Texture *background[10];
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *label[14];
    TTF_Font *font;

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
    int x;
    double f=2*game->res.f;
    game->font = TTF_OpenFont("Fonts/BigSpace.otf", 48);
    if(!game->font)
    {
        printf("Cannot find font file!\n\n");
        SDL_Quit();
        exit(1);
    }
    SDL_Color lightBlue = { 128, 192, 255, 200 };
    SDL_Color Blue = { 0, 0, 255, 255 };
    SDL_Surface *Surface = NULL;
    game->background[1]=loadTexture(game->background[1], "Images/Background1.jpg", game);
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "New Game", lightBlue);
    game->menu[0].w = tmp->w*f;
    game->menu[0].h = tmp->h*f;
    game->label[0] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "New Game", Blue);
    game->label[4] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Load Game", lightBlue);
    game->menu[1].w = tmp->w*f;
    game->menu[1].h = tmp->h*f;
    game->label[1] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Load Game", Blue);
    game->label[5] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Options", lightBlue);
    game->menu[2].w = tmp->w*f;
    game->menu[2].h = tmp->h*f;
    game->label[2] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Options", Blue);
    game->label[6] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Exit", lightBlue);
    game->menu[3].w = tmp->w*f;
    game->menu[3].h = tmp->h*f;
    game->label[3] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Exit", Blue);
    game->label[7] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    x=(game->res.h-4*game->menu[0].h)/2;
    for (int i=0; i<4; i++)
    {
        game->menu[i].x = 40*game->res.f;
        game->menu[i].y = x+(i*(game->menu[i].h));
    }
}
void gameover(GameState *game)
{
    game->gameover= loadTexture(game->gameover, "Images/GameOver.png", game);
}
void confirmquit(GameState *game)
{
    double f=1.5*game->res.f;
    game->background[2]= loadTexture( game->background[2], "Images/Background9.png", game);
    game->confirm = loadTexture(game->confirm, "Images/Confirm.png", game);
    SDL_Color lightBlue = { 128, 192, 255, 255 };
    SDL_Color Blue = { 0, 0, 255, 255 };
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "Yes", lightBlue);
    game->menu[4].w = tmp->w*f;
    game->menu[4].h = tmp->h*f;
    game->label[8] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Yes", Blue);
    game->label[10] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "No", lightBlue);
    game->menu[5].w = tmp->w*f;
    game->menu[5].h = tmp->h*f;
    game->label[9] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "No", Blue);
    game->label[11] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "Are you sure", lightBlue);
    game->menu[6].w = tmp->w*game->res.f;
    game->menu[6].h = tmp->h*game->res.f;
    game->label[12] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font, "you want to quit?", lightBlue);
    game->menu[7].w = tmp->w*game->res.f;
    game->menu[7].h = tmp->h*game->res.f;
    game->label[13] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    game->menu[4].x=867*game->res.f-game->menu[4].w/2;
    game->menu[4].y=620*game->res.f-game->menu[4].h/2;
    game->menu[5].x=game->menu[4].x+243*game->res.f-game->menu[5].w/2;
    game->menu[5].y=game->menu[4].y;
    game->menu[6].x=(game->res.w-game->menu[6].w)/2;
    game->menu[6].y=430*game->res.f;
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
    char ch[30]="Images/count(0).png";
    for(int i=0; i<3; i++)
    {
        ch[13]++;
        game->Count[i] = loadTexture(game->Count[i], ch, game);
    }
    game->counts.w=200*game->res.f;
    game->counts.h=game->counts.w*(467/366);
    game->counts.x=(game->res.w-game->counts.w)/2;
    game->counts.y=(game->res.h-game->counts.h)/2;
}
void newgame(GameState *game)
{
    game->ball = loadTexture(game->ball, "Images/Ball.png", game);
    game->pad = loadTexture(game->pad, "Images/pad.png", game);
    game->background[0]= loadTexture(game->background[0], "Images/Background.png", game);
    char ch[20]="Images/block0.png";
    ch[12]--;
    for(int i=0; i<7; i++)
    {
        ch[12]++;
        game->block[i]=loadTexture(game->block[i], ch, game);
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
    game->balls.h = 20*game->res.f;
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
    }
    if( game->balls.vy<0 && game->balls.y<=0 )
    {
        game->balls.vy=(game->balls.vy*-1);
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->balls.vy+=game->balls.spf;
    }
    if(game->balls.y>=(game->pads.y-game->balls.h) && (game->pads.x+game->pads.w)>(game->balls.x) && (game->pads.x)<(game->balls.x+game->balls.w) && game->balls.vy>0)
    {
        game->balls.vy=(game->balls.vy*-1);
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->balls.vy-=game->balls.spf;
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
        game->State = GameOver;
    }
    int i,j;
    j=(game->balls.x-game->res.l)/game->blocks.w;
    i=(game->balls.y/game->blocks.h);
    game->ballpoints[0].x=game->balls.x+game->balls.w/2;
    game->ballpoints[0].y=game->balls.y;
    game->ballpoints[1].x=game->balls.x+game->balls.w;
    game->ballpoints[1].y=game->balls.y+game->balls.h/2;
    game->ballpoints[2].x=game->balls.x+game->balls.w/2;
    game->ballpoints[2].y=game->balls.y+game->balls.h;
    game->ballpoints[3].x=game->balls.x;
    game->ballpoints[3].y=game->balls.y+game->balls.h/2;
    if(i<=20)
    {
        if(game->balls.vy>0 && game->ballpoints[2].x<=(game->brick[i][j].x+game->blocks.w) && game->ballpoints[2].x>=game->brick[i][j].x && game->ballpoints[2].y>=game->brick[i][j].y && game->ballpoints[2].y<=(game->brick[i][j].y+game->blocks.h) && game->brick[i][j].b!=7 && i<20)
        {
            game->balls.vy=(game->balls.vy*-1);
            game->balls.vy-=game->balls.spf;
            if(game->balls.vx<0)
                game->balls.vx-=game->balls.spf;
            else if(game->balls.vx>0)
                game->balls.vx+=game->balls.spf;
            game->brick[i][j].b=7;
        }
        i--;
        if(game->balls.vy<0 && game->ballpoints[0].x<=(game->brick[i][j].x+game->blocks.w) && game->ballpoints[0].x>=game->brick[i][j].x && game->ballpoints[0].y>=game->brick[i][j].y && game->ballpoints[0].y<=(game->brick[i][j].y+game->blocks.h) && game->brick[i][j].b!=7 && i<20)
        {
            game->balls.vy=(game->balls.vy*-1);
            game->balls.vy+=game->balls.spf;
            if(game->balls.vx<0)
                game->balls.vx-=game->balls.spf;
            else if(game->balls.vx>0)
                game->balls.vx+=game->balls.spf;
            game->brick[i][j].b=7;
        }
        if(game->balls.vx>0 && game->ballpoints[1].y>=game->brick[i][j].y && game->ballpoints[1].y<=(game->brick[i][j].y+game->blocks.h) && game->ballpoints[1].x>=game->brick[i][j+1].x && game->ballpoints[1].x<=(game->brick[i][j+1].x+game->blocks.w) && game->brick[i][j+1].b!=7)
        {
            game->balls.vx=(game->balls.vx*-1);
            game->balls.vx-=game->balls.spf;
            if(game->balls.vy<0)
                game->balls.vy-=game->balls.spf;
            else if(game->balls.vy>0)
                game->balls.vy+=game->balls.spf;
            game->brick[i][j+1].b=7;
        }
        if(game->balls.vx<0 && game->ballpoints[3].y>=game->brick[i][j].y && game->ballpoints[3].y<=(game->brick[i][j].y+game->blocks.h) && game->ballpoints[1].x>=game->brick[i][j].x && game->ballpoints[3].x<=(game->brick[i][j].x+game->blocks.w) && game->brick[i][j].b!=7)
        {
            game->balls.vx=(game->balls.vx*-1);
            game->balls.vx+=game->balls.spf;
            if(game->balls.vy<0)
                game->balls.vy-=game->balls.spf;
            else if(game->balls.vy>0)
                game->balls.vy+=game->balls.spf;
            game->brick[i][j].b=7;
        }
    }
}

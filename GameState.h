//#if       _WIN32_WINNT < 0x0500
//  #undef  _WIN32_WINNT
//  #define _WIN32_WINNT   0x0500
//#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <math.h>
//#include<windows.h>
#include <time.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
#define Paused 12
#define NameInput 13
#define HIGHSCORE 14

struct linkedlist
{
    int level;
    int b[20][20];
    struct linkedlist *next;
    struct linkedlist *prev;
};
typedef struct linkedlist *node;
struct acclist
{
    int level;
    int score;
    int lives;
    char name[11];
    struct acclist *next;
    struct acclist *prev;
};
typedef struct acclist *acc;
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
    double x, y, w, h, vecx, vecy;
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
} Text;
typedef struct
{
    double x, y, w, h;
} Heart;
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
    int State, prevState, secs, music[3], prevScore, HighScore, lives, playerlives, stateload[12], trig, lvl, playerscore, cnt, cnt1, bgmv, sfxv;
    char PlayerName[100], num[4];
    bool blink;
    node head, n, lcn;
    acc headacc, nacc;
    Res res;
    Mouse mouse;
    Ball balls;
    Pad pads;
    Load load;
    Menu menu[20];
    Heart heart[3];
    Text text[45];
    Score score[8];
    Block blocks;
    Counts counts;
    Brick brick[20][20];
    Ballpoints ballpoints;
    SDL_Texture *hearts;
    SDL_Texture *pad;
    SDL_Texture *ball;
    SDL_Texture *gameover;
    SDL_Texture *confirm;
    SDL_Texture *Count[3];
    SDL_Texture *block[7];
    SDL_Texture *background[10];
    SDL_Texture *number[10];
    SDL_Texture *Fonts[45];
    SDL_Texture *label[28];
    SDL_Renderer *renderer;
    SDL_Window *window;
    TTF_Font *font[3];
    Mix_Music *mIngame;
    Mix_Music *mMenu;
    Mix_Music *mGameOver;
    Mix_Chunk *cPadbounce;
    Mix_Chunk *cBrickbounce;
    Mix_Chunk *cWallbounce;
    Mix_Chunk *cSelect;

} GameState;
void changeLevel(GameState *game);
void initbricks(GameState *game);
void loadGame(GameState *game);
acc addAcc(acc head, FILE *fp, int t, GameState *game);
SDL_Texture* loadTexture(SDL_Texture *loadedTexture, char *ch, GameState *game)
{
    SDL_Surface *Surface = NULL;
    Surface = IMG_Load(ch);
    loadedTexture = SDL_CreateTextureFromSurface(game->renderer, Surface);
    SDL_FreeSurface(Surface);
    return loadedTexture;
}
double windowSize()
{
    double w;
    SDL_DisplayMode current;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetCurrentDisplayMode(0, &current);
    w=current.w;
    w/=1920.0;
    return w;
}
void mainmenu(GameState *game)
{
    game->background[1]=loadTexture(game->background[1], "Resources/Images/Background1.png", game);
    game->menu[0].w = 435*game->res.f;
    game->menu[0].h = 86*game->res.f;
    game->label[0] = loadTexture(game->label[0], "Resources/Images/Newgame0.png", game);
    game->label[4] = loadTexture(game->label[4], "Resources/Images/Newgame1.png", game);
    game->menu[1].w = 484*game->res.f;
    game->menu[1].h = game->menu[0].h;
    game->label[1] = loadTexture(game->label[1], "Resources/Images/Loadgame0.png", game);
    game->label[5] = loadTexture(game->label[5], "Resources/Images/Loadgame1.png", game);
    game->menu[2].w = 370*game->res.f;
    game->menu[2].h = game->menu[0].h;
    game->label[2] = loadTexture(game->label[2], "Resources/Images/Options0.png", game);
    game->label[6] = loadTexture(game->label[6], "Resources/Images/Options1.png", game);
    game->menu[3].w = 196*game->res.f;
    game->menu[3].h = game->menu[0].h;
    game->label[3] = loadTexture(game->label[3], "Resources/Images/Exit0.png", game);
    game->label[7] = loadTexture(game->label[7], "Resources/Images/Exit1.png", game);
    int y=(game->res.h-4*game->menu[0].h-3*20*game->res.f)/2;
    for (int i=0; i<4; i++)
    {
        game->menu[i].x = 40*game->res.f;
        game->menu[i].y = y+(i*(game->menu[i].h+20*game->res.f));
    }
    game->mMenu = Mix_LoadMUS( "Resources/Sounds/Menu.mod" );
    game->cSelect = Mix_LoadWAV( "Resources/Sounds/Select.wav" );
    game->label[15] = loadTexture(game->label[15], "Resources/Images/name.png", game);
    game->menu[8].w = 600*game->res.f;
    game->menu[8].h = game->menu[8].w/2;
    game->menu[8].x= game->res.w/2-game->menu[8].h;
    game->menu[8].y=(game->res.h-game->menu[8].h)/2;
}
void gameover(GameState *game)
{
    game->gameover= loadTexture(game->gameover, "Resources/Images/GameOver.png", game);
}
void confirmquit(GameState *game)
{
    double s=300*game->res.f;
    game->background[2]= loadTexture( game->background[2], "Resources/Images/Background9.jpg", game);
    game->menu[4].w = 180*game->res.f;
    game->menu[4].h = 90*game->res.f;
    game->label[8] = loadTexture( game->label[8], "Resources/Images/Yes0.png", game);
    game->label[10] = loadTexture( game->label[10], "Resources/Images/Yes1.png", game);
    game->menu[5].w = game->menu[4].w;
    game->menu[5].h = game->menu[4].h;
    game->label[9] = loadTexture( game->label[9], "Resources/Images/No0.png", game);
    game->label[11] = loadTexture( game->label[11], "Resources/Images/No1.png", game);
    game->label[12] = loadTexture( game->label[12], "Resources/Images/confirmquit1.png", game);
    game->menu[6].x=game->res.w/2-s;
    game->menu[6].y=(game->res.h-s)/2;
    game->menu[6].w = s+s;
    game->menu[6].h = s;
    game->menu[4].x=game->menu[6].x+(s+s)/3-game->menu[4].w/2;
    game->menu[4].y=620*game->res.f-game->menu[4].h/2;
    game->menu[5].x=game->menu[6].x+s/3+s-game->menu[4].w/2;
    game->menu[5].y=game->menu[4].y;
}
void loading(GameState *game)
{
    game->background[3]= loadTexture(game->background[3], "Resources/Images/Loading.png", game);
    game->load.w=18.5*game->res.f;
    game->load.h=60*game->res.f;
    game->load.x=170*game->res.f-game->load.w/2;
    game->load.y=550*game->res.f-game->load.h/2;
    game->label[14]= loadTexture(game->label[14], "Resources/Images/paused.png", game);
    game->menu[7].w=1166*game->res.f;
    game->menu[7].h=218*game->res.f;
    game->menu[7].x=(game->res.w-game->menu[7].w)/2;
    game->menu[7].y=(game->res.h-game->menu[7].h)/2;
}
void countdown(GameState *game)
{
    char ch[40]="Resources/Images/count (0).png";
    for(int i=0; i<3; i++)
    {
        ch[24]++;
        game->Count[i] = loadTexture(game->Count[i], ch, game);
    }
    game->counts.w=600*game->res.f;
    game->counts.h=game->counts.w;
    game->counts.x=(game->res.w-game->counts.w)/2;
    game->counts.y=(game->res.h-game->counts.h)/2;
}
void newgame(GameState *game)
{
    game->ball = loadTexture(game->ball, "Resources/Images/Ball.png", game);
    game->pad = loadTexture(game->pad, "Resources/Images/pad.png", game);
    game->background[0]= loadTexture(game->background[0], "Resources/Images/Background.jpg", game);
    game->label[13]= loadTexture(game->label[13], "Resources/Images/edge2.png", game);
    game->hearts=loadTexture(game->hearts, "Resources/Images/heart.png", game);
    char ch[40]="Resources/Images/block (0).png";
    for(int i=0; i<7; i++)
    {
        ch[24]++;
        game->block[i]=loadTexture(game->block[i], ch, game);
    }
    strcpy(ch,"Resources/Images/num (0).png");
    for(int i=0; i<10; i++)
    {
        if(i<8)
        {
            game->score[i].w=140*game->res.f;
            game->score[i].h=game->score[i].w*.9;
            game->score[i].x=game->res.r+game->res.l/2-game->score[i].w/2;
            game->score[i].y=i*game->score[i].h;
        }
        game->number[i]=loadTexture(game->number[i], ch, game);
        ch[22]++;
    }
    game->pads.w = 200*game->res.f;
    game->pads.h = 20*game->res.f;
    game->pads.x = (game->res.w-(game->pads.w))/2;
    game->pads.y = game->res.h-20*game->res.f-(game->pads.h);
    game->pads.vecx = game->pads.w/2;
    game->pads.vecy = game->pads.vecx*.4;
    game->balls.spf = .05*game->res.f;
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
            game->brick[i][j].b=0;
            game->brick[i][j].p=0;
        }
    }
    for(int i=0; i<game->lives; i++)
    {
        game->heart[i].w=100*game->res.f;
        game->heart[i].h=game->heart[i].w;
        game->heart[i].x=(game->res.l-game->heart[i].w)/2;
        game->heart[i].y=((game->res.l-game->heart[i].w)/2)+(i*game->heart[i].w);
    }
}
void highScore(GameState *game)
{
    game->cnt1=0;
    SDL_Color Black = { 255, 255, 0, 255 };
    SDL_Surface *tmp;
    char ar[11];
    game->nacc=game->headacc;
    for(int i=10;i<40;i+=3)
    {
        tmp = TTF_RenderText_Blended(game->font[2], game->nacc->name, Black);
        game->text[i].h = tmp->h*game->res.f;
        game->text[i].w = tmp->w*game->res.f;
        game->Fonts[i] = SDL_CreateTextureFromSurface(game->renderer, tmp);
        SDL_FreeSurface(tmp);
        sprintf(ar,"%d",game->nacc->level);
        tmp = TTF_RenderText_Blended(game->font[1], ar, Black);
        game->text[i+1].h = tmp->h*game->res.f;
        game->text[i+1].w = tmp->w*game->res.f;
        game->Fonts[i+1] = SDL_CreateTextureFromSurface(game->renderer, tmp);
        SDL_FreeSurface(tmp);
        sprintf(ar,"%d",game->nacc->score);
        tmp = TTF_RenderText_Blended(game->font[1], ar, Black);
        game->text[i+2].h = tmp->h*game->res.f;
        game->text[i+2].w = tmp->w*game->res.f;
        game->Fonts[i+2] = SDL_CreateTextureFromSurface(game->renderer, tmp);
        SDL_FreeSurface(tmp);
        game->cnt1++;
        if(game->nacc->next==NULL)
            break;
        game->nacc=game->nacc->next;
    }
}
void levelcreator(GameState *game)
{
    game->background[4]=loadTexture(game->background[4], "Resources/Images/highscores.png", game);
    game->font[1] = TTF_OpenFont("Resources/Fonts/pdark.ttf", 48);
    game->font[2] = TTF_OpenFont("Resources/Fonts/BigSpace.otf", 48);
    game->font[0] = TTF_OpenFont("Resources/Fonts/Crazy-Pixel.ttf", 48);
    SDL_Color Black = { 255, 255, 0, 255 };
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font[0], "Level: 0", Black);
    game->text[0].h = tmp->h*game->res.f;
    game->text[0].w = tmp->w*game->res.f;
    game->Fonts[0] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "For Overwrite press LCTRL+O", Black);
    game->text[1].h = tmp->h*game->res.f;
    game->text[1].w = tmp->w*game->res.f;
    game->Fonts[1] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "For Insert press LCTRL+S", Black);
    game->text[2].h = tmp->h*game->res.f;
    game->text[2].w = tmp->w*game->res.f;
    game->Fonts[2] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "For Delete press LCTRL+D", Black);
    game->text[3].h = tmp->h*game->res.f;
    game->text[3].w = tmp->w*game->res.f;
    game->Fonts[3] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "For Switching press LCTRL+(Left/Right) Arrow", Black);
    game->text[4].h = tmp->h*game->res.f;
    game->text[4].w = tmp->w*game->res.f;
    game->Fonts[4] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "For Level No. press LCTRL+L", Black);
    game->text[5].h = tmp->h*game->res.f;
    game->text[5].w = tmp->w*game->res.f;
    game->Fonts[5] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "Confirm Y/N", Black);
    game->text[6].h = tmp->h*game->res.f;
    game->text[6].w = tmp->w*game->res.f;
    game->Fonts[6] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "ERROR. Press Spacebar.", Black);
    game->text[7].h = tmp->h*game->res.f;
    game->text[7].w = tmp->w*game->res.f;
    game->Fonts[7] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "Enter Level No:", Black);
    game->text[8].h = tmp->h*game->res.f;
    game->text[8].w = tmp->w*game->res.f;
    game->Fonts[8] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(game->font[0], "|", Black);
    game->text[9].h = tmp->h*game->res.f;
    game->text[9].w = tmp->w*game->res.f;
    game->Fonts[9] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);

}
void loadgame(GameState *game)
{
    game->background[7]=loadTexture(game->background[7], "Resources/Images/Background9.png", game);
    game->background[6]=loadTexture(game->background[6], "Resources/Images/loadgame.png", game);
    game->background[8]=loadTexture(game->background[8], "Resources/Images/Background2.png", game);
    game->label[16]=loadTexture(game->label[16], "Resources/Images/left1.png", game);
    game->label[17]=loadTexture(game->label[17], "Resources/Images/left2.png", game);
    game->label[18]=loadTexture(game->label[18], "Resources/Images/ok1.png", game);
    game->label[19]=loadTexture(game->label[19], "Resources/Images/ok2.png", game);
    game->label[20]=loadTexture(game->label[20], "Resources/Images/right1.png", game);
    game->label[21]=loadTexture(game->label[21], "Resources/Images/right2.png", game);
    game->label[22]=loadTexture(game->label[22], "Resources/Images/delete0.png", game);
    game->label[23]=loadTexture(game->label[23], "Resources/Images/delete1.png", game);
    game->menu[9].x=489*game->res.f;
    game->menu[9].y=542*game->res.f;
    game->menu[9].w=180*game->res.f;
    game->menu[9].h=182*game->res.f;
    game->menu[10].x=797*game->res.f;
    game->menu[10].y=540*game->res.f;
    game->menu[10].w=327*game->res.f;
    game->menu[10].h=184*game->res.f;
    game->menu[11].x=1251*game->res.f;
    game->menu[11].y=539*game->res.f;
    game->menu[11].w=189*game->res.f;
    game->menu[11].h=182*game->res.f;
    game->menu[12].x=1560*game->res.f;
    game->menu[12].y=320*game->res.f;
    game->menu[12].w=69*game->res.f;
    game->menu[12].h=69*game->res.f;
    game->label[25]=loadTexture(game->label[25], "Resources/Images/vol.png", game);
    game->label[26]=loadTexture(game->label[26], "Resources/Images/lvol.png", game);
    game->label[27]=loadTexture(game->label[27], "Resources/Images/rvol.png", game);
    game->label[24]=loadTexture(game->label[24], "Resources/Images/credit.png", game);
    game->menu[13].x=793*game->res.f;
    game->menu[13].y=862*game->res.f;
    game->menu[13].w=332*game->res.f;
    game->menu[13].h=55*game->res.f;
    game->menu[14].x=943*game->res.f;
    game->menu[14].y=620*game->res.f;
    game->menu[14].w=55*game->res.f;
    game->menu[14].h=58*game->res.f;
    game->menu[15].x=1218*game->res.f;
    game->menu[15].y=game->menu[14].y;
    game->menu[15].w=55*game->res.f;
    game->menu[15].h=58*game->res.f;
    game->menu[16].x=game->menu[14].x;
    game->menu[16].y=722*game->res.f;
    game->menu[16].w=55*game->res.f;
    game->menu[16].h=58*game->res.f;
    game->menu[17].x=game->menu[15].x;
    game->menu[17].y=game->menu[16].y;
    game->menu[17].w=55*game->res.f;
    game->menu[17].h=58*game->res.f;
    game->menu[18].x=1009*game->res.f;
    game->menu[18].y=620*game->res.f;
    game->menu[18].w=25*game->res.f;
    game->menu[18].h=58*game->res.f;
    game->menu[19].x=game->menu[18].x;
    game->menu[19].y=722*game->res.f;
    game->menu[19].w=game->menu[18].w;
    game->menu[19].h=game->menu[18].h;
}
void initloadgame(GameState *game)
{
    SDL_Color Black = { 255, 255, 0, 255 };
    char ar[100];
    double f=1.8;
    sprintf(ar,"Name: %s", game->nacc->name);
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font[2], ar, Black);
    game->text[41].h = tmp->h*game->res.f*f;
    game->text[41].w = tmp->w*game->res.f*f;
    game->text[41].x = 380*game->res.f;
    game->text[41].y = 325*game->res.f;
    game->Fonts[41] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    sprintf(ar,"HighScore: %d", game->nacc->score);
    tmp = TTF_RenderText_Blended(game->font[2], ar, Black);
    game->text[42].h = tmp->h*game->res.f*f;
    game->text[42].w = tmp->w*game->res.f*f;
    game->text[42].x = game->text[41].x;
    game->text[42].y = game->text[41].y+game->text[41].h;
    game->Fonts[42] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    sprintf(ar,"Level: %d", game->nacc->level);
    tmp = TTF_RenderText_Blended(game->font[2], ar, Black);
    game->text[43].h = tmp->h*game->res.f*f;
    game->text[43].w = tmp->w*game->res.f*f;
    game->text[43].x = game->text[41].x+450*game->res.f*f;
    game->text[43].y = game->text[41].y;
    game->Fonts[43] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    sprintf(ar,"Lives: %d", game->nacc->lives);
    tmp = TTF_RenderText_Blended(game->font[2], ar, Black);
    game->text[44].h = tmp->h*game->res.f*f;
    game->text[44].w = tmp->w*game->res.f*f;
    game->text[44].x = game->text[43].x;
    game->text[44].y = game->text[42].y;
    game->Fonts[44] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);

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
    if(game->balls.y>=(game->pads.y-game->balls.h) && (game->pads.x+game->pads.w)>(game->balls.x+game->balls.r) && (game->pads.x)<(game->balls.x+game->balls.r) && game->balls.vy>0)
    {
        double x=(game->balls.x+game->balls.r)-(game->pads.vecx+game->pads.x),y=game->pads.vecy, h;
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy-=game->balls.spf;
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        h=sqrt((game->balls.vy*game->balls.vy+game->balls.vx*game->balls.vx)/(x*x+y*y));
        game->balls.vy=-h*y;
        game->balls.vx=h*x;
        Mix_PlayChannel( 1, game->cPadbounce, 0 );
    }
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
        Mix_PlayChannel( 1, game->cWallbounce, 0 );
    }
    if( game->balls.vy<0 && game->balls.y<=0 )
    {
        game->balls.vy=(game->balls.vy*-1);
        if(game->balls.vx<0)
            game->balls.vx-=game->balls.spf;
        else if(game->balls.vx>0)
            game->balls.vx+=game->balls.spf;
        game->balls.vy+=game->balls.spf;
        Mix_PlayChannel( 1, game->cWallbounce, 0 );
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
            game->playerlives=game->lives;
            game->playerscore=game->HighScore;
            game->headacc=addAcc(game->headacc,NULL,1,game);
            game->lives=3;
            game->playerlives=game->lives;
            game->prevScore=0;
            game->HighScore=0;
            game->playerscore=game->HighScore;
            game->secs=1;
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
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
    }
    if(i<20 && j<19 && j>0 && game->balls.vx>0 && game->balls.vy>0 && (r*r)>=((game->brick[i][j+1].x-bx)*(game->brick[i][j+1].x-bx)+(game->brick[i][j+1].y-by)*(game->brick[i][j+1].y-by)) && game->brick[i][j+1].b!=0)
    {
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy-=game->balls.spf;
        game->balls.vx=(game->balls.vx*-1);
        game->balls.vx-=game->balls.spf;
        game->HighScore+=game->brick[i][j+1].b*5;
        game->brick[i][j+1].b=0;
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
    }
    if(i<20 && j>0 && j<19 && game->balls.vx<0 && game->balls.vy>0 && (r*r)>=((game->brick[i][j-1].x+game->blocks.w-bx)*(game->brick[i][j-1].x+game->blocks.w-bx)+(game->brick[i][j-1].y-by)*(game->brick[i][j-1].y-by)) && game->brick[i][j-1].b!=0)
    {
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy-=game->balls.spf;
        game->balls.vx=(game->balls.vx*-1);
        game->balls.vx+=game->balls.spf;
        game->HighScore+=game->brick[i][j-1].b*5;
        game->brick[i][j-1].b=0;
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
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
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
    }
    if(i<20 && i>=0 && j>0 && j<19 && game->balls.vx>0 && game->balls.vy<0 && (r*r)>=((game->brick[i][j+1].x-bx)*(game->brick[i][j+1].x-bx)+(game->brick[i][j+1].y+game->blocks.h-by)*(game->brick[i][j+1].y+game->blocks.h-by)) && game->brick[i][j+1].b!=0)
    {
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy+=game->balls.spf;
        game->balls.vx+=game->balls.spf;
        game->balls.vx=(game->balls.vx*-1);
        game->HighScore+=game->brick[i][j+1].b*5;
        game->brick[i][j+1].b=0;
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
    }
    if(i<20 && i>=0 && j>0 && j<19 && game->balls.vx<0 && game->balls.vy<0 && (r*r)>=((game->brick[i][j-1].x+game->blocks.w-bx)*(game->brick[i][j-1].x+game->blocks.w-bx)+(game->brick[i][j-1].y+game->blocks.h-by)*(game->brick[i][j-1].y+game->blocks.h-by)) && game->brick[i][j-1].b!=0)
    {
        game->balls.vy=(game->balls.vy*-1);
        game->balls.vy+=game->balls.spf;
        game->balls.vx-=game->balls.spf;
        game->balls.vx=(game->balls.vx*-1);
        game->HighScore+=game->brick[i][j-1].b*5;
        game->brick[i][j-1].b=0;
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
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
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
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
        game->cnt--;
        Mix_PlayChannel( 1, game->cBrickbounce, 0 );
    }
    if(game->cnt==0)
    {
        game->prevState=game->State;
        game->State=Countdown;
        game->secs=1;
        initbricks(game);
        game->playerscore = game->HighScore;
        game->playerlives=game->lives;
    }
}
void loadMusic(GameState *game)
{
    game->mIngame = Mix_LoadMUS( "Resources/Sounds/Ingame.mod" );
    game->mGameOver = Mix_LoadMUS( "Resources/Sounds/GameOver.mod" );
    game->cPadbounce = Mix_LoadWAV( "Resources/Sounds/PadBounce.wav" );
    game->cBrickbounce = Mix_LoadWAV( "Resources/Sounds/BrickBounce.wav" );
    game->cWallbounce = Mix_LoadWAV( "Resources/Sounds/WallBounce.wav" );
}
void playMusic(GameState *game)
{
    if((game->prevState==MainMenu && game->State!= MainMenu && game->music[0]==1))
    {
        Mix_HaltMusic();
        game->music[0]=0;
    }
    if((game->prevState==NewGame && game->State!= NewGame && game->State!= Paused && game->music[1]==1) || (game->prevState==Paused && game->State!= Paused && game->music[0]==1))
    {
        Mix_HaltMusic();
        game->music[1]=0;
    }
    if(game->prevState==GameOver && game->State!= GameOver && game->music[2]==1)
    {
        Mix_HaltMusic();
        game->music[2]=0;
    }
    if(game->prevState!=MainMenu && game->State== MainMenu && game->music[0]==0)
    {
        Mix_PlayMusic( game->mMenu, 0 );
        game->music[0]=1;
    }
    if (game->prevState!=NewGame && game->State== NewGame && game->music[1]==0)
    {
        Mix_PlayMusic( game->mIngame, 0 );
        game->music[1]=1;
    }
    if (game->prevState!=GameOver && game->State== GameOver && game->music[2]==0)
    {
        Mix_PlayMusic( game->mGameOver, 0 );
        game->music[2]=1;
    }
}
void changeLevel(GameState *game)
{
    char ar[40];
    sprintf(ar,"Level: %d", game->lvl);
    SDL_Color Black = { 255, 255, 0, 255 };
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font[0], ar, Black);
    game->text[0].h = tmp->h*game->res.f;
    game->text[0].w = tmp->w*game->res.f;
    game->Fonts[0] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
    sprintf(ar,"Enter Level No: %d", game->lvl);
    tmp = TTF_RenderText_Blended(game->font[0], ar, Black);
    game->text[8].h = tmp->h*game->res.f;
    game->text[8].w = tmp->w*game->res.f;
    game->Fonts[8] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
}
void changeName(GameState *game)
{
    SDL_Color Black = { 255, 255, 0, 255 };
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font[0], game->PlayerName, Black);
    game->text[40].h = tmp->h*game->res.f;
    game->text[40].w = tmp->w*game->res.f;
    game->text[40].x = (game->res.w-game->text[40].w)/2;
    game->text[40].y = 570*game->res.f;
    game->Fonts[40] = SDL_CreateTextureFromSurface(game->renderer, tmp);
    SDL_FreeSurface(tmp);
}
void changeVol(GameState *game)
{
    Mix_VolumeMusic(game->bgmv*16);
    Mix_Volume(1, game->sfxv*16);
}
void boot(GameState *game)
{
    srand((int)time(NULL));
    //ShowWindow(GetConsoleWindow(),SW_HIDE);
    game->res.f=windowSize();
    game->res.w=1920*game->res.f;
    game->res.h=1080*game->res.f;
    game->res.l=240*game->res.f;
    game->res.r=1680*game->res.f;
    game->window = NULL;
    game->renderer = NULL;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
    srand((int)time(NULL));
    game->window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game->res.w, game->res.h, 0);
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game->background[5]=loadTexture(game->background[5], "Resources/Images/splash.png", game);
    SDL_Rect splash={0,0,game->res.w,game->res.h};
    SDL_RenderCopy(game->renderer, game->background[5], NULL, &splash);
    SDL_RenderPresent(game->renderer);
    SDL_Delay(1000);
    loadGame(game);
    SDL_SetWindowGrab(game->window,SDL_TRUE);
}

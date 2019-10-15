#include "FileOps.h"
void loadGame(GameState *game)
{
    for (int i=0; i<20; i++)
        game->menu[i].c=0;
    for (int i=0; i<12; i++)
        game->stateload[i]=0;
    game->State=MainMenu;
    game->prevState=0;
    for(int i=40; i<45; i++)
        game->Fonts[i]=NULL;
    mainmenu(game);
    loading(game);
    confirmquit(game);
    game->secs=0;
    for (int i=0; i<3; i++)
        game->music[i]=0;
    initLevels(game);
    initAccounts(game);
    game->prevScore=0;
    game->HighScore=0;
    game->lives=3;
    game->playerlives=3;
    game->trig=0;
    game->lvl=0;
    game->playerscore=0;
    game->num[0]='\0';
    game->cnt=1;
    game->blink=1;
    game->PlayerName[0]='\0';
    newgame(game);
    countdown(game);
    loadMusic(game);
    levelcreator(game);
    gameover(game);
    loadgame(game);
    game->n=game->head;
    game->lcn=game->head;
    game->cnt1=0;
    game->bgmv=4;
    game->sfxv=4;
    changeVol(game);
}

void quitGame(GameState *game)
{
    int i;
    SDL_DestroyTexture(game->ball);
    SDL_DestroyTexture(game->gameover);
    SDL_DestroyTexture(game->pad);
    SDL_DestroyTexture(game->hearts);
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
    for(i=0; i<28; i++)
        SDL_DestroyTexture(game->label[i]);
    for(i=0; i<9; i++)
        SDL_DestroyTexture(game->background[i]);
    for(i=0; i<3; i++)
        SDL_DestroyTexture(game->Count[i]);
    for(i=0; i<7; i++)
        SDL_DestroyTexture(game->block[i]);
    for(i=0; i<10; i++)
        SDL_DestroyTexture(game->number[i]);
    for(i=0; i<((game->cnt1*3)+10); i++)
        SDL_DestroyTexture(game->Fonts[i]);
    for(i=40; i<45; i++)
        if(game->Fonts[i]!=NULL)
        SDL_DestroyTexture(game->Fonts[i]);
    Mix_FreeChunk( game->cPadbounce );
    Mix_FreeChunk( game->cBrickbounce );
    Mix_FreeChunk( game->cWallbounce );
    Mix_FreeChunk( game->cSelect );
    Mix_FreeMusic(game->mIngame);
    Mix_FreeMusic(game->mMenu);
    Mix_FreeMusic(game->mGameOver);
    Mix_Quit();
    for(i=0; i<3; i++)
        TTF_CloseFont(game->font[i]);
    TTF_Quit();
    SDL_Quit();
    saveAccounts(game);
    exit(0);
}
void processEvents(GameState *game)
{
    SDL_Event event;
    char c;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_TEXTINPUT:
        {
            if( event.text.text[0] >= '0' && event.text.text[0] <= '9' && game->trig==6 && game->State==LevelCreator && strlen(game->num)<3)
            {
                int i=1, l=strlen(game->num), j;
                c=event.text.text[0];
                game->num[l]=c;
                game->num[l+1]='\0';
                l=strlen(game->num);
                game->lvl=0;
                while(l--)
                {
                    game->lvl+=i*(game->num[l]-'0');
                    i*=10;
                }
                game->trig=7;
            }
            if( event.text.text[0] >=34 && event.text.text[0] <=126 && game->trig==0 && game->State==NameInput && strlen(game->PlayerName)<11)
            {
                int l=strlen(game->PlayerName);
                c=event.text.text[0];
                game->PlayerName[l]=c;
                game->PlayerName[l+1]='\0';
                game->trig=1;
            }
        }
        break;
        case SDL_WINDOWEVENT_CLOSE:
        {
            if(game->window)
            {
                game->prevState=game->State;
                game->State=Confirmquit;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            if( event.key.keysym.sym == SDLK_RALT && SDL_GetModState() & KMOD_CTRL && game->State==MainMenu)
            {
                game->prevState=game->State;
                game->State = Loading;
                game->stateload[LevelCreator]=1;
                game->secs=1;
            }
            if(game->State == LevelCreator)
            {
                if(event.key.keysym.sym == SDLK_SPACE && game->trig==2)
                    game->trig=0;
                if(event.key.keysym.sym == SDLK_RIGHT && SDL_GetModState() & KMOD_CTRL)
                {
                    if(game->lcn->next!=NULL)
                    {
                        game->lcn=game->lcn->next;
                        initbricks(game);
                    }
                }
                if(event.key.keysym.sym == SDLK_LEFT && SDL_GetModState() & KMOD_CTRL)
                {
                    if(game->lcn->prev!=NULL)
                    {
                        game->lcn=game->lcn->prev;
                        initbricks(game);
                    }
                }
                if(event.key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_CTRL)
                    game->trig=3;
                if(event.key.keysym.sym == SDLK_o && SDL_GetModState() & KMOD_CTRL)
                    game->trig=4;
                if(event.key.keysym.sym == SDLK_d && SDL_GetModState() & KMOD_CTRL)
                    game->trig=5;
                if(event.key.keysym.sym == SDLK_l && SDL_GetModState() & KMOD_CTRL)
                {
                    game->trig=6;
                    SDL_StartTextInput();
                }
                if(event.key.keysym.sym == SDLK_h && game->trig==0 && SDL_GetModState() & KMOD_CTRL)
                    game->trig=1;
                else if(event.key.keysym.sym == SDLK_h && game->trig==1 && SDL_GetModState() & KMOD_CTRL)
                    game->trig=0;
                if(event.key.keysym.sym == SDLK_n && (game->trig==3 || game->trig==4 || game->trig==5))
                    game->trig=0;
                if(event.key.keysym.sym == SDLK_y && (game->trig==3 || game->trig==4 || game->trig==5))
                {
                    game->head=addNode(game->head, game->lvl, NULL, game->trig, game);
                    if(game->trig!=2)
                        game->trig=0;
                }
                if((game->trig==3 || game->trig==4 || game->trig==5) && game->lvl==0)
                    game->trig=2;
            }
            if(event.key.keysym.sym == SDLK_h && SDL_GetModState() & KMOD_CTRL && game->State==MainMenu)
            {
                highScore(game);
                game->prevState=game->State;
                game->State=HIGHSCORE;
            }
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                if(game->State==MainMenu)
                {
                    game->prevState=game->State;
                    game->State=Confirmquit;
                }
                else if(game->State==Options)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                }
                else if(game->State==HIGHSCORE)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                }
                else if(game->State==LoadGame)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                }
                else if(game->State==NewGame)
                {
                    game->prevState=game->State;
                    game->State=Paused;
                    Mix_PauseMusic();
                }
                else if(game->State==LevelCreator)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                    saveLevels(game);
                    game->trig=0;
                }
                else if(game->State==Paused)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                    if(game->headacc==NULL)
                    {
                        game->headacc=createAcc(game);
                        game->headacc->level=game->lvl;
                        strcpy(game->headacc->name,game->PlayerName);
                        game->headacc->score=game->playerscore;
                        game->headacc->lives=game->playerlives;
                        game->nacc=game->headacc;
                    }
                    else
                        game->headacc=addAcc(game->headacc,NULL,1,game);
                }
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            break;
            case SDLK_BACKSPACE:
            {
                if(game->trig==6 && game->State==LevelCreator)
                {
                    game->lvl=0;
                    int i=1, l=strlen(game->num), j;
                    if(l>0)
                        game->num[l-1]='\0';
                    l=strlen(game->num);
                    while(l--)
                    {
                        game->lvl+=i*(game->num[l]-'0');
                        i*=10;
                    }
                    game->trig=7;
                }
                if(game->trig==0 && game->State==NameInput)
                {
                    int l=strlen(game->PlayerName);
                    if(l>0)
                        game->PlayerName[l-1]='\0';
                    game->trig=1;
                }
            }
            break;
            case SDLK_p:
            {
                if(game->balls.vx>0)
                    game->balls.vx++;
                else if(game->balls.vx<0)
                    game->balls.vx--;
                if(game->balls.vy>0)
                    game->balls.vy++;
                else if(game->balls.vy<0)
                    game->balls.vy--;
            }
            break;
            case SDLK_m:
            {
                if(game->balls.vx>0)
                    game->balls.vx--;
                else if(game->balls.vx<0)
                    game->balls.vx++;
                if(game->balls.vy>0)
                    game->balls.vy--;
                else if(game->balls.vy<0)
                    game->balls.vy++;
            }
            break;
            case SDLK_SPACE:
            {
                if(game->State==GameOver)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                else if(game->balls.vx==0 && game->balls.vy==0 && game->State == NewGame)
                {
                    game->balls.vx=2*game->res.f;
                    game->balls.vy=-2*game->res.f;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                if(game->State==Paused)
                {
                    game->prevState=game->State;
                    game->State=NewGame;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                    Mix_ResumeMusic();
                }
            }
            break;
            case SDLK_RETURN:
            {
                if(game->State==GameOver)
                {
                    highScore(game);
                    game->prevState=game->State;
                    game->State=HIGHSCORE;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                else if(game->State==HIGHSCORE)
                {
                    game->prevState=game->State;
                    game->State=MainMenu;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                else if(game->State==MainMenu)
                {
                    int i;
                    for(i=0; i<4; i++)
                        if (game->menu[i].c!=0)
                            break;
                    if(i==0)
                    {
                        game->PlayerName[0]='\0';
                        game->State=NameInput;
                        game->secs=1;
                        SDL_StartTextInput();
                        game->trig=0;
                        game->n=game->head;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else if(i==1)
                    {
                        game->prevState=game->State;
                        game->State=LoadGame;
                        game->nacc=game->headacc;
                        initloadgame(game);
                        game->trig=0;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else if(i==2)
                    {
                        game->prevState=game->State;
                        game->State=Options;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else if(i==3)
                    {
                        game->prevState=game->State;
                        game->State=Confirmquit;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                }
                else if(game->State==Confirmquit)
                {
                    if(game->menu[4].c==1)
                    {
                        if(game->prevState==NewGame)
                            game->headacc=addAcc(game->headacc,NULL,1,game);
                        game->State=Quit;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else if(game->menu[5].c==1)
                    {
                        game->State= game->prevState;
                        game->prevState=0;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                }
                else if(game->State==LevelCreator && game->trig==6)
                {
                    game->trig=0;
                    SDL_StopTextInput();
                }
                else if(game->State==NameInput && game->trig==0 && game->PlayerName[0]!='\0')
                {
                    SDL_StopTextInput();
                    game->prevState=MainMenu;
                    game->State=Loading;
                    game->trig=1;
                    game->secs=1;
                }
            }
            break;
            case SDLK_UP:
            {
                if(game->State==MainMenu)
                {
                    if( game->menu[0].c==0 && game->menu[1].c==0 && game->menu[2].c==0 && game->menu[3].c==0)
                    {
                        game->menu[0].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else
                    {
                        int j, i;
                        for(i=0; i<4; i++)
                            if (game->menu[i].c!=0)
                                j=i;
                        game->menu[j].c=0;
                        j--;
                        if(j<0)
                            j+=4;
                        game->menu[j].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                }
            }
            break;
            case SDLK_DOWN:
            {
                if(game->State==MainMenu)
                {
                    if( game->menu[0].c==0 && game->menu[1].c==0 && game->menu[2].c==0 && game->menu[3].c==0)
                    {
                        game->menu[0].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else
                    {
                        int j, i;
                        for(i=0; i<4; i++)
                            if (game->menu[i].c!=0)
                                j=i;
                        game->menu[j].c=0;
                        j++;
                        if(j>3)
                            j-=4;
                        game->menu[j].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                }
            }
            break;
            case SDLK_LEFT:
            {
                if(game->State==Confirmquit)
                {
                    if(game->menu[4].c==0 && game->menu[5].c==0)
                    {
                        game->menu[4].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else if(game->menu[4].c==0 && game->menu[5].c==1)
                    {
                        game->menu[5].c=0;
                        game->menu[4].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                }
                if(game->State==LevelCreator)
                {
                    int i,j;
                    j=(x-game->res.l)/game->blocks.w;
                    i=(y/game->blocks.h)-1;
                    if(game->brick[i][j].p==1 && game->brick[i][j].b>0)
                        game->brick[i][j].b--;
                }
                if (game->State==LoadGame && game->menu[9].c==0)
                {
                    if(game->nacc->prev!=NULL)
                    {
                        game->menu[9].c=1;
                        game->nacc=game->nacc->prev;
                        initloadgame(game);
                    }
                    else
                        game->menu[9].c=2;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                    game->trig=1;
                }
            }
            break;
            case SDLK_RIGHT:
            {
                if(game->State==Confirmquit)
                {
                    if( game->menu[4].c==0 && game->menu[5].c==0)
                    {
                        game->menu[5].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                    else
                    {
                        game->menu[4].c=0;
                        game->menu[5].c=1;
                        Mix_PlayChannel( 1, game->cSelect, 0 );
                    }
                }
                if(game->State==LevelCreator)
                {
                    int i,j;
                    j=(x-game->res.l)/game->blocks.w;
                    i=(y/game->blocks.h)-1;
                    if(game->brick[i][j].p==1 && game->brick[i][j].b<7)
                        game->brick[i][j].b++;
                }
                if (game->State==LoadGame && game->menu[11].c==0)
                {
                    if(game->nacc->next!=NULL)
                    {
                        game->menu[11].c=1;
                        game->nacc=game->nacc->next;
                        initloadgame(game);
                    }
                    else
                        game->menu[11].c=2;
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                    game->trig=1;
                }
            }
            break;
            }
        }
        break;
        case SDL_KEYUP:
        {
            case SDLK_LEFT:
            {
                if (game->State==LoadGame && game->menu[9].c!=0)
                    game->menu[9].c=0;
                game->trig=0;
            }
            break;
            case SDLK_RIGHT:
            {
                if (game->State==LoadGame && game->menu[11].c!=0)
                    game->menu[11].c=0;
                game->trig=0;
            }
            break;
        }
        break;
        case SDL_MOUSEMOTION:
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            if(game->State==Paused)
            {
                SDL_WarpMouseGlobal(game->pads.x+game->pads.w/2,game->res.h/2);
            }
            else if(game->State==Countdown || game->State==Confirmquit)
                game->mouse.x1=x;
            else if(game->State==NewGame)
            {
                game->mouse.x2=x;
                game->pads.x += (game->mouse.x2-game->mouse.x1);
                if(game->pads.x<=game->res.l)
                    game->pads.x=game->res.l;
                if(game->pads.x>=game->res.r-game->pads.w)
                    game->pads.x=game->res.r-game->pads.w;
                if(game->balls.vx==0)
                    game->balls.x=game->pads.x+game->pads.w/2-game->balls.w/2;
                SDL_WarpMouseGlobal(game->pads.x+game->pads.w/2,game->res.h/2);
                game->mouse.x1=game->pads.x+game->pads.w/2;
            }

        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            for (int i=0; i<4; i++)
            {
                if (game->State==MainMenu && x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h && event.button.button==SDL_BUTTON_LEFT)
                {
                    if(i==0)
                    {
                        game->PlayerName[0]='\0';
                        game->State=NameInput;
                        game->trig=0;
                        game->secs=1;
                        SDL_StartTextInput();
                        game->n=game->head;
                    }
                    else if (i==1)
                    {
                        game->prevState=game->State;
                        game->State=LoadGame;
                        game->nacc=game->headacc;
                        initloadgame(game);
                        game->secs=1;
                        game->trig=0;
                    }
                    else if (i==2)
                    {
                        game->prevState=game->State;
                        game->State=Options;
                    }
                    else if (i==3)
                    {
                        game->prevState=game->State;
                        game->State=Confirmquit;
                    }
                }
            }
            for (int i=4; i<6; i++)
            {
                if (game->State==Confirmquit && x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h && event.button.button==SDL_BUTTON_LEFT)
                {
                    if(i==4)
                    {
                        if(game->prevState==NewGame)
                            game->headacc=addAcc(game->headacc,NULL,1,game);
                        game->State=Quit;
                    }
                    if(i==5)
                    {
                        game->State=game->prevState;
                        game->prevState=0;
                    }
                }
            }
            for (int i=14; i<18; i++)
            {
                if (game->State==Options && x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h && event.button.button==SDL_BUTTON_LEFT)
                {
                    if(i==14)
                        if(game->bgmv>0)
                            game->bgmv--;
                    if(i==15)
                        if(game->bgmv<8)
                            game->bgmv++;
                    if(i==16)
                        if(game->sfxv>0)
                            game->sfxv--;
                    if(i==17)
                        if(game->sfxv<8)
                            game->sfxv++;
                    changeVol(game);
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
            }
            if(event.button.button==SDL_BUTTON_LEFT && game->balls.vx==0 && game->State==NewGame)
            {
                game->balls.vx=2*game->res.f;
                game->balls.vy=-2*game->res.f;
            }
            else if(event.button.button==SDL_BUTTON_LEFT && game->State==GameOver)
            {
                highScore(game);
                game->prevState=game->State;
                game->State=HIGHSCORE;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if(event.button.button==SDL_BUTTON_LEFT && game->State==HIGHSCORE)
            {
                game->prevState=game->State;
                game->State=MainMenu;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            if(event.button.button==SDL_BUTTON_LEFT && game->State==Paused)
            {
                game->prevState=game->State;
                game->State=NewGame;
                Mix_PlayChannel( 1, game->cSelect, 0 );
                Mix_ResumeMusic();
            }
            if (game->State==LevelCreator)
            {
                int i,j;
                j=(x-game->res.l)/game->blocks.w;
                i=(y/game->blocks.h)-1;
                if(i>=0 && i<=20 && j>=0 && j<=20 && event.button.button==SDL_BUTTON_LEFT)
                {
                    if(game->brick[i][j].p==0)
                        game->brick[i][j].p=1;
                    else if(game->brick[i][j].p==1)
                        game->brick[i][j].p=0;
                }
            }
            if (game->State==LoadGame)
            {
                if (game->menu[9].c==1 && (x>=game->menu[9].x && x<=game->menu[9].x+game->menu[9].w  && y>=game->menu[9].y && y<=game->menu[9].y+game->menu[9].h))
                {
                    game->nacc=game->nacc->prev;
                    initloadgame(game);
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                if (game->menu[10].c==1 && (x>=game->menu[10].x && x<=game->menu[10].x+game->menu[10].w  && y>=game->menu[10].y && y<=game->menu[10].y+game->menu[10].h))
                {
                    game->lives=game->nacc->lives;
                    game->playerlives=game->lives;
                    game->HighScore=game->nacc->score;
                    game->prevScore=game->HighScore;
                    game->playerscore=game->HighScore;
                    strcpy(game->PlayerName,game->nacc->name);
                    game->lvl=game->nacc->level;
                    game->n=game->head;
                    while(game->n->level<game->lvl)
                        game->n=game->n->next;
                    game->prevState=game->State;
                    game->State=Loading;
                    game->secs=1;
                    if(game->headacc->next==NULL && game->headacc->prev==NULL)
                    {
                        game->headacc=NULL;
                        game->nacc=game->headacc;
                    }
                    else
                        game->headacc=deleteacc(game);
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                if (game->menu[11].c==1 && (x>=game->menu[11].x && x<=game->menu[11].x+game->menu[11].w  && y>=game->menu[11].y && y<=game->menu[11].y+game->menu[11].h))
                {
                    game->nacc=game->nacc->next;
                    initloadgame(game);
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
                if (game->menu[12].c==1 && (x>=game->menu[12].x && x<=game->menu[12].x+game->menu[12].w  && y>=game->menu[12].y && y<=game->menu[12].y+game->menu[12].h))
                {
                    game->headacc=deleteacc(game);
                    initloadgame(game);
                    Mix_PlayChannel( 1, game->cSelect, 0 );
                }
            }
        }
        break;
        case SDL_QUIT:
        {
            if(game->State!=Confirmquit)
                game->prevState=game->State;
            game->State=Confirmquit;
        }

        break;
        }
    }
    int x,y;
    SDL_GetMouseState(&x,&y);
    if (game->State==Options)
    {
        for (int i=13; i<18; i++)
        {
            if (game->menu[i].c==0  && x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h)
            {
                game->menu[i].c=1;
                if(Mix_Playing(1)==0)
                    Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if(game->menu[i].c==1 && !(x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h))
                game->menu[i].c=0;
        }
    }
    if (game->State==MainMenu)
    {
        for (int i=0; i<4; i++)
        {
            if (game->menu[i].c==0  && x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h)
            {
                game->menu[i].c=1;
                if(Mix_Playing(1)==0)
                    Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if(game->menu[i].c==1 && !(x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h))
                game->menu[i].c=0;
        }
    }
    if (game->State==Confirmquit)
    {
        for (int i=4; i<6; i++)
        {
            if (game->menu[i].c==0 && (x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h))
            {
                game->menu[i].c=1;
                if(Mix_Playing(1)==0)
                    Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if (game->menu[i].c==1 && !(x>=game->menu[i].x && x<=game->menu[i].x+game->menu[i].w  && y>=game->menu[i].y && y<=game->menu[i].y+game->menu[i].h))
                game->menu[i].c=0;
        }
    }
    if (game->State==LoadGame && game->trig!=1)
    {
        if ((x>=game->menu[9].x && x<=game->menu[9].x+game->menu[9].w  && y>=game->menu[9].y && y<=game->menu[9].y+game->menu[9].h))
        {
            if(game->nacc->prev!=NULL && game->menu[9].c!=1)
            {
                game->menu[9].c=1;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if(game->nacc->prev==NULL && game->menu[9].c!=2)
            {
                game->menu[9].c=2;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
        }
        else if(!(x>=game->menu[9].x && x<=game->menu[9].x+game->menu[9].w  && y>=game->menu[9].y && y<=game->menu[9].y+game->menu[9].h))
            game->menu[9].c=0;
        if ((x>=game->menu[10].x && x<=game->menu[10].x+game->menu[10].w  && y>=game->menu[10].y && y<=game->menu[10].y+game->menu[10].h))
        {
            if(game->nacc->lives!=0 && game->menu[10].c!=1)
            {
                game->menu[10].c=1;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if(game->nacc->lives==0 && game->menu[10].c!=2)
            {
                game->menu[10].c=2;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
        }
        else if(!(x>=game->menu[10].x && x<=game->menu[10].x+game->menu[10].w  && y>=game->menu[10].y && y<=game->menu[10].y+game->menu[10].h))
            game->menu[10].c=0;
        if ((x>=game->menu[11].x && x<=game->menu[11].x+game->menu[11].w  && y>=game->menu[11].y && y<=game->menu[11].y+game->menu[11].h))
        {
            if(game->nacc->next!=NULL && game->menu[11].c!=1)
            {
                game->menu[11].c=1;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
            else if(game->nacc->next==NULL && game->menu[11].c!=2)
            {
                game->menu[11].c=2;
                Mix_PlayChannel( 1, game->cSelect, 0 );
            }
        }
        else if(!(x>=game->menu[11].x && x<=game->menu[11].x+game->menu[11].w  && y>=game->menu[11].y && y<=game->menu[11].y+game->menu[11].h))
            game->menu[11].c=0;
        if (game->menu[12].c==0 && (x>=game->menu[12].x && x<=game->menu[12].x+game->menu[12].w  && y>=game->menu[12].y && y<=game->menu[12].y+game->menu[12].h))
        {
            game->menu[12].c=1;
            if(Mix_Playing(1)==0)
                Mix_PlayChannel( 1, game->cSelect, 0 );
        }
        else if(!(x>=game->menu[12].x && x<=game->menu[12].x+game->menu[12].w  && y>=game->menu[12].y && y<=game->menu[12].y+game->menu[12].h))
            game->menu[12].c=0;
    }
    if(game->State==LevelCreator && game->trig==7)
    {
        changeLevel(game);
        game->trig=6;
    }
    if(game->State==NameInput && game->trig==1)
    {
        if(game->PlayerName[0]!='\0')
            changeName(game);
        game->trig=0;
    }
    if(game->State==NewGame)
        collisionDetect(game);
    if(game->State!=NewGame && game->State!=Confirmquit && game->State!=Paused && game->balls.vx!=0)
    {
        game->pads.x = (game->res.w-(game->pads.w))/2;
        game->pads.y = game->res.h-(game->pads.h)-10;
        game->balls.vx = 0;
        game->balls.vy = 0;
        game->balls.x = (game->res.w-(game->balls.w))/2;
        game->balls.y = game->pads.y-(game->balls.h)-1;
    }
}

void doRender(GameState *game)
{
    if (game->State == GameOver)
    {
        SDL_Rect gameoverRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->gameover, NULL, &gameoverRect);
    }
    else if (game->State == Quit)
    {
        quitGame(game);
    }
    else if (game->State == LevelCreator)
    {
        SDL_Rect backgroundRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[0], NULL, &backgroundRect);
        SDL_Rect edge = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->label[13], NULL, &edge);
        for(int i=0; i<20; i++)
        {
            for(int j=0; j<20; j++)
            {
                if(game->brick[i][j].b!=0)
                {
                    SDL_Rect blockRect = { game->brick[i][j].x, game->brick[i][j].y, game->blocks.w, game->blocks.h };
                    SDL_RenderCopy(game->renderer, game->block[game->brick[i][j].b-1], NULL, &blockRect);
                }
            }
        }
        if(game->trig==0)
        {
            SDL_Rect textr = { 20*game->res.f, 0-game->text[0].h/4, game->text[0].w, game->text[0].h };
            SDL_RenderCopy(game->renderer, game->Fonts[0], NULL, &textr);
        }
        else if(game->trig==1)
        {
            for(int i=1; i<6; i++)
            {
                SDL_Rect textr = { 20*game->res.f, (i-1)*game->text[i-1].h/2, game->text[i].w, game->text[i].h };
                SDL_RenderCopy(game->renderer, game->Fonts[i], NULL, &textr);
            }
        }
        else if(game->trig==3 || game->trig==4 ||game->trig==5)
        {
            SDL_Rect textr = { 20*game->res.f, -game->text[0].h/4, game->text[6].w, game->text[6].h };
            SDL_RenderCopy(game->renderer, game->Fonts[6], NULL, &textr);
        }
        else if(game->trig==2)
        {
            SDL_Rect textr = { 20*game->res.f, -game->text[0].h/4, game->text[7].w, game->text[7].h };
            SDL_RenderCopy(game->renderer, game->Fonts[7], NULL, &textr);
        }
        else if(game->trig==6)
        {
            SDL_Rect textr = { 20*game->res.f, -game->text[0].h/4, game->text[8].w, game->text[8].h };
            SDL_RenderCopy(game->renderer, game->Fonts[8], NULL, &textr);
        }
    }
    else if (game->State == Loading)
    {
        if(game->secs)
        {
            time(&time1);
            game->secs=0;
        }
        time(&time2);
        double diff_t=difftime(time2,time1);
        if(diff_t<3.0)
        {
            SDL_Rect loadingRect = { 0, 0, game->res.w, game->res.h };
            SDL_RenderCopy(game->renderer, game->background[3], NULL, &loadingRect);
            SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
            int i=(diff_t*25)+1;
            SDL_Rect loadingbar = { game->load.x, game->load.y, game->load.w*i, game->load.h };
            SDL_RenderFillRect(game->renderer, &loadingbar);
        }
        else if (diff_t>=3.0)
        {
            SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
            SDL_RenderClear(game->renderer);
            game->prevState=game->State;
            if(game->stateload[LevelCreator]==1)
            {
                game->State=LevelCreator;
                game->stateload[LevelCreator]=0;
                initbricks(game);
            }
            else
            {
                game->State=Countdown;
                initbricks(game);
                game->secs=1;
            }
            game->secs=1;
        }
    }
    else if (game->State == NewGame || game->State==Countdown || game->State==Paused)
    {
        SDL_Rect backgroundRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[0], NULL, &backgroundRect);
        SDL_Rect edge = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->label[13], NULL, &edge);
        for(int i=0; i<game->lives; i++)
        {
            SDL_Rect heartRect = { game->heart[i].x, game->heart[i].y,  game->heart[i].w,  game->heart[i].h };
            SDL_RenderCopy(game->renderer, game->hearts, NULL, &heartRect);
        }
        for(int i=0; i<20; i++)
        {
            for(int j=0; j<20; j++)
            {
                if(game->brick[i][j].b!=0)
                {
                    SDL_Rect blockRect = { game->brick[i][j].x, game->brick[i][j].y, game->blocks.w, game->blocks.h };
                    SDL_RenderCopy(game->renderer, game->block[game->brick[i][j].b-1], NULL, &blockRect);
                }
            }
        }
        SDL_Rect padRect = { game->pads.x, game->pads.y, game->pads.w, game->pads.h };
        SDL_RenderCopy(game->renderer, game->pad, NULL, &padRect);
        SDL_Rect ballRect = { game->balls.x, game->balls.y, game->balls.w, game->balls.h };
        SDL_RenderCopy(game->renderer, game->ball, NULL, &ballRect);
        if(game->prevScore<game->HighScore)
            game->prevScore++;
        int score=game->prevScore, k=log10(game->prevScore), y, l;
        if(k<0)
            k=0;
        l=k;
        y=(game->res.h-(k+1)*game->score[0].h)/2;
        for(int i=7; i>=(7-k); i--)
        {
            SDL_Rect numRect = { game->score[i].x, y+l*game->score[i].h, game->score[i].w, game->score[i].h };
            SDL_RenderCopy(game->renderer, game->number[score%10], NULL, &numRect);
            score/=10;
            l--;
        }
        if(game->secs)
        {
            time(&time1);
            game->secs=0;
        }
        if(game->State==Countdown)
        {
            time(&time2);
            SDL_WarpMouseInWindow(game->window,game->res.w/2,game->res.h/2);
            double diff_t=difftime(time2,time1);
            int i=diff_t;
            if (diff_t>=3.0)
            {
                game->prevState=game->State;
                game->State=NewGame;
            }
            else
            {
                SDL_Rect countRect = { game->counts.x, game->counts.y, game->counts.w, game->counts.h };
                SDL_RenderCopy(game->renderer, game->Count[2-i], NULL, &countRect);
            }
        }
        if(game->State==Paused)
        {
            SDL_Rect pauseRect = { game->menu[7].x, game->menu[7].y, game->menu[7].w, game->menu[7].h };
            SDL_RenderCopy(game->renderer, game->label[14], NULL, &pauseRect);
        }
    }
    else if (game->State == MainMenu)
    {
        SDL_Rect backgroundRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[1], NULL, &backgroundRect);
        for (int i=0; i<4; i++)
        {
            SDL_Rect rect = { game->menu[i].x, game->menu[i].y, game->menu[i].w, game->menu[i].h };
            if(game->menu[i].c)
                SDL_RenderCopy(game->renderer, game->label[4+i], NULL, &rect);
            else
                SDL_RenderCopy(game->renderer, game->label[i], NULL, &rect);
        }
    }
    else if (game->State == NameInput)
    {
        SDL_Rect backgroundRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[7], NULL, &backgroundRect);
        SDL_Rect nameRect = { game->menu[8].x, game->menu[8].y, game->menu[8].w, game->menu[8].h };
        SDL_RenderCopy(game->renderer, game->label[15], NULL, &nameRect);
        if(game->secs)
        {
            time(&time1);
            game->secs=0;
        }
        time(&time2);
        double diff_t=difftime(time2,time1);
        int i=diff_t;
        if(game->PlayerName[0]!='\0')
        {
            SDL_Rect text1Rect = { game->text[40].x, game->text[40].y, game->text[40].w, game->text[40].h };
            SDL_RenderCopy(game->renderer, game->Fonts[40], NULL, &text1Rect);
        }
        if (i%2)
        {
            int x;
            if(game->PlayerName[0]=='\0')
                x = game->res.w/2;
            else
                x = game->text[40].x+game->text[40].w;
            SDL_Rect textRect = { x, 570*game->res.f, game->text[9].w, game->text[9].h };
            SDL_RenderCopy(game->renderer, game->Fonts[9], NULL, &textRect);
        }
    }
    else if (game->State == LoadGame)
    {
        SDL_Rect backgroundRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[7], NULL, &backgroundRect);
        SDL_Rect background1Rect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[6], NULL, &background1Rect);
        if(game->menu[9].c==1)
        {
            SDL_Rect rRect = { game->menu[9].x, game->menu[9].y, game->menu[9].w, game->menu[9].h };
            SDL_RenderCopy(game->renderer, game->label[16], NULL, &rRect);
        }
        else if(game->menu[9].c==2)
        {
            SDL_Rect rRect = { game->menu[9].x, game->menu[9].y, game->menu[9].w, game->menu[9].h };
            SDL_RenderCopy(game->renderer, game->label[17], NULL, &rRect);
        }
        else if(game->menu[10].c==1)
        {
            SDL_Rect rRect = { game->menu[10].x, game->menu[10].y, game->menu[10].w, game->menu[10].h };
            SDL_RenderCopy(game->renderer, game->label[18], NULL, &rRect);
        }
        else if(game->menu[10].c==2)
        {
            SDL_Rect rRect = { game->menu[10].x, game->menu[10].y, game->menu[10].w, game->menu[10].h };
            SDL_RenderCopy(game->renderer, game->label[19], NULL, &rRect);
        }
        else if(game->menu[11].c==1)
        {
            SDL_Rect rRect = { game->menu[11].x, game->menu[11].y, game->menu[11].w, game->menu[11].h };
            SDL_RenderCopy(game->renderer, game->label[20], NULL, &rRect);
        }
        else if(game->menu[11].c==2)
        {
            SDL_Rect rRect = { game->menu[11].x, game->menu[11].y, game->menu[11].w, game->menu[11].h };
            SDL_RenderCopy(game->renderer, game->label[21], NULL, &rRect);
        }
        if(game->menu[12].c==0)
        {
            SDL_Rect rRect = { game->menu[12].x, game->menu[12].y, game->menu[12].w, game->menu[12].h };
            SDL_RenderCopy(game->renderer, game->label[22], NULL, &rRect);
        }
        else if(game->menu[12].c==1)
        {
            SDL_Rect rRect = { game->menu[12].x, game->menu[12].y, game->menu[12].w, game->menu[12].h };
            SDL_RenderCopy(game->renderer, game->label[23], NULL, &rRect);
        }
        for(int i=41; i<45; i++)
        {
            SDL_Rect tRect = { game->text[i].x, game->text[i].y, game->text[i].w, game->text[i].h };
            SDL_RenderCopy(game->renderer, game->Fonts[i], NULL, &tRect);
        }
    }
    else if (game->State == Confirmquit)
    {
        SDL_Rect confirmRect= { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[2], NULL, &confirmRect);
        SDL_Rect yesno = { game->menu[6].x, game->menu[6].y, game->menu[6].w, game->menu[6].h };
        SDL_RenderCopy(game->renderer, game->label[12], NULL, &yesno);
        SDL_Rect yes = { game->menu[4].x, game->menu[4].y, game->menu[4].w, game->menu[4].h };
        if(!game->menu[4].c)
            SDL_RenderCopy(game->renderer, game->label[8], NULL, &yes);
        else
            SDL_RenderCopy(game->renderer, game->label[10], NULL, &yes);
        SDL_Rect no = { game->menu[5].x, game->menu[5].y, game->menu[5].w, game->menu[5].h };
        if(!game->menu[5].c)
            SDL_RenderCopy(game->renderer, game->label[9], NULL, &no);
        else
            SDL_RenderCopy(game->renderer, game->label[11], NULL, &no);
    }
    else if (game->State == HIGHSCORE)
    {
        SDL_Rect hRect= { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[4], NULL, &hRect);
        int y=320*game->res.f, x1=312*game->res.f,x2=game->res.w/2,x3=1620*game->res.f, y1=8*game->res.f;
        for(int i=10, j=0; i<((game->cnt1*3)+10); i+=3, j++)
        {
            SDL_Rect oRect= { x1, y+(j*game->text[i].h), game->text[i].w, game->text[i].h };
            SDL_RenderCopy(game->renderer, game->Fonts[i], NULL, &oRect);
            SDL_Rect tRect= { x2-(game->text[i+1].w/2), y+(j*game->text[i].h)+y1, game->text[i+1].w, game->text[i+1].h };
            SDL_RenderCopy(game->renderer, game->Fonts[i+1], NULL, &tRect);
            SDL_Rect thRect= { x3-game->text[i+2].w, y+(j*game->text[i].h)+y1, game->text[i+2].w, game->text[i+2].h };
            SDL_RenderCopy(game->renderer, game->Fonts[i+2], NULL, &thRect);
        }
    }
    else if (game->State == Options)
    {
        SDL_Rect hRect= { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[8], NULL, &hRect);
        int i;
        for(i=13;i<18;i++)
        {
            if(game->menu[i].c==1)
            {
                SDL_Rect m= { game->menu[i].x, game->menu[i].y, game->menu[i].w, game->menu[i].h };
                int j=0;
                if(i==13)
                    j=24;
                if(i==14 || i==16)
                    j=26;
                if(i==15 || i==17)
                    j=27;
                SDL_RenderCopy(game->renderer, game->label[j], NULL, &m);
            }
        }
        for(i=0;i<game->bgmv;i++)
        {
            SDL_Rect sRect= { game->menu[18].x+(i*game->menu[18].w), game->menu[18].y, game->menu[18].w, game->menu[18].h };
            SDL_RenderCopy(game->renderer, game->label[25], NULL, &sRect);
        }
        for(i=0;i<game->sfxv;i++)
        {
            SDL_Rect sRect= { game->menu[19].x+(i*game->menu[19].w), game->menu[19].y, game->menu[19].w, game->menu[19].h };
            SDL_RenderCopy(game->renderer, game->label[25], NULL, &sRect);
        }
    }
    playMusic(game);
    SDL_RenderPresent(game->renderer);
}
int main(int argc, char *argv[])
{
    GameState game;
    boot(&game);
    while(1)
    {
        processEvents(&game);
        doRender(&game);
        if( game.State==NewGame || game.State==Loading || game.State==Countdown || game.State==Paused )
            SDL_ShowCursor(SDL_DISABLE);
        else
            SDL_ShowCursor(SDL_ENABLE);
    }
    quitGame(&game);
    return 0;
}

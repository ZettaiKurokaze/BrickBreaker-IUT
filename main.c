#include "GameState.h"
void loadGame(GameState *game)
{
    for (int i=0; i<8; i++)
        game->menu[i].c=0;
    game->State=MainMenu;
    game->prevState=0;
    mainmenu(game);
    newgame(game);
    gameover(game);
    loading(game);
    confirmquit(game);
    countdown(game);
    game->secs=0;
}

void quitGame(GameState *game)
{
    SDL_DestroyTexture(game->ball);
    SDL_DestroyTexture(game->gameover);
    SDL_DestroyTexture(game->pad);
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
    for(int i=0; i<14; i++)
    {
        SDL_DestroyTexture(game->label[i]);
        game->label[i] = NULL;
    }
    for(int i=0; i<3; i++)
        SDL_DestroyTexture(game->background[i]);
    for(int i=0; i<3; i++)
        SDL_DestroyTexture(game->Count[i]);
    for(int i=0; i<7; i++)
        SDL_DestroyTexture(game->block[i]);
    TTF_CloseFont(game->font);
    TTF_Quit();
    SDL_Quit();
    exit(0);
}
void processEvents(GameState *game)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
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
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                if(game->State==MainMenu)
                {
                    game->prevState=game->State;
                    game->State=Confirmquit;
                }
                else if(game->State==NewGame)
                    game->State=MainMenu;
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
                    game->State=Countdown;
                    game->secs=1;
                }
                else if(game->balls.vx==0 && game->balls.vy==0 && game->State == NewGame)
                {
                    game->balls.vx=2;
                    game->balls.vy=-2;
                }
            }
            break;
            case SDLK_RETURN:
            {
                if(game->State==GameOver)
                {
                    game->State=Countdown;
                    game->secs=1;
                }
                else if(game->State==MainMenu)
                {
                    int i;
                    for(i=0; i<4; i++)
                        if (game->menu[i].c!=0)
                            break;
                    if(i==0)
                    {
                        game->State=Loading;
                        game->secs=1;
                    }
                    if(i==3)
                    {
                        game->prevState=game->State;
                        game->State=Confirmquit;
                    }
                }
                else if(game->State==Confirmquit)
                {
                    if(game->menu[4].c==1)
                        game->State=Quit;
                    else if(game->menu[5].c==1)
                        game->State= game->prevState;
                }
            }
            break;
            case SDLK_UP:
            {
                if(game->State==MainMenu)
                    {
                        if( game->menu[0].c==0 && game->menu[1].c==0 && game->menu[2].c==0 && game->menu[3].c==0)
                            game->menu[0].c=1;
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
                        }
                    }
            }
            break;
            case SDLK_DOWN:
            {
                if(game->State==MainMenu)
                    {
                        if( game->menu[0].c==0 && game->menu[1].c==0 && game->menu[2].c==0 && game->menu[3].c==0)
                            game->menu[0].c=1;
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
                        }
                    }
            }
            break;
            case SDLK_LEFT:
            {
                if(game->State==Confirmquit)
                    {
                        if(game->menu[4].c==0 && game->menu[5].c==0)
                            game->menu[4].c=1;
                        else if(game->menu[4].c==0 && game->menu[5].c==1)
                        {
                            game->menu[5].c=0;
                            game->menu[4].c=1;
                        }
                    }
            }
            break;
            case SDLK_RIGHT:
            {
                if(game->State==Confirmquit)
                    {
                        if( game->menu[4].c==0 && game->menu[5].c==0)
                            game->menu[5].c=1;
                        else
                        {
                            game->menu[4].c=0;
                            game->menu[5].c=1;
                        }
                    }
            }
            break;
            }
        }
        break;
        case SDL_MOUSEMOTION:
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            if(game->State==Countdown || game->State==Confirmquit)
                game->mouse.x1=x;
            if(game->State==NewGame)
            {
                game->mouse.x2=x;
                game->pads.x += (game->mouse.x2-game->mouse.x1);
                if(game->pads.x<=game->res.l)
                    game->pads.x=game->res.l;
                if(game->pads.x>=game->res.r-game->pads.w)
                    game->pads.x=game->res.r-game->pads.w;
                if(game->balls.vx==0)
                    game->balls.x=game->pads.x+game->pads.w/2-game->balls.w/2;
                game->mouse.x1=game->mouse.x2;
            }
            if (game->State==MainMenu)
            {
                for (int i=0; i<4; i++)
                {
                    if (event.button.x>=game->menu[i].x && event.button.x<=game->menu[i].x+game->menu[i].w  && event.button.y>=game->menu[i].y+15 && event.button.y<=game->menu[i].y+game->menu[i].h-12)
                        game->menu[i].c=1;
                    else
                        game->menu[i].c=0;
                }
            }
            if (game->State==Confirmquit)
            {
                for (int i=4; i<6; i++)
                {
                    if (event.button.x>=game->menu[i].x && event.button.x<=game->menu[i].x+game->menu[i].w  && event.button.y>=game->menu[i].y+15 && event.button.y<=game->menu[i].y+game->menu[i].h-12)
                        game->menu[i].c=1;
                    else
                        game->menu[i].c=0;
                }
            }
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            for (int i=0; i<4; i++)
            {
                if (game->State==MainMenu && event.button.x>=game->menu[i].x && event.button.x<=game->menu[i].x+game->menu[i].w  && event.button.y>=game->menu[i].y+15 && event.button.y<=game->menu[i].y+game->menu[i].h-12 && event.button.button==SDL_BUTTON_LEFT)
                {
                    if(i==0)
                    {
                        game->State=Loading;
                        game->secs=1;
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
                if (game->State==Confirmquit && event.button.x>=game->menu[i].x && event.button.x<=game->menu[i].x+game->menu[i].w  && event.button.y>=game->menu[i].y+15 && event.button.y<=game->menu[i].y+game->menu[i].h-12 && event.button.button==SDL_BUTTON_LEFT)
                {
                    if(i==4)
                        game->State=Quit;
                    if(i==5)
                        game->State=game->prevState;
                }
            }
            if(event.button.button==SDL_BUTTON_LEFT && game->balls.vx==0 && game->State==NewGame)
            {
                game->balls.vx=2;
                game->balls.vy=-2;
            }
            else if(event.button.button==SDL_BUTTON_LEFT && game->State==GameOver)
            {
                game->State=Countdown;
                game->secs=1;
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
    if(game->State==NewGame)
        collisionDetect(game);
}

void doRender(GameState *game)
{
    if (game->State == GameOver)
    {
        SDL_Rect gameoverRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->gameover, NULL, &gameoverRect);
    }
    else if (game->State == Quit)
        quitGame(game);
    else if (game->State == Loading)
    {
        if(game->secs)
        {
            time(&time1);
            game->secs=0;
        }
        time(&time2);
        double diff_t=difftime(time2,time1);
        if(diff_t<4.0)
        {
            SDL_Rect loadingRect = { 0, 0, game->res.w, game->res.h };
            SDL_RenderCopy(game->renderer, game->background[8], NULL, &loadingRect);
            SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
            int i=(diff_t*25)+1;
            SDL_Rect loadingbar = { game->load.x, game->load.y, game->load.w*i, game->load.h };
            SDL_RenderFillRect(game->renderer, &loadingbar);
        }
        else if (diff_t>=4.0)
        {
            SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
            SDL_RenderClear(game->renderer);
            game->State=Countdown;
            game->secs=1;
        }
    }
    else if (game->State == NewGame || game->State==Countdown)
    {
        SDL_Rect backgroundRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[0], NULL, &backgroundRect);
        for(int i=0; i<20; i++)
        {
            for(int j=0; j<20; j++)
            {
                if(game->brick[i][j].b!=7)
                {
                    SDL_Rect blockRect = { game->brick[i][j].x, game->brick[i][j].y, game->blocks.w, game->blocks.h };
                    SDL_RenderCopy(game->renderer, game->block[game->brick[i][j].b], NULL, &blockRect);
                }
            }
        }
        SDL_Rect padRect = { game->pads.x, game->pads.y, game->pads.w, game->pads.h };
        SDL_RenderCopy(game->renderer, game->pad, NULL, &padRect);
        SDL_Rect ballRect = { game->balls.x, game->balls.y, game->balls.w, game->balls.h };
        SDL_RenderCopy(game->renderer, game->ball, NULL, &ballRect);
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
            if (diff_t>=3)
                game->State=NewGame;
            else
            {
                int i=diff_t;
                SDL_Rect countRect = { game->counts.x, game->counts.y, game->counts.w, game->counts.h };
                SDL_RenderCopy(game->renderer, game->Count[2-i], NULL, &countRect);
            }
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
    else if (game->State == Confirmquit)
    {
        double size=100*game->res.f;
        SDL_Rect confirmRect = { 0, 0, game->res.w, game->res.h };
        SDL_RenderCopy(game->renderer, game->background[2], NULL, &confirmRect);
        SDL_Rect yesno = { (game->res.w-(size*4))/2, (game->res.h-(size*3))/2, size*4, size*3 };
        SDL_RenderCopy(game->renderer, game->confirm, NULL, &yesno);
        SDL_Rect are = { game->menu[6].x, game->menu[6].y, game->menu[6].w, game->menu[6].h };
        SDL_RenderCopy(game->renderer, game->label[12], NULL, &are);
        SDL_Rect you = { game->menu[7].x, game->menu[7].y, game->menu[7].w, game->menu[7].h };
        SDL_RenderCopy(game->renderer, game->label[13], NULL, &you);
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
    SDL_RenderPresent(game->renderer);
}
int main(int argc, char *argv[])
{
    srand((int)time(NULL));
	//ShowWindow(GetConsoleWindow(),SW_HIDE);
    GameState game;
    game.res.f=.75;
    game.res.w=1920*game.res.f;
    game.res.h=1080*game.res.f;
    game.res.l=240*game.res.f;
    game.res.r=1680*game.res.f;
    game.window = NULL;
    game.renderer = NULL;
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    srand((int)time(NULL));
    game.window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game.res.w, game.res.h, 0);
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    loadGame(&game);
    SDL_SetWindowGrab(game.window,SDL_TRUE);
    while(1)
    {
        processEvents(&game);
        doRender(&game);
        if( game.State==NewGame || game.State==Loading || game.State==Countdown )
            SDL_ShowCursor(SDL_DISABLE);
        else
            SDL_ShowCursor(SDL_ENABLE);
    }
    quitGame(&game);
    return 0;
}

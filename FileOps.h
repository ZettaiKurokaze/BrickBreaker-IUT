#include "GameState.h"
node createNode()
{
    node temp;
    temp=(node)malloc(sizeof(struct linkedlist));
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}
acc createAcc()
{
    acc temp;
    temp=(acc)malloc(sizeof(struct acclist));
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}
acc addAcc(acc head, FILE *fp, int t, GameState *game)
{
    acc temp=createAcc(), p, n;
    char c;
    int i, l;
    if(t==0)
    {
        fscanf(fp, "%s%c%d%c%d%c%d%c", &temp->name,&c, &temp->level,&c, &temp->score,&c, &temp->lives,&c);
    }
    else
    {
        temp->level=game->lvl;
        strcpy(temp->name,game->PlayerName);
        temp->score=game->playerscore;
        temp->lives=game->playerlives;
    }
    n=head;
    while(n->score>=temp->score && n->next!=NULL)
    {
        p=n;
        n=n->next;
    }
    if(temp->score<=n->score && n->next==NULL)
    {
        temp->prev=n;
        n->next=temp;
    }
    else if(temp->score>=n->score && n->prev==NULL)
    {
        temp->next=n;
        n->prev=temp;
        head=temp;
    }
    else if(temp->score>n->score)
    {
        p->next=temp;
        temp->prev=p;
        temp->next=n;
        n->prev=temp;
    }
    return head;
}
node addNode(node head, int val, FILE *fp, int t, GameState *game)
{
    node temp=createNode(), p, n;
    int i,j;
    char c;
    temp->level=val;
    if(t==0)
    {
        for(i=0; i<20; i++)
        {
            for(j=0; j<20; j++)
            {
                fscanf(fp, "%d%c", &temp->b[i][j],&c);
            }
        }
    }
    else if(t==3)
    {
        for(i=0; i<20; i++)
        {
            for(j=0; j<20; j++)
                temp->b[i][j]=game->brick[i][j].b;
        }
    }
    n=head;
    if(t==0 || t==3)
    {
        if(val<=n->level)
        {
            temp->next=n;
            n->prev=temp;
            head=temp;
            n->level++;
            while(n->next!=NULL)
            {
                n=n->next;
                n->level++;
            }
            return head;
        }
        while(n->level<val && n->next!=NULL)
        {
            p=n;
            n=n->next;
        }
        if(n->level>val)
        {

            p->next=temp;
            temp->prev=p;
            temp->next=n;
            n->prev=temp;
        }
        else if(n->level<val)
        {
            temp->prev=n;
            n->next=temp;
        }
        else if(n->level==val)
        {
            p->next=temp;
            temp->prev=p;
            temp->next=n;
            n->prev=temp;
            n->level++;
            while(n->next!=NULL)
            {
                n=n->next;
                n->level++;
            }
        }
        return head;
    }
    if(t==4)
    {
        if(val==head->level)
        {
            for(i=0; i<20; i++)
            {
                for(j=0; j<20; j++)
                    n->b[i][j]=game->brick[i][j].b;
            }
            return head;
        }
        while(n->level<val && n->next!=NULL)
        {
            p=n;
            n=n->next;
        }
        if(n->level==val)
        {
            for(i=0; i<20; i++)
            {
                for(j=0; j<20; j++)
                    n->b[i][j]=game->brick[i][j].b;
            }
            return head;
        }
        else
        {
            game->trig=2;
            return head;
        }
    }
    if(t==5)
    {
        if(val==head->level)
        {
            n=n->next;
            n->prev=NULL;
            head=n;
            n->level--;
            while(n->next!=NULL)
            {
                n=n->next;
                n->level--;
            }
            return head;
        }
        while(n->level<val && n->next!=NULL)
        {
            p=n;
            n=n->next;
        }
        if(n->level==val && n->next!=NULL)
        {
            if(n->next==NULL)
            {
                p->next=NULL;
                return head;
            }
            n=n->next;
            p->next=n;
            n->prev=p;
            n->level--;
            while(n->next!=NULL)
            {
                n=n->next;
                n->level--;
            }
            return head;
        }
        else
        {
            game->trig=2;
            return head;
        }
    }
    return head;
}
void initLevels(GameState *game)
{
    FILE *fp= fopen("Resources/Files/levels.txt", "r");
    game->head=NULL;
    int val, i, j;
    char c=1;
    while(c!=EOF)
    {
        if(game->head==NULL)
        {
            game->head=createNode();
            fscanf(fp, "%d%c", &game->head->level, &c);
            for(i=0; i<20; i++)
            {
                for(j=0; j<20; j++)
                {
                    fscanf(fp, "%d%c", &game->head->b[i][j], &c);
                }
            }
        }
        else if(game->head!=NULL)
        {
            fscanf(fp, "%d%c", &val, &c);
            if(feof(fp))
                break;
            game->head=addNode(game->head, val, fp, 0, game);
        }
        if(feof(fp))
            break;
    }
    fclose(fp);
}
void initAccounts(GameState *game)
{
    FILE *fp= fopen("Resources/Files/accounts.txt", "r");
    game->headacc=NULL;
    char c=1,d,e;
    while(c!=EOF)
    {
        if(game->headacc==NULL)
        {
            game->headacc=createAcc();
            fscanf(fp, "%s%c%d%c%d%c%d%c", &game->headacc->name,&c, &game->headacc->level,&c, &game->headacc->score,&c, &game->headacc->lives,&c);
        }
        else if(game->headacc!=NULL)
        {
            game->headacc=addAcc(game->headacc, fp, 0, game);
        }
        if(feof(fp))
            break;
    }
//    acc n=game->headacc;
//    while(n!=NULL)
//    {
//        printf("%s %d %d %d\n", n->name, n->level, n->score, n->lives);
//        if(n->next==NULL)
//            break;
//        n=n->next;
//    }
//    fclose(fp);
}
void saveLevels(GameState *game)
{
    FILE *fp;
    fp = fopen("Resources/Files/levels.txt", "w");
    node n;
    n=game->head;
    int i, j;
    while(n!=NULL)
    {
        fprintf(fp, "%d\n", n->level);
        for(i=0; i<20; i++)
        {
            for(j=0; j<20; j++)
            {
                fprintf(fp, "%d", n->b[i][j]);
                if(j!=19)
                    fprintf(fp, " ");
            }
            fprintf(fp, "\n");
        }
        if(n->next==NULL)
            break;
        n=n->next;
    }
    fclose(fp);
}
void saveAccounts(GameState *game)
{
    FILE *fp;
    fp = fopen("Resources/Files/accounts.txt", "w");
    acc n;
    n=game->headacc;
    while(n!=NULL)
    {
        fprintf(fp, "%s %d %d %d", n->name, n->level, n->score, n->lives);
        if(n->next==NULL)
            break;
        fprintf(fp,"\n");
        n=n->next;
    }
    fclose(fp);
}
void initbricks(GameState *game)
{
    int i, j;
    if(game->State==Countdown)
    {
        game->lvl=game->n->level;
        game->cnt=0;
        for(i=0; i<20; i++)
        {
            for(j=0; j<20; j++)
            {
                game->brick[i][j].b=game->n->b[i][j];
                game->brick[i][j].p=0;
                if( game->brick[i][j].b!=0)
                    game->cnt++;
            }
        }
        game->n=game->n->next;
    }
    else if(game->State==LevelCreator)
    {
        game->lvl=game->lcn->level;
        changeLevel(game);
        for(i=0; i<20; i++)
        {
            for(j=0; j<20; j++)
            {
                game->brick[i][j].b=game->lcn->b[i][j];
                game->brick[i][j].p=0;
            }
        }
    }
}
acc deleteacc(GameState *game)
{
    if(game->nacc->next==NULL && game->nacc->prev==NULL)
    {
        return game->headacc;
    }
    else if(game->nacc->next==NULL)
    {
        game->nacc=game->nacc->prev;
        game->nacc->next=NULL;
    }
    else if(game->nacc->prev==NULL)
    {
        game->nacc=game->nacc->next;
        game->nacc->prev=NULL;
        game->headacc=game->nacc;
    }
    else
    {
        acc p=game->nacc->prev;
        game->nacc=game->nacc->next;
        p->next=game->nacc;
        game->nacc->prev=p;
    }
    return game->headacc;
}

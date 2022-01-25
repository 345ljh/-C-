#include "actions.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>

///���ļ���game����Ϊȫ�ֱ�������

//�ƶѳ�ʼ��
void PileInit(void)
{
    int temp[160];
    int pointer;

    memset(temp, 0, sizeof(temp));

    for(int i = 0; i <= 159; i++)
    {
        pointer = rand() % 160;
        while(temp[pointer]) ++pointer %= 160;
        game.card[i] = pointer;
        temp[pointer] = 1;
    }

    game.nowpile = 160;
}

//ϴ��
void Shuffle(void)
{
    int pilecards = 0;
    int pointer;
    int* temp = NULL;
    temp = (int*)calloc(pilecards, sizeof(int));
    memset(temp, 0, sizeof(int) * pilecards);

    //ͳ�����ƶ����Ƶ�����
    for(int i = 0; i <= 150; i++)
    {
        if(card_inf[i].owner == -1) pilecards++;
    }

    //�˲�����Pileinit����
    for(int i = 0; i <= pilecards - 1; i++)
    {
        pointer = rand() % 160;
        while(temp[pointer]) ++pointer %= pilecards;
        game.card[i] = pointer;
        temp[pointer] = 1;

    }

    game.nowpile = pilecards;
    free(temp);
}

//���ƶѶ�����
void Takecard(player_t player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        if(card_inf[0].owner == -1) Shuffle();
        card_inf[game.card[0]].owner = player.id;
        player.card[player.cardamount] = game.card[0];
        player.cardamount++;
        game.nowpile--;

        for(int j = 0; j <= game.nowpile - 1; j++)
        {
            game.card[j] = game.card[j + 1];
        }
        game.card[game.nowpile] = -1;

        /* skills here */
    }
}

//����,����executorΪ������,playerΪ��������
void Throwcard(player_t executor, player_t player, int amount)
{
    for(int i = 1; i <= amount; i++)
    {
        int selected = 0;
        //TODO: a function that the executor choose a card's id of player to throw
        card_inf[player.card[selected]].owner = -1;
        player.cardamount--;  //haven't added throwing cards in equipments or judge areas here

        for(int j = selected; j <= player.cardamount - 1; j++)
        {
            player.card[j] = player.card[j + 1];
        }
        player.card[player.cardamount] = -1;

        /* skills here */
    }
}

//չʾ����
///amount=-1��ʾȫ��չʾ
void Showcard(player_t executor, player_t player, int amount)
{
    if(amount != -1)
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /* select cards */
        }
        for(int j = 0; j <= amount - 1; j++)
        {
            /* output thr information of player.card[selected[j]] */
        }
    }
}

//���������ɫ��
///Ĭ��type=0��ʾexecutor���player����,type=1��ʾexecutor����player��
//TODO: amount=-1 stands for getting or giving all cards
void Getcard(player_t executor, player_t player, int amount, int type = 0)
{
    if(type)
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /*
            card_inf[executor.card[selected[j]]] = player.id;
            player.card[player.cardamount] = executor.card[selected[j]];
            executor.card[selected[j]] = -1;
            */
            executor.cardamount--;
            player.cardamount++;
        }
        /* skills here */
    }
    else
    {
        /*
        int* selected = NULL;
        selected = (int*)calloc(amount, sizeof(int));
        */
        for(int i = 1; i <= amount; i++)
        {
            /*
            card_inf[player.card[selected[j]]] = executor.id;
            executor.card[executor.cardamount] = player.card[selected[j]];
            player.card[selected[j]] = -1;
            */
            player.cardamount--;
            executor.cardamount++;
        }
        /* skills here */
    }
}

//����˺�
///executor=0��ʾ�˺�����Դ(������),=-1��ʾʧȥ����
void Damage(player_t executor, player_t player, int amount, damage_e type)
{
    player.health -= amount;
    if(player.health <= 0)
    {
        Neardeath(player);
    }
    /* skills here */
    /* chain here */
}

//��������
void Neardeath(player_t player)
{
    /* skills here */
    for(int i = 0; i <= PLAYERS; i++)
    {
        do
        {
            /*  ask player that id = (player.id + i) % PLAYERS for a peach */
            if(player.health > 0) return;
        }while(0/* used a peach */);
    }

    if(player.health < 0) Death(player);
}

//��������
void Death(player_t player)
{
    player.controller = DEAD;
    /* VictoryJudge(); */

    for(int i = 0; i <= player.cardamount - 1; i++)
    {
        card_inf[player.card[i]].owner = -1;
    }

    Takecard(game.player[4 - player.id], 1);
}

//ʤ�������ж�
void VictoryJudge(void)
{
    if(game.player[0].controller == DEAD && game.player[3].controller == DEAD)  //1����4�ž�����
    {
        if(game.humanid != 0 && game.humanid != 3)  /* victory */;  //1����4�ž�Ϊ����
        else /* failed */;
    }
    else if( game.player[1].controller == DEAD && game.player[2].controller == DEAD)
    {
        if(game.humanid != 1 && game.humanid != 2)  /* victory */;
        else /* failed */;
    }
}

//ʹ����
void Usecard(player_t executor)
{
    int card = 0;
    /* choose card */
    switch( (type_e)card)
    {
        case SHA:
        case HUOSHA:
        case LEISHA:
            {
                /* choose target */
                /* calculate distance */
                /* ask for SHAN */
                /* if no, Damage(executor, game.player[target], damage, (damage_e)((int)card_inf[executor.card[card]].type & 3) */
                break;
            }
        case TAO:
            {
                if(executor.health < executor.maxhealth)
                {
                    executor.health++;
                }
                else /* failed to use */;
                break;
            }
        case JIU:
            {
                if(!executor.spirits) executor.spirits = 1;
                else /* failed to use */;
                break;
            }
        case JUEDOU:
            {
                /* choose target */
                /* while ask for SHA */
                /* Damage(executor/target, another one, 1, COMMON) */
                break;
            }
        default:
            //TODO
            break;
    }

}
